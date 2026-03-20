#!/usr/bin/env python3
# update_pdxinfo.py
# Updates the version and buildNumber fields in all */Source/pdxinfo files
# found in subdirectories of the current working directory.
#
# Usage:
#   python update_pdxinfo.py <version> <buildNumber>
#
# Examples:
#   python update_pdxinfo.py v1.4.0 6
#   python update_pdxinfo.py v2.0.0 10

import sys
import os
import re

def update_pdxinfo(path, new_version, new_build):
    with open(path, "r", encoding="utf-8") as f:
        content = f.read()

    updated = re.sub(r"(?m)^version=.*$", f"version={new_version}", content)
    updated = re.sub(r"(?m)^buildNumber=.*$", f"buildNumber={new_build}", updated)

    if updated == content:
        return False

    with open(path, "w", encoding="utf-8") as f:
        f.write(updated)

    return True

def main():
    if len(sys.argv) != 3:
        print("Usage: python update_pdxinfo.py <version> <buildNumber>")
        print("  version      e.g. v1.4.0")
        print("  buildNumber  e.g. 6")
        sys.exit(1)

    new_version = sys.argv[1]
    new_build   = sys.argv[2]

    cwd = os.getcwd()
    found = 0
    updated = 0

    for entry in sorted(os.scandir(cwd), key=lambda e: e.name):
        if not entry.is_dir():
            continue
        pdxinfo = os.path.join(entry.path, "Source", "pdxinfo")
        if not os.path.isfile(pdxinfo):
            continue

        found += 1
        rel = os.path.relpath(pdxinfo, cwd)

        if update_pdxinfo(pdxinfo, new_version, new_build):
            updated += 1
            print(f"  Updated  : {rel}")
        else:
            print(f"  No change: {rel}")

    print(f"\nDone. Found {found} pdxinfo file(s), updated {updated}.")

if __name__ == "__main__":
    main()
