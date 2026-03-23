#!/usr/bin/env python3
"""
Zip up all [game]/XXXX.pdx folders found in the current directory.
Each zip is named XXXX.pdx.zip and contains:
  - XXXX.pdx/ at the root level of the zip
  - LICENSE / license.txt (case-insensitive) from the [game] folder, if present
  - README.txt / README.md (case-insensitive) from the [game] folder, if present
"""

import os
import zipfile
import sys


def find_extra_files(folder: str) -> list:
    """Return paths to license and/or readme files in folder (case-insensitive)."""
    matches = []
    for entry in os.listdir(folder):
        if entry.lower() in ("license", "license.txt", "readme.txt", "readme.md"):
            matches.append(os.path.join(folder, entry))
    return matches


def zip_pdx(game_folder: str, pdx_folder: str, release_dir: str) -> str:
    """
    Zip pdx_folder (and optional extra files from game_folder) into XXXX.pdx.zip
    placed inside release_dir.  Returns the zip path.
    """
    pdx_name = os.path.basename(pdx_folder)          # e.g. MyGame.pdx
    zip_name = pdx_name + ".zip"                      # e.g. MyGame.pdx.zip
    zip_path = os.path.join(release_dir, zip_name)

    extra_files = find_extra_files(game_folder)

    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as zf:
        # Walk the .pdx folder and add every file, rooted at pdx_name/
        for root, _dirs, files in os.walk(pdx_folder):
            for file in files:
                abs_path = os.path.join(root, file)
                # arcname keeps XXXX.pdx/ at the zip root
                rel_path = os.path.relpath(abs_path, start=os.path.dirname(pdx_folder))
                zf.write(abs_path, arcname=rel_path)

        # Add license / readme files at zip root
        for extra in extra_files:
            zf.write(extra, arcname=os.path.basename(extra))

    return zip_path


def main():
    base_dir = os.getcwd()
    release_dir = os.path.join(base_dir, "Release")
    os.makedirs(release_dir, exist_ok=True)
    found = 0

    for entry in sorted(os.listdir(base_dir)):
        game_folder = os.path.join(base_dir, entry)
        if not os.path.isdir(game_folder) or entry == "Release":
            continue

        # Look for .pdx folders inside this game folder
        pdx_folders = [
            os.path.join(game_folder, sub)
            for sub in os.listdir(game_folder)
            if sub.endswith(".pdx") and os.path.isdir(os.path.join(game_folder, sub))
        ]

        if not pdx_folders:
            continue  # no .pdx folder here, skip silently

        for pdx_folder in pdx_folders:
            pdx_name = os.path.basename(pdx_folder)
            zip_path = zip_pdx(game_folder, pdx_folder, release_dir)

            extra_files = find_extra_files(game_folder)
            extras_note = "".join(f" + {os.path.basename(f)}" for f in sorted(extra_files))
            print(f"[{entry}]  {pdx_name}{extras_note}  →  {os.path.basename(zip_path)}")
            found += 1

    if found == 0:
        print("No .pdx folders found. Make sure you run this script from the")
        print("directory that contains your [game] folders.")
        sys.exit(1)
    else:
        print(f"\nDone — {found} zip(s) created.")


if __name__ == "__main__":
    main()
