#!/usr/bin/env python3
import os
import re

ccd_dir = "dart/collision/hit/ccd"

# Patterns to replace
patterns = [
    (r"\bccd_([a-zA-Z_])", r"dart_ccd_\1"),  # ccd_xxx -> dart_ccd_xxx
    (r"\bCCD_([A-Z_])", r"DART_CCD_\1"),  # CCD_XXX -> DART_CCD_XXX
    (r"_ccd_([a-z])", r"_dart_ccd_\1"),  # _ccd_xxx -> _dart_ccd_xxx
]

for filename in os.listdir(ccd_dir):
    filepath = os.path.join(ccd_dir, filename)
    if not os.path.isfile(filepath):
        continue
    if not (filename.endswith(".h") or filename.endswith(".c")):
        continue

    with open(filepath, "r") as f:
        content = f.read()

    original = content
    for pattern, replacement in patterns:
        content = re.sub(pattern, replacement, content)

    if content != original:
        with open(filepath, "w") as f:
            f.write(content)
        print(f"Updated {filename}")

print("Done renaming all ccd symbols to dart_ccd")
