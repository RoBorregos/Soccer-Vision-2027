#!/usr/bin/env bash
set -euo pipefail

# Formats C/C++ files in place with the same style as .github/workflows/clang-format.yml
cd "$(git rev-parse --show-toplevel)"

if ! command -v clang-format &> /dev/null; then
    echo "clang-format not found. Install it with: pip install clang-format==18.1.8"
    exit 1
fi

# Arguments are paths relative to the repo root; with none, check every tracked firmware C/C++ file
if [ "$#" -gt 0 ]; then
    files=("$@")
else
    # Tracked and untracked (not ignored) files, so new sources are included too
    mapfile -t files < <(git ls-files --cached --others --exclude-standard -- 'firmware/*.c' 'firmware/*.cc' 'firmware/*.cpp' 'firmware/*.h' 'firmware/*.hpp')
fi

STYLE="{BasedOnStyle: LLVM, IndentWidth: 4}"
changed=()
for file in "${files[@]}"; do
    # --dry-run --Werror exits non-zero only when the file would change
    if ! clang-format --style="$STYLE" --dry-run --Werror "$file" 2> /dev/null; then
        clang-format --style="$STYLE" -i "$file"
        changed+=("$file")
    fi
done

if [ "${#changed[@]}" -eq 0 ]; then
    echo "All ${#files[@]} file(s) already formatted."
else
    echo "Formatted ${#changed[@]} of ${#files[@]} file(s): ${changed[*]}"
fi
