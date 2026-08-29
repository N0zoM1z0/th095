#!/usr/bin/env bash

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
export JAVA_HOME="$repo_root/.tools/jdk"
export GHIDRA_HOME="$repo_root/.tools/ghidra"
export TH095_MSVC71_ROOT="$repo_root/.tools/msvc710"
export PATH="$GHIDRA_HOME/support:$repo_root/.tools/objdiff:$PATH"
unset repo_root
