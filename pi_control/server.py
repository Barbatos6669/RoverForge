#!/usr/bin/env python3
"""Simple aiohttp + pyserial server to forward websocket commands to Arduino."""
import asyncio
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

async def serial_reader():
    loop = asyncio.get_running_loop()
    while True:
        try:
            line = await asyncio.to_thread(ser.readline)
            if line:
                text = line.decode(errors="replace").strip()
                for ws in list(clients):
                    try:
                        await ws.send_str(text)
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
