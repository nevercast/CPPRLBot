#pragma once
#include <iostream>
#include <Windows.h>
#include "ByteBuffer.h"
#include "PacketStructs/PacketStructs.hpp"

// Could this be done better with templates? I don't know how to use templates, I don't even know what they are.
#define RLBOT_API(RETURN_TYPE, FUNC_NAME, ...) typedef RETURN_TYPE( __cdecl *RLPROC_##FUNC_NAME) (__VA_ARGS__); RLPROC_##FUNC_NAME FUNC_NAME = nullptr
#define RLBOT_API_LOAD(RLBOT_DLL, FUNC_NAME) \
	FUNC_NAME = (RLPROC_##FUNC_NAME)GetProcAddress(RLBOT_DLL, #FUNC_NAME); \
	if (!FUNC_NAME) { \
		std::cout << "Failed to get " << #FUNC_NAME << ", is it even exported?\n"; \
		std::exit(-2); \
	}

// I don't know why there is a warning here, I'm ignoring it.
RLBOT_API(bool, IsInitialized);
RLBOT_API(ByteBuffer, UpdateFieldInfoFlatbuffer);
RLBOT_API(void, UpdatePlayerInput, PlayerInput playerInput, int playerIndex);
RLBOT_API(ByteBuffer, UpdateLiveDataPacketFlatbuffer);
RLBOT_API(void, Free, void *ptr);


void LoadRLBot() {
	auto rlbot_dll = LoadLibrary(TEXT("..\\RLBot_Core_Interface_32.dll")); 
	if (!rlbot_dll) {
		std::cout << "Failed to load RLBot DLL.\n";
		std::exit(-1);
	} else {
		std::cout << "RLBot DLL: " << rlbot_dll;
		
		RLBOT_API_LOAD(rlbot_dll, IsInitialized);
		RLBOT_API_LOAD(rlbot_dll, UpdateFieldInfoFlatbuffer);
		RLBOT_API_LOAD(rlbot_dll, UpdatePlayerInput); // Use Flatbuffer instead when we understand how to create them and use the Builder.
		RLBOT_API_LOAD(rlbot_dll, UpdateLiveDataPacketFlatbuffer);
		RLBOT_API_LOAD(rlbot_dll, Free);

		// TODO Wait for IsInitialized to be true, and consider timing out.
	}
}

void FreeByteBuffer(ByteBuffer buf) {
	Free(buf.ptr);
}
