#pragma once
#include <iostream>
#include <Windows.h>
#include "ByteBuffer.h"
#include "PacketStructs/PacketStructs.hpp"

typedef bool(__cdecl *IS_INITIALIZED)();
typedef ByteBuffer(__cdecl *UPDATE_FIELD_INFO_FLATBUFFER)();
typedef void(__cdecl *UPDATE_PLAYER_INPUT)(PlayerInput playerInput, int playerIndex);
typedef ByteBuffer(__cdecl *UPDATE_LIVE_DATA_PACKET_FLATBUFFER)();

IS_INITIALIZED IsInitialized = nullptr;
UPDATE_FIELD_INFO_FLATBUFFER UpdateFieldInfoFlatbuffer = nullptr;
UPDATE_PLAYER_INPUT UpdatePlayerInput = nullptr;
UPDATE_LIVE_DATA_PACKET_FLATBUFFER UpdateLiveDataPacketFlatbuffer = nullptr;


void LoadRLBot() {
	auto rlbot_dll = LoadLibrary(TEXT("..\\RLBot_Core_Interface_32.dll")); 
	if (!rlbot_dll) {
		std::cout << "Failed to load RLBot DLL.\n";
		std::exit(-1);
	} else {
		std::cout << "RLBot DLL: " << rlbot_dll;

		IsInitialized = (IS_INITIALIZED)GetProcAddress(rlbot_dll, "IsInitialized");
		if (IsInitialized) {
			auto isInitializeTho = IsInitialized();
			std::cout << isInitializeTho;
		}
		else {
			std::cout << "Failed to get IsInitialized, is it even exported?\n";
			std::exit(-2);
		}

		UpdateFieldInfoFlatbuffer = (UPDATE_FIELD_INFO_FLATBUFFER)GetProcAddress(rlbot_dll, "UpdateFieldInfoFlatbuffer");
		if (!UpdateFieldInfoFlatbuffer) {
			std::cout << "Failed to get UpdateFieldInfoFlatbuffer, is it even exported?\n";
			std::exit(-3);
		}

		UpdatePlayerInput = (UPDATE_PLAYER_INPUT)GetProcAddress(rlbot_dll, "UpdatePlayerInput");
		if (!UpdatePlayerInput) {
			std::cout << "Failed to get UpdatePlayerInput, is it even exported?\n";
			std::exit(-4);
		}

		UpdateLiveDataPacketFlatbuffer = (UPDATE_LIVE_DATA_PACKET_FLATBUFFER)GetProcAddress(rlbot_dll, "UpdateLiveDataPacketFlatbuffer");
		if (!UpdateLiveDataPacketFlatbuffer) {
			std::cout << "Failed to get UpdateLiveDataPacketFlatbuffer, is it even exported?\n";
			std::exit(-5);
		}
		
	}
}
