#ifndef PACKETS_MMO_RESPONSE
#define PACKETS_MMO_RESPONSE

#include "Packets/PacketBaseMessage.h"

#pragma pack(push, 1)
#include <Constructors/Trick.h>
#include <Constructors/Item.h>

struct BM_SC_LOGIN_RESP : public TS_MESSAGE
{
	char resonse[8];
	static const uint16_t packetID = 2144;
};
struct BM_SC_LOGIN_RESP_FAILURE : public TS_MESSAGE
{
	char resonse[16];
	static const uint16_t packetID = 2144;
};
struct BM_SC_PLAYER_CHAR_LIST_RESP : public TS_MESSAGE
{
	char resonse[8]; // SUCCESS
	uint64_t uk1;
	uint64_t uk2;
	uint8_t uk3;
	char charname[43]; // Charname, you can use some html code like: <#ff0000> and <glow>
	uint16_t firstlogin;
	uint32_t uk4; 
	uint32_t chartype;
	uint32_t uk5;
	static const uint16_t packetID = 2318;
};
struct BM_SC_SELECT_CHAR_RESP : public TS_MESSAGE
{
	char resonse[8];
	static const uint16_t packetID = 2067;
};
struct BM_SC_PLAYER_INFO_RESP : public TS_MESSAGE
{
	uint32_t uk1; //needs to be 4
	uint16_t size1; // size of initpacket 1
	char successmessage[8];
	
	uint32_t uk2;
	uint32_t uk3;
	uint32_t uk4;
	uint32_t uk5;
	uint32_t uk6;
	uint16_t uk7;
	uint32_t uk8 = 5;

	//Subpacket 1
	uint16_t sp1id = 1;
	uint16_t sp1size = 50;
	char mapname1[12];
	uint8_t uk9;
	uint32_t uk10;
	uint32_t uk11;
	uint32_t uk12;
	uint32_t uk13;
	uint32_t uk14;
	uint32_t uk15;
	uint32_t uk16;
	char mapname2[9];

	//Subpacket 2
	uint16_t sp2id = 2;
	uint16_t sp2size = 12;
	uint16_t uk17 = 5;
	uint16_t uk18 = 5;
	uint64_t level;

	//Subpacket 4
	uint16_t sp4id = 4;
	uint16_t sp4size = 12;
	uint32_t uk19 = 5;
	uint32_t uk20 = 6;
	uint32_t uk31 = 7; 

	//Subpacket 64
	uint16_t sp64id = 64;
	uint16_t sp64size = 16;
	uint32_t uk32 = 7;
	uint32_t uk33 = 8;
	uint32_t uk34 = 9;
	uint32_t uk35 = 10;

	//Subpacket 512
	uint16_t sp512id = 512;
	uint16_t sp512size = 44;
	float speed;
	float accel;
	float turn;
	float brake;
	float boostspeed;
	float overspeed;
	float boosterc;
	float trick;
	float uk36;
	float uk37;

	//Subpacket 1024
	uint16_t sp1024id = 511;
	uint16_t sp1024size = 12;
	uint32_t count1 = 2;
	uint32_t count2 = 1;
	uint32_t count3 = 2;
	
	static const uint16_t packetID = 2314;
};
struct BM_SC_TRICK_LIST_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint8_t uk3;
	uint16_t trickcount; //13	ad
	Trickconstructor tricklist[13];

	static const uint16_t packetID = 2105;
};
struct BM_SC_BALANCE_INFO_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint32_t rupees;
	uint32_t coin;
	uint32_t gpotatos;
	uint32_t questpoints;
	static const uint16_t packetID = 2095;
};
struct BM_SC_CHANNEL_LIST_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint16_t channelcount;
	uint8_t channelid;
	char channelname[12];
	uint32_t uk3 = 5;
	uint32_t uk4 = 6;
	uint32_t uk5 = 7;
	uint32_t currentplayers = 15;
	uint32_t maximumplayers = 1337;
	static const uint16_t packetID = 2006;
};
struct BM_SC_ENTER_CHANNEL_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint8_t uk3;
	uint16_t uk4;
	static const uint16_t packetID = 2008;
};
struct BM_SC_LEAVE_CHANNEL_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint32_t uk3;
	static const uint16_t packetID = 2010;
};
struct ID_BZ_SC_ENTER_LOBBY_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint32_t uk3;
	static const uint16_t packetID = 19019;
};
struct BM_SC_SET_SESSION_MESSAGE_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint16_t length;
	char sessionmessage[21];
	static const uint16_t packetID = 2119;
};
struct BM_SC_MMO_OX_ENTER_RESP : public TS_MESSAGE
{
	char successmessage[8];
	static const uint16_t packetID = 2031;
};
struct BM_SC_MMO_OX_LEAVE_RESP : public TS_MESSAGE
{
	char successmessage[8];
	static const uint16_t packetID = 2033;
};
struct BM_SC_CREATE_CHAR_RESP : public TS_MESSAGE
{
	char successmessage[8];
	static const uint16_t packetID = 2069;
};
struct BM_SC_MISSION_LIST_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint16_t count;
	uint8_t uk3; //must be "1"
	std::vector<uint64_t> missions;
	static const uint16_t packetID = 2073;
};
struct BM_SC_START_MISSION_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t missionid;
	static const uint16_t packetID = 2075;
};
struct BM_SC_INVENTORY_RESP : public TS_MESSAGE
{
	char successmessage[8];
	uint64_t uk1;
	uint64_t uk2;
	uint8_t uk3;
	uint16_t count;
	std::vector<Item> items;
	static const uint16_t packetID = 2099;
};
#pragma pack(pop)

#endif // PACKETS_TS_CA_ACCOUNT_H