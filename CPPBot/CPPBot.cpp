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

	FreeByteBuffer(buf);

	LiveDataPacket packet = LiveDataPacket();
	ByteBuffer buf2 = UpdateLiveDataPacketFlatbuffer();
	FlatbufferTranslator::translateToStruct(buf2, &packet);

	FreeByteBuffer(buf2);

	PlayerInput input = PlayerInput();
	input.Throttle = 1;
	// input.Handbrake = false;
	SetPlayerControls(input, 0);


	while (1) {

		// Leaks
		ByteBuffer buf2 = UpdateLiveDataPacketFlatbuffer();
		FlatbufferTranslator::translateToStruct(buf2, &packet);
		FreeByteBuffer(buf2);

		auto ballPosition = packet.GameBall.Physics.Location;
		for (int i = 0; i < packet.NumCars; i++) {
		
			auto car = packet.GameCars[i];
			auto carPosition = car.Physics.Location;

			// Add approximately PI, then the values don't wrap around :)
			float y = 3.2f + car.Physics.Rotation.Yaw;

			float carBallOffset[] = { ballPosition.X - carPosition.X, ballPosition.Y - carPosition.Y };
			float deltaAngle = 3.2f + atan2(carBallOffset[1], carBallOffset[0]);
			
			if (i == 0) {
				std::cout << "Delta Angle:" << deltaAngle << ", Yaw: " << y << "\n";
			}
	
			if (deltaAngle < y) {
				input.Steer = -0.5f;
			}
			else {
				input.Steer = 0.5f;
			}

			SetPlayerControls(input, i);
		}
		DrawText2D();
	}
}