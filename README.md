# Terraria Internal Cheat
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows)
![Visual Studio](https://img.shields.io/badge/Visual_Studio-5C2D91?style=flat&logo=visualstudio)

Terraria Internal Memory Manipulation Tool
A lightweight internal C++ library for Terraria (x86), demonstrating low-level software instrumentation, register hijacking, and memory pattern scanning.

🛠 Technical Overview
Architecture: x86 (32-bit).

Hooking Method: Inline Assembly (__declspec(naked)) for direct register manipulation.

Signature Scanning: AOB (Array of Bytes) scanning using VirtualQuery for dynamic instruction locating.

Library: MinHook (used for hook management and cleanup).

🚀 Key Features
Player Object Hijacking: Intercepts the CPU execution flow to capture the ebx register, retrieving the live pointer to the local player object.

Teleport to Cursor: Sophisticated world-to-screen coordinate translation allowing the player to reposition instantly to the cursor's world position.

State Management: Direct modification of player attributes (HP, Mana) and physics (velocity zeroing to prevent fall damage during teleportation).

Dynamic Memory Protection Handling: Scans memory regions and automatically verifies access rights before performing I/O operations.

💻 Code Highlights
Naked Hook Implementation
The tool uses a naked function to capture the player object address without corrupting the stack or other registers:

C++
void __declspec(naked) myHook() {
    __asm {
        mov player, ebx  // Captures the player object pointer from EBX register
        jmp [jmpback]    // Jumps back to the original instruction flow
    }
}
Signature Scanning Logic
Implementation of an efficient scanner that traverses the process memory space to find specific instruction patterns:

C++
if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_EXECUTE_READWRITE) {
    // Logic for pattern matching across the memory region
}

Offsets:
- 0x20 - X position (float)
- 0x24 - Y position (float)  
- 0x28 - X velocity (float)
- 0x2C - Y velocity (float)
- 0x424 - HP (int)

## Build
Visual Studio 2022, x86, Release

⚠️ Disclaimer
This project is for educational purposes only. It demonstrates techniques used in software reverse engineering, memory safety, and debugging.
