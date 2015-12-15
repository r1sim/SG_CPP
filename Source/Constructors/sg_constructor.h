#pragma once
#include <cinttypes>
#include <string>

namespace sg_constructor
{
	struct Item
	{
		Item() {}
		Item(uint32_t Unique, uint32_t ID, uint32_t trade, uint32_t Dura, uint16_t uk2, uint8_t equip, uint32_t uk4)
			: UniqueID(Unique), ItemID(ID), Trade_Status(trade), Duration(Dura), uk_2(uk2), Equip_Status(equip), uk_4(uk4) { }
		//	Item ID; Itemtype ID; Handelbar; Verbleibende Zeit; Angelegt; ?;

		uint32_t UniqueID;
		uint32_t ItemID;
		uint32_t Trade_Status; // 0 = No | 1 = Yes
		uint16_t uk1;
		uint32_t Duration;
		uint32_t uk2;
		uint16_t uk_2;
		uint8_t Equip_Status; // 0 = No | 1 = Yes
		uint32_t uk_4;
	};
	struct Trickconstructor
	{
		Trickconstructor() { }
		Trickconstructor(uint32_t ID, uint32_t level, uint8_t apply) : TrickID(ID), Tricklvl(level), ApplyTrick(apply) { }

		uint32_t TrickID;
		uint32_t Tricklvl;
		uint8_t ApplyTrick;
	};
	struct Room
	{
		Room(){}
		Room(std::string name, std::string pw, uint32_t mode, uint8_t max_player, uint8_t level)
			: Name(name), Password(pw), Mode(mode), Max_Player(max_player), Level(level) { }

		uint32_t RoomID = 0;
		std::string Name;
		std::string Password;

		// 0/1 = free item-mode	| 2/3 = password item-mode
		// 4/5 = free speed-mode	| 6/7 = password speed-mode
		uint32_t Mode;
		uint8_t Actual_Player = 0;
		uint8_t Max_Player;
		uint8_t State = 0; // enter state -> 0 = can enter | 1 = no enter
		uint8_t Level; // level -> 0 = all | 1 = anfaenger | 2 = profi | 3 = profi II | 4 = meister | 5 = superstar
	};
}