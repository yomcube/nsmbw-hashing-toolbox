# NSMBW Hashing Toolbox

This project provides a simple interface to aid in cracking the NSMBW symbols from the djb2 hashes provided by the Shield TV version of the game.

## Current state

This project is still in development. Current status of the project:

- ✅ Demangler
- ✅ Inverse hash function
- ✅ XOR trick
- ❌ KMP attack
- 🚧 Z3 attack
- ✅ Dictionary attack

## Building

The build process has only been tested on Windows so far, but there shouldn't be many issues for building on Linux or MacOS.

First, download the z3 library from https://github.com/Z3Prover/z3/releases and place the z3 folder inside `/libs`.

Build the project with qmake / Qt Creator. Requires Qt >=4, <6