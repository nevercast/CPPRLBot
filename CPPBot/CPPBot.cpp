// CPPBot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "rlbot_dll.h"
#include <iostream>
#include "rlbot_dll.h"
#include "FlatbufferTranslator.hpp"

// extern "C" bool RLBOT_CORE_API IsInitialized()


int main()
{
	LoadRLBot();
	ByteBuffer buf = UpdateFieldInfoFlatbuffer();
	FieldInfo fieldInfoInst;
	FlatbufferTranslator::translateToFieldInfoStruct(buf, &fieldInfoInst);

	LiveDataPacket packet = LiveDataPacket();
	ByteBuffer buf2 = UpdateLiveDataPacketFlatbuffer();
	FlatbufferTranslator::translateToStruct(buf2, &packet);

	PlayerInput input = PlayerInput();
	input.Throttle = 1;
	// input.Handbrake = false;
	UpdatePlayerInput(input, 0);

	while (1) {

		// Leaks
		ByteBuffer buf2 = UpdateLiveDataPacketFlatbuffer();
		FlatbufferTranslator::translateToStruct(buf2, &packet);

		auto ballPosition = packet.GameBall.Physics.Location;
		for (int i = 0; i < packet.NumCars; i++) {
		
			auto car = packet.GameCars[i];
			auto carPosition = car.Physics.Location;

			float y = car.Physics.Rotation.Yaw;
			float f[] = {  cos(y), -sin(y) };
			float l[] = {  sin(y),  cos(y) };

			float dx[] = { ballPosition.X - carPosition.X, ballPosition.Y - carPosition.Y };

			float theta = atan2(l[0]*dx[0] + l[1]+dx[1], f[0]*dx[0] + f[1]*dx[1]);
			
			if (i == 0) {
				std::cout << theta << "\n";
			}

			input.Steer = fmaxf(fminf(3.0f * theta, 1.0f), -1.0f);

			UpdatePlayerInput(input, i);

		}
	}
}