#!/usr/bin/env python3
import os
import re

ccd_dir = "dart/collision/hit/ccd"

for filename in os.listdir(ccd_dir):
    filepath = os.path.join(ccd_dir, filename)
    if not os.path.isfile(filepath):
        continue
    if not (filename.endswith(".h") or filename.endswith(".c")):
        continue

    with open(filepath, "r") as f:
        content = f.read()

    original = content

    # Remove DART_CCD_EXPORT from function declarations and definitions
    content = re.sub(r"\bDART_CCD_EXPORT\s+", "", content)

    if content != original:
        with open(filepath, "w") as f:
            f.write(content)
        print(f"Removed DART_CCD_EXPORT from {filename}")
    else:
        print(f"No DART_CCD_EXPORT found in {filename}")

print("\nDone removing DART_CCD_EXPORT")
