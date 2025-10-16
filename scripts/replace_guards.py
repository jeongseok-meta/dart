#!/usr/bin/env python3
import os
import re
import sys


def replace_include_guards(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Pattern to match #ifndef GUARD_NAME \n #define GUARD_NAME
    pattern = r'#ifndef\s+(\w+)\s*\n\s*#define\s+\1\s*\n'

    if re.search(pattern, content):
        # Replace the #ifndef/#define with #pragma once
        content = re.sub(pattern, '#pragma once\n\n', content, count=1)

        # Remove the trailing #endif that closes the include guard
        lines = content.split('\n')
        for i in range(len(lines) - 1, -1, -1):
            stripped = lines[i].strip()
            if stripped.startswith('#endif'):
                lines[i] = ''
                break

        content = '\n'.join(lines)

        with open(filepath, 'w') as f:
            f.write(content)
        return True
    return False

# Process all .h files
os.chdir(sys.argv[1])
count = 0
for root, dirs, files in os.walk('.'):
    for file in files:
        if file.endswith('.h'):
            filepath = os.path.join(root, file)
            if replace_include_guards(filepath):
                count += 1

print(f"Replaced include guards in {count} files")
