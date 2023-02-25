/**
 * @file run.cpp
 * @author Purdue Solar Racing (Aidan Orr)
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <cstring>

#include "run.h"
#include "can_lib.h"
#include STM32_INCLUDE(BOARD_STM32, hal_uart.h)


using namespace PSR;

extern "C"
{

void run(UART_HandleTypeDef* uart, CAN_HandleTypeDef* hcan)
{
	// Setup
	CANBus can = CANBus(*hcan, (CANBus::Config){});
	can.Init();

	// Infinite Loop
	constexpr int SYNC_RATE = 31;	// Number of frames between sync frame
	int syncCount = 0;
	while (true)
	{
		CANBus::Frame frame;
		while (can.Receive(frame))
		{
			union
			{
				struct
				{
					uint32_t Id;		// CAN Extended or Standard Id
					uint8_t IsExtended; // 1 or 0, whether the frame is extended
					uint8_t IsRTR;		// 1 or 0, whether the frame is a remote transmission
					uint8_t Length;		// Number of bytes in the data
					uint8_t Data[8];	// Payload of the CAN frame
				};
				uint8_t Bytes[16];
			} buffer;
			memset(buffer.Bytes, 0, sizeof(buffer));

			buffer.Id = frame.Id;
			buffer.IsExtended = frame.IsExtended ? 1 : 0;
			buffer.IsRTR = frame.IsRTR ? 1 : 0;
			buffer.Length = frame.Length;

			memcpy(buffer.Bytes, frame.Data.Bytes, frame.Length);

			HAL_UART_Transmit(uart, buffer.Bytes, sizeof(buffer), 100);
			syncCount++;

			if (syncCount == SYNC_RATE)
			{
				// Send a signal to sync frame bytes
				// Sends "SYNC" followed by 12 bytes of 0xFF to mark the byte sync area
				uint8_t data[16];
				memset(data, 0xFF, sizeof(data));
				memcpy(data, "SYNC", 4);
				HAL_UART_Transmit(uart, data, sizeof(data), 100);

				syncCount = 0;
			}
		}
	}
}

}
