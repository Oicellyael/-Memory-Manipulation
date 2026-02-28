# -Memory-Manipulation


# Terraria Internal Cheat
Educational project exploring memory reading/writing  and function hooking in a .NET game process.  ## Technical details - Internal DLL injection - MinHook for function hooking - Pattern scanning for AOB signatures - Direct memory read/write for player state

## Features
- Teleport to cursor (F)
- HP modification

## How it works
Uses MinHook to hook HP regen function and capture 
player object. Reads/writes player coordinates directly 
from memory.

## Build
Visual Studio 2022, x86, Release
