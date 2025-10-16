#!/usr/bin/env python3
import os
import re
import sys


def compact_namespaces(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    original = content

    # Replace opening namespace declarations
    content = re.sub(
        r'namespace dart \{\s*\nnamespace collision \{\s*\nnamespace hit \{',
        'namespace dart::collision::hit {',
        content
    )

    # Replace closing namespace declarations
    content = re.sub(
        r'\} // namespace hit\s*\n\} // namespace collision\s*\n\} // namespace dart',
        '} // namespace dart::collision::hit',
        content
    )

    if content != original:
        with open(filepath, 'w') as f:
            f.write(content)
        return True
    return False

# Process all .h and .cpp files
os.chdir(sys.argv[1])
count = 0
for root, dirs, files in os.walk('.'):
    for file in files:
        if file.endswith('.h') or file.endswith('.cpp'):
            filepath = os.path.join(root, file)
            if compact_namespaces(filepath):
                count += 1

print(f"Compacted namespaces in {count} files")
