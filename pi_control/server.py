#!/usr/bin/env python3
"""WebSocket server for RoverForge - bridges browser UI to Arduino serial."""
import asyncio
import json
from aiohttp import web, WSMsgType
import serial
import os

SERIAL_PORT = os.getenv("ARDUINO_PORT", "/dev/ttyACM0")
BAUD = int(os.getenv("ARDUINO_BAUD", "9600"))
WEB_PORT = int(os.getenv("WEB_PORT", "8080"))

routes = web.RouteTableDef()
clients = set()

# open serial
ser = serial.Serial(SERIAL_PORT, BAUD, timeout=0.1)

@routes.get("/")
async def index(request):
    return web.FileResponse(os.path.join(os.path.dirname(__file__), "static", "index.html"))

@routes.get("/ws")
async def websocket_handler(request):
    ws = web.WebSocketResponse()
    await ws.prepare(request)
    clients.add(ws)
    try:
        async for msg in ws:
            if msg.type == WSMsgType.TEXT:
                cmd = msg.data.strip()
                if cmd:
                    try:
                        ser.write(cmd.encode())
                    except Exception:
                        pass
            elif msg.type == WSMsgType.ERROR:
                break
    finally:
        clients.discard(ws)
    return ws

def parse_telemetry_line(line):
    """Parse telemetry lines into structured JSON.
    
    Supports formats:
    - CSV: "T,L:200,R:200,D:25,B:12.3"
    - Returns: {"type": "telemetry", "data": {"L": 200, "R": 200, "D": 25, "B": 12.3}}
    """
    if not line.startswith("T,"):
        return None
    
    data = {}
    parts = line[2:].split(",")
    for part in parts:
        if ":" in part:
            key, val = part.split(":", 1)
            try:
                # Try int first, then float
                data[key] = int(val) if "." not in val else float(val)
            except ValueError:
                data[key] = val  # fallback to string
    
    return {"type": "telemetry", "data": data} if data else None

async def serial_reader():
    loop = asyncio.get_running_loop()
    while True:
        try:
            line = await asyncio.to_thread(ser.readline)
            if line:
                text = line.decode(errors="replace").strip()
                
                # Try parsing structured telemetry
                parsed = parse_telemetry_line(text)
                if parsed:
                    import json
                    message = json.dumps(parsed)
                else:
                    # Send raw text for non-telemetry lines
                    message = text
                
                for ws in list(clients):
                    try:
                        await ws.send_str(message)
                    except Exception:
                        clients.discard(ws)
        except Exception:
            await asyncio.sleep(0.5)

async def start_background(app):
    app["sr_task"] = asyncio.create_task(serial_reader())

async def cleanup(app):
    task = app.get("sr_task")
    if task:
        task.cancel()
    try:
        ser.close()
    except Exception:
        pass

app = web.Application()
app.add_routes(routes)
app.on_startup.append(start_background)
app.on_cleanup.append(cleanup)

if __name__ == "__main__":
    web.run_app(app, port=WEB_PORT)
