# Terraria Internal Cheat
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows)
![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=flat&logo=visualstudio)

# Terraria Internal Cheat
Educational project exploring memory reading/writing  and function hooking in a .NET game process.  ## Technical details - Internal DLL injection - MinHook for function hooking - Pattern scanning for AOB signatures - Direct memory read/write for player state

## Features
- Teleport to cursor (F)
- HP modification

## How it works
1. Scans executable memory regions for AOB pattern targeting HP regen instruction
2. Installs naked hook via MinHook to capture player object pointer from EBX register
3. Uses captured pointer to read/write player state directly in memory

Offsets:
- 0x20 - X position (float)
- 0x24 - Y position (float)  
- 0x28 - X velocity (float)
- 0x2C - Y velocity (float)
- 0x424 - HP (int)

## Build
Visual Studio 2022, x86, Release
