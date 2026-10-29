#!/usr/bin/env bash
set -euo pipefail
th095_repo_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$th095_repo_root/build/gogai/play"
for th095_file in th095-gogai.exe th095.dat thbgm.dat; do
    if [[ ! -f "$th095_file" ]]; then
        echo "Missing $th095_file. Build with scripts/build-gogai.py --data-dir /path/to/original-game first." >&2
        exit 1
    fi
done
exec wine th095-gogai.exe "$@"
