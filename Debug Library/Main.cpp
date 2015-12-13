#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <fstream>

#include <detours.h>
#pragma comment(lib, "detours.lib")

std::ofstream ofstrFile;

int32_t __cdecl hEncryptMessage(uint32_t hashtable_ptr, uint32_t from_Buffer, uint32_t to_Buffer, uint32_t Size)
{
	uint32_t hashtable_ptr_old = hashtable_ptr;
	uint32_t Hashtable_1 = *reinterpret_cast<uint32_t*>(hashtable_ptr);
	uint32_t Hashtable_2 = *reinterpret_cast<uint32_t*>(hashtable_ptr + 4);

	for (auto i = 0; i != Size; ++i)
	{
		Hashtable_1 = static_cast<uint8_t>(Hashtable_1 + 1);
		char tmpKey_1 = *reinterpret_cast<BYTE *>(hashtable_ptr + Hashtable_1 + 8);

		Hashtable_2 = static_cast<uint8_t>(tmpKey_1 + Hashtable_2);
		char tmpKey_2 = *reinterpret_cast<uint8_t *>(hashtable_ptr + Hashtable_2 + 8);

		*reinterpret_cast<uint8_t *>(hashtable_ptr + Hashtable_1 + 8) = tmpKey_2;
		*reinterpret_cast<uint8_t *>(hashtable_ptr + Hashtable_2 + 8) = tmpKey_1;

		*reinterpret_cast<uint8_t *>(to_Buffer + i) = *reinterpret_cast<uint8_t *>(from_Buffer + i);
	}

	*reinterpret_cast<DWORD *>(hashtable_ptr) = Hashtable_1;
	*reinterpret_cast<DWORD *>(hashtable_ptr + 4) = Hashtable_2;

	return hashtable_ptr_old;
}

void WINAPI hOutputDebugStringA(LPCTSTR lpOutputString)
{
	std::cout << lpOutputString;
	ofstrFile << lpOutputString;
	ofstrFile.flush();
}

void Patch()
{
	DetourFunction(reinterpret_cast<uint8_t*>(0x007B25A0), reinterpret_cast<uint8_t*>(hEncryptMessage)); // remove packet encryption [SEND]
	//DetourFunction((uint8_t*)0x00440D2F, (uint8_t*)0x00440D43); // remove password encryption
	DetourFunction(reinterpret_cast<uint8_t*>(0x00443500), reinterpret_cast<uint8_t*>(0x00443528)); // remove game guard init
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		Patch();

	return TRUE;
}