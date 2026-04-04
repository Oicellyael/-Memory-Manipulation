# \# InsightX-x32: Low-Level Instrumentation & Memory Orchestration
### Project Overview

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus)
![.NET](https://img.shields.io/badge/.NET-512BD4?style=flat&logo=dotnet&logoColor=white)
![RE](https://img.shields.io/badge/RE-Reverse_Engineering-blueviolet?style=flat)

**InsightX-x32** is a high-performance in-process instrumentation library designed for 32-bit (x86) legacy applications. The project demonstrates advanced techniques in **context interception**, **manual stack management**, and **dynamic memory protection handling**.

Developed as a research tool to explore software reverse engineering, real-time debugging, and the mechanics of register-level data acquisition.

## 🛠 Technical Highlights

  * **Architecture:** Optimized for x86 (32-bit) execution environments.
  * **Context Interception:** Implements **Naked Functions (`__declspec(naked)`)** and Inline Assembly to capture CPU register states without corrupting the calling convention or stack frames.
  * **Signature-Based Scanning:** Features an AOB (Array of Bytes) scanning engine utilizing `VirtualQuery` for dynamic verification of memory region access rights (MEM\_COMMIT, PAGE\_EXECUTE\_READWRITE).
  * **Hook Management:** Integrated with **MinHook** for stable control-flow redirection and automated cleanup.

## 🚀 Key Features

  * **Runtime Context Acquisition:** Intercepts the CPU execution flow to capture specific register states (e.g., `EBX`), allowing for the retrieval of live pointers to internal application data structures.
  * **Spatial State Transformation:** Implements coordinate translation logic (World-to-Screen) and real-time vector manipulation for entity state modulation.
  * **Asynchronous State Management:** Direct modification of internal application attributes (Health, Physics, Velocity) via cross-referenced memory offsets.
  * **Memory Safety Layer:** Verifies memory protection flags before performing I/O operations, ensuring system stability during instrumentation.

## 💻 Implementation Details

### Low-Level Register Hijacking

The library uses "naked" functions to manually manage the stack and preserve the CPU state while extracting critical runtime pointers.

```cpp
// Capturing internal object pointers via direct register access
void __declspec(naked) hkContextCapture() {
    __asm {
        mov pInternalObject, ebx // Store state from EBX for external analysis
        jmp [dwOriginalFlow]     // Resume host application execution
    }
}
```

### Dynamic Instruction Locating

Implements an efficient memory scanner that traverses the virtual address space to identify specific logic patterns.

```cpp
// Verifying memory protection before scanning
if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_EXECUTE_READWRITE) {
    // Perform pattern-matching across the identified executable region
}
```

## 📁 Internal Data Mapping (Analysis Build)

| Attribute | Offset | Data Type |
|-----------|--------|-----------|
| Vector\_X  | 0x20   | float     |
| Vector\_Y  | 0x24   | float     |
| Velocity\_X| 0x28   | float     |
| Health    | 0x424  | int       |

## ⚠️ Research Disclaimer

This project is for **educational and research purposes only**. It explores the boundaries of software instrumentation, memory safety, and low-level debugging in legacy x86 environments.
