#!/usr/bin/env python3
"""Fails when any authored file reaches 300 whitespace-separated words."""
import os
import sys

LIMIT = 300
SKIP_DIRS = {".git", "build", "__pycache__", "node_modules"}
EXTS = {".c", ".h", ".S", ".md", ".bat", ".py", ".vert", ".frag", ".xml",
        ".json", ".txt"}


def main():
    bad = []
    total = 0
    for root, dirs, files in os.walk("."):
        dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
        for name in files:
            ext = os.path.splitext(name)[1]
            if ext not in EXTS:
                continue
            path = os.path.join(root, name)
            try:
                text = open(path, encoding="utf-8", errors="ignore").read()
            except OSError:
                continue
            count = len(text.split())
            total += 1
            if count >= LIMIT:
                bad.append((count, path))
    for count, path in sorted(bad, reverse=True):
        print("%6d  %s" % (count, path))
    if bad:
        print("FAIL: %d of %d files are >= %d words" % (len(bad), total, LIMIT))
        return 1
    print("OK: %d files, all under %d words" % (total, LIMIT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
