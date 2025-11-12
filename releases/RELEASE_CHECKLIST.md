# Release checklist

Use this template when preparing a release:

- [ ] Update `VERSION` file
- [ ] Update `CHANGELOG.md` with release notes
- [ ] Tag the git commit: `git tag -a vX.Y.Z -m "Release vX.Y.Z"`
- [ ] Push tags: `git push --tags`
- [ ] Create GitHub release and attach build artifacts (if any)
- [ ] Update `releases/` with notes and any binary artifacts
- [ ] Verify CI passing on the release branch
