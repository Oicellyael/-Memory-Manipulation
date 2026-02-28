// dllmain.cpp : DLL entry point definition
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <limits>
#include "minhook/include/MinHook.h"
#include <vector>
#include <Psapi.h>
using namespace std;

HMODULE ghModule;
uintptr_t player = 0;      // stores player object pointer captured from hook
uintptr_t jmpback = 0;     // original function address to jump back after hook
uintptr_t adress = 0;      // address of found AOB pattern
uintptr_t superaddress = 0; // current address for memory scanning

MEMORY_BASIC_INFORMATION mbi;

// Naked hook - captures EBX (player object) and jumps back to original code
void __declspec(naked) myHook() {
    __asm {
        mov player, ebx  // save player pointer from EBX register
        jmp[jmpback]    // jump back to original instruction
    }
}

BOOL __stdcall Main() {
    // Open debug console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w+", stdout);
    cout << "Dll for terraria" << endl;
    cout << "DLL loaded!" << endl;

    uintptr_t Base = (uintptr_t)GetModuleHandleA("Terraria.exe");

    // AOB signature - targets HP regen instruction to capture player object
    unsigned char arr[] = { 0x0F, 0x8D, 0x16, 0x02, 0x00, 0x00, 0xFF, 0x83, 0x2C, 0x04, 0x00, 0x00 };

    MODULEINFO mInfo = { 0 };
    GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("Terraria.exe"), &mInfo, sizeof(mInfo));

    // Scan all committed executable memory regions for our AOB pattern
    while (VirtualQuery((LPCVOID)superaddress, &mbi, sizeof(mbi))) {
        if (mbi.State == MEM_COMMIT && mbi.RegionSize > 0) {
            if (mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)) {
                // Compare each byte in region against our pattern
                for (size_t i = 0; i < (mbi.RegionSize - sizeof(arr)); i++) {
                    if (memcmp((void*)((uintptr_t)mbi.BaseAddress + i), arr, sizeof(arr)) == 0) {
                        adress = (uintptr_t)mbi.BaseAddress + i;
                        break;
                    }
                }
            }
            if (adress) break;
        }
        superaddress += mbi.RegionSize;
    }

    cout << hex << (adress + 6) << endl;

    // Install hook at pattern+6 (the INC DWORD instruction)
    if (MH_Initialize() == MH_OK) {
        MH_CreateHook((LPVOID)(adress + 6), (LPVOID)myHook, (LPVOID*)&jmpback);
        MH_EnableHook(MH_ALL_HOOKS);
        cout << "Hooked!" << endl;
    }

    while (!GetAsyncKeyState(VK_DELETE)) {
        if (player != 0) {
            // F4 - Teleport to cursor position
            if (GetAsyncKeyState(VK_F4) & 1) {
                POINT p;
                GetCursorPos(&p); // get cursor screen coordinates

                // Read current player position
                float playerX = *(float*)(player + 0x20);
                float playerY = *(float*)(player + 0x24);

                // Calculate top-left corner of screen in world coordinates
                // Camera is centered on player, so subtract half screen size
                float screenPosX = playerX - 1280; // 2560 / 2
                float screenPosY = playerY - 720;  // 1440 / 2

                // Convert cursor screen coords to world coords
                float targetX = screenPosX + p.x;
                float targetY = screenPosY + p.y;

                // Write new position
                *(float*)(player + 0x20) = targetX;
                *(float*)(player + 0x24) = targetY;

                // Zero out velocity to prevent death on teleport
                *(float*)(player + 0x28) = 0;
                *(float*)(player + 0x2C) = 0;
                if (GetAsyncKeyState(VK_F5) & 1) {
                    // Restore HP/mana on teleport
                    *(int*)(player + 0x424) = 500;
                    *(int*)(player + 0x428) = 500;
                    *(int*)(player + 0x42C) = 500;
                }
            }
        }
        Sleep(10);
    }

    // Cleanup before unloading
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(ghModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        // Create main thread to avoid blocking DllMain
        HANDLE thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr);
        ghModule = hModule;
        if (thread != INVALID_HANDLE_VALUE) {
            CloseHandle(thread);
        }
    }
    return TRUE;
}