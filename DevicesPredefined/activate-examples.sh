#!/bin/sh

set -eu

catalog_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
project_dir=$(dirname -- "$catalog_dir")

if [ "$#" -eq 0 ]; then
    echo "Usage: sh DevicesPredefined/activate-examples.sh <package> [package ...]" >&2
    exit 2
fi

for package in "$@"; do
    package_dir="$catalog_dir/$package"
    if [ ! -d "$package_dir" ]; then
        echo "Unknown predefined package: $package" >&2
        exit 1
    fi

    for tree in include src; do
        source_tree="$package_dir/$tree"
        [ -d "$source_tree" ] || continue

        find "$source_tree" -type f -print | while IFS= read -r source_file; do
            relative_path=${source_file#"$package_dir/"}
            target_path=$(printf '%s\n' "$relative_path" | sed 's/\.example$//')
            target_file="$project_dir/$target_path"
            mkdir -p "$(dirname -- "$target_file")"
            cp "$source_file" "$target_file"
        done
    done
done

echo "Activated predefined package(s): $*"
