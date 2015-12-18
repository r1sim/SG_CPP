#pragma once
#include <Packets/PacketBaseMessage.h>

struct MM_SC_MSN : public TS_MESSAGE
{
	//has some data
	static const uint16_t packetID = 5001;
};
struct MM_SC_MSN_FIND_USER : public TS_MESSAGE
{
	char username[42];
	static const uint16_t packetID = 5013;
};
struct MM_SC_FRIEND_REQUEST : public TS_MESSAGE
{
	char uk[40];
	char username[402];
	static const uint16_t packetID = 5015;
};
struct BM_SC_CHAT_MESSAGE : public TS_MESSAGE
{
	char sender[33];
	uint8_t type; // 0 = normal chat; 3 = whisper; 5 = shout
	uint16_t messagelength;
	char msg[70];
	static const uint16_t packetID = 2206;
};