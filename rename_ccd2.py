#!/usr/bin/env python3
import os
import re

ccd_dir = "dart/collision/hit/ccd"

# Create a mapping of old to new names
# We'll do this carefully to avoid repeated replacements
replacements = []

# Collect all patterns - we'll apply them all at once
for filename in os.listdir(ccd_dir):
    filepath = os.path.join(ccd_dir, filename)
    if not os.path.isfile(filepath):
        continue
    if not (filename.endswith(".h") or filename.endswith(".c")):
        continue

    with open(filepath, "r") as f:
        content = f.read()

    # Apply all replacements at once using a single pass
    # Order matters - do more specific patterns first
    new_content = content

    # Replace header guards first
    new_content = re.sub(
        r"#ifndef\s+__CCD_H__\s*\n#define\s+__CCD_H__", "#pragma once", new_content
    )
    new_content = re.sub(
        r"#ifndef\s+__CCD_VEC3_H__\s*\n#define\s+__CCD_VEC3_H__",
        "#pragma once",
        new_content,
    )
    new_content = re.sub(
        r"#ifndef\s+__CCD_QUAT_H__\s*\n#define\s+__CCD_QUAT_H__",
        "#pragma once",
        new_content,
    )
    new_content = re.sub(
        r"#ifndef\s+__CCD_COMPILER_H__\s*\n#define\s+__CCD_COMPILER_H__",
        "#pragma once",
        new_content,
    )
    new_content = re.sub(
        r"#ifndef\s+__CCD_EXPORT_H__\s*\n#define\s+__CCD_EXPORT_H__",
        "#pragma once",
        new_content,
    )
    new_content = re.sub(r"\n#endif\s*/\*\s*__CCD_\w+__\s*\*/\s*$", "", new_content)
    new_content = re.sub(r"\n#endif\s*/\*\s*__CCD_\w+__\s*\*/", "", new_content)

    # Replace include paths
    new_content = re.sub(
        r"#include\s+<ccd/vec3\.h>",
        '#include "dart/collision/hit/ccd/vec3.h"',
        new_content,
    )
    new_content = re.sub(
        r"#include\s+<ccd/quat\.h>",
        '#include "dart/collision/hit/ccd/quat.h"',
        new_content,
    )
    new_content = re.sub(
        r"#include\s+<ccd/ccd\.h>",
        '#include "dart/collision/hit/ccd/ccd.h"',
        new_content,
    )
    new_content = re.sub(
        r"#include\s+<ccd/compiler\.h>",
        '#include "dart/collision/hit/ccd/compiler.h"',
        new_content,
    )
    new_content = re.sub(
        r"#include\s+<ccd/ccd_export\.h>",
        '#include "dart/collision/hit/ccd/ccd_export.h"',
        new_content,
    )

    # Replace macros and constants - use word boundaries
    new_content = re.sub(r"\bCCD_EXPORT\b", "DART_CCD_EXPORT", new_content)
    new_content = re.sub(r"\bCCD_REAL\b", "DART_CCD_REAL", new_content)
    new_content = re.sub(r"\bCCD_SQRT\b", "DART_CCD_SQRT", new_content)
    new_content = re.sub(r"\bCCD_FABS\b", "DART_CCD_FABS", new_content)
    new_content = re.sub(r"\bCCD_FMIN\b", "DART_CCD_FMIN", new_content)
    new_content = re.sub(r"\bCCD_FMAX\b", "DART_CCD_FMAX", new_content)
    new_content = re.sub(r"\bCCD_INIT\b", "DART_CCD_INIT", new_content)

    # Replace function names - only those starting with ccd
    new_content = re.sub(r"\bccdVec3", "dartCcdVec3", new_content)
    new_content = re.sub(r"\bccdQuat", "dartCcdQuat", new_content)
    new_content = re.sub(r"\bccdGJK", "dartCcdGJK", new_content)
    new_content = re.sub(r"\bccdMPR", "dartCcdMPR", new_content)
    new_content = re.sub(r"\bccdFirst", "dartCcdFirst", new_content)

    # Replace type names with struct/typedef
    new_content = re.sub(r"\bccd_real_t\b", "dart_ccd_real_t", new_content)
    new_content = re.sub(r"\bccd_vec3_t\b", "dart_ccd_vec3_t", new_content)
    new_content = re.sub(r"\bccd_quat_t\b", "dart_ccd_quat_t", new_content)
    new_content = re.sub(r"\bccd_t\b", "dart_ccd_t", new_content)
    new_content = re.sub(r"\bccd_support_fn\b", "dart_ccd_support_fn", new_content)
    new_content = re.sub(r"\bccd_center_fn\b", "dart_ccd_center_fn", new_content)
    new_content = re.sub(r"\bccd_first_dir_fn\b", "dart_ccd_first_dir_fn", new_content)

    # Replace struct names
    new_content = re.sub(r"\b_ccd_vec3_t\b", "_dart_ccd_vec3_t", new_content)
    new_content = re.sub(r"\b_ccd_quat_t\b", "_dart_ccd_quat_t", new_content)
    new_content = re.sub(r"\b_ccd_t\b", "_dart_ccd_t", new_content)

    # Replace inline specifier
    new_content = re.sub(r"\b_ccd_inline\b", "_dart_ccd_inline", new_content)

    if new_content != content:
        with open(filepath, "w") as f:
            f.write(new_content)
        print(f"Updated {filename}")
    else:
        print(f"No changes needed for {filename}")

print("\nDone renaming all ccd symbols to dart_ccd")
