#include "SG_MMOHandler.h"
#include "Tools/SG_Logger.h"
#include <Packets/MMO/MMOPacketsResponse.h>
void SG_MMOHandler::HandleLogin(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_LOGIN* packet)
{

	std::string skey(packet->sessionKey, packet->sessionKey + 33);
	Session->m_Player->SessionKey = skey;
	Session->m_Player->SessionKey.resize(32);
	
	//Get Accountsettings
	MySQLQuery accqry(Session->SQLConn, "Select id, ingamecash from Accounts where Sessionkey = ?;");
	accqry.setString(1, Session->m_Player->SessionKey);
	accqry.ExecuteQuery();
	if (accqry.GetResultRowCount()) // Some error occured. The Client will timeout after a few seconds.
	{
		Session->m_Player->playerid = accqry.getInt(1, "id");
		//Get Chars
		MySQLQuery qry(Session->SQLConn, "Select id, Name, Rank, CharType, Level, XP, License, Rupees, Coins, Questpoints from Chars where AccountID =  ?;");
		qry.setInt(1, Session->m_Player->playerid);
		qry.ExecuteQuery();
		if (qry.GetResultRowCount()) // Some error occured. The Client will timeout after a few seconds.
		{
			Session->m_Player->charname = qry.getString(1, "Name");
			Session->m_Player->chartype = qry.getInt(1, "CharType");
			Session->m_Player->charlevel = qry.getInt(1, "Level");
			Session->m_Player->gpotatos = qry.getInt(1, "Rupees");
			Session->m_Player->rupees = qry.getInt(1, "Rupees");
			Session->m_Player->coins = qry.getInt(1, "Coins");
			Session->m_Player->questpoints = qry.getInt(1, "Questpoints");
			Session->m_Player->charcreated = 1;
		}
		else
		{
			Session->m_Player->charcreated = 0;
		}
		//Send login successfull
		BM_SC_LOGIN_RESP response;
		BM_SC_LOGIN_RESP::initMessage<BM_SC_LOGIN_RESP>(&response);
		strcpy_s(response.resonse, static_cast<std::string>("SUCCESS").c_str());
		response.resonse[7] = static_cast<uint8_t>(0);
		Session->SendPacketStruct(&response);
	}else
	{
		//kick client
		BM_SC_LOGIN_RESP_FAILURE response;
		BM_SC_LOGIN_RESP_FAILURE::initMessage<BM_SC_LOGIN_RESP_FAILURE>(&response);
		strcpy_s(response.resonse, static_cast<std::string>("INVALID_REQUEST").c_str());
		response.resonse[15] = static_cast<uint8_t>(0);
		Session->SendPacketStruct(&response);
	}
}

void SG_MMOHandler::HandleCharCreation(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_CREATE_CHAR* packet)
{
	BM_SC_CREATE_CHAR_RESP response;
	BM_SC_CREATE_CHAR_RESP::initMessage<BM_SC_CREATE_CHAR_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);

	//Insert everything in Database
	MySQLQuery qry(Session->SQLConn, "INSERT INTO Chars (Name, Rank, CharType, Level, XP, License, Rupees, Coins, Questpoints, AccountID) VALUES (?,0,1,0,0,0,10,10,0,?);");
	qry.setString(1, packet->charname);
	qry.setInt(2, Session->m_Player->playerid);
	qry.ExecuteInsert();
	SG_Logger::instance().log(Session->m_Player->SessionKey + " has charname " + Session->m_Player->charname, SG_Logger::kLogLevelMMO);

	//std::cout << packet << std::endl;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendCharList(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_CHAR_LIST* packet)
{
	BM_SC_PLAYER_CHAR_LIST_RESP response;
	BM_SC_PLAYER_CHAR_LIST_RESP::initMessage<BM_SC_PLAYER_CHAR_LIST_RESP>(&response);
	strcpy_s(response.resonse, static_cast<std::string>("SUCCESS").c_str());
	response.resonse[7] = static_cast<uint8_t>(0);
	strcpy_s(response.charname, Session->m_Player->Username.c_str());
	for (auto i = Session->m_Player->Username.length(); i != 43; i++)
	{
		response.charname[i] = static_cast<uint8_t>(0);
	}
	response.firstlogin = Session->m_Player->charcreated; //0 will push the Player to the Charcreation
	response.chartype = Session->m_Player->chartype;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SelectChar(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_SELECT_CHAR_RESP response;
	BM_SC_SELECT_CHAR_RESP::initMessage<BM_SC_SELECT_CHAR_RESP>(&response);
	strcpy_s(response.resonse, static_cast<std::string>("SUCCESS").c_str());
	response.resonse[7] = static_cast<uint8_t>(0);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendTrickList(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_TRICK_LIST_RESP response;
	BM_SC_TRICK_LIST_RESP::initMessage<BM_SC_TRICK_LIST_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);

	response.trickcount = 13;
	std::copy(std::begin(Session->m_Player->tricks), std::end(Session->m_Player->tricks), std::begin(response.tricklist));

	BM_SC_TRICK_LIST_RESP::recalcheader<BM_SC_TRICK_LIST_RESP>(&response);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendPlayerInfo(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_PLAYER_INFO_RESP response;
	BM_SC_PLAYER_INFO_RESP::initMessage<BM_SC_PLAYER_INFO_RESP>(&response);

	//Set string values
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	strcpy_s(response.mapname1, static_cast<std::string>("ID1_Testo_2").c_str());
	strcpy_s(response.mapname2, static_cast<std::string>("ID1_Test").c_str());

	//Fill empty values with nothing
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.mapname1[11] = static_cast<uint8_t>(0);
	response.mapname2[8] = static_cast<uint8_t>(0);
	
	response.uk8 = 5;
	response.sp2id = 2;
	response.sp2size = 12;
	response.uk17 = 5;
	response.uk18 = 5;

	response.sp4id = 4;
	response.sp4size = 12;
	response.uk19 = 5;
	response.uk20 = 6;
	response.uk31 = 7;

	response.sp64id = 64;
	response.sp64size = 16;
	response.uk32 = 7;
	response.uk33 = 8;
	response.uk34 = 9;
	response.uk35 = 10;

	response.sp512id = 512;
	response.sp512size = 44;

	response.sp1024id = 511;
	response.sp1024size = 12;
	response.count1 = 2;
	response.count2 = 1;
	response.count3 = 2;



	//Playerstuff
	response.level = Session->m_Player->charlevel;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendBalanceInfo(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_BALANCE_INFO_RESP response;
	BM_SC_BALANCE_INFO_RESP::initMessage<BM_SC_BALANCE_INFO_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.rupees = Session->m_Player->rupees;
	response.coin = Session->m_Player->coins;
	response.gpotatos = Session->m_Player->gpotatos;
	response.questpoints = Session->m_Player->questpoints;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendInventory(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_INVENTORY_RESP response;
	BM_SC_INVENTORY_RESP::initMessage<BM_SC_INVENTORY_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	
	response.count = static_cast<uint16_t>(Session->m_Player->items.size());
	/*for (const auto &iter : Session->m_Player->items)
	{
		response.items.push_back(iter);
	}*/
	BM_SC_INVENTORY_RESP::recalcheader<BM_SC_INVENTORY_RESP>(&response);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendChannellist(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_CHANNEL_LIST_RESP response;
	BM_SC_CHANNEL_LIST_RESP::initMessage<BM_SC_CHANNEL_LIST_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	strcpy_s(response.channelname, static_cast<std::string>("CHANNEL 1").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	for (auto i = static_cast<std::string>("CHANNEL 1").length(); i != 12; i++)
	{
		response.channelname[i] = static_cast<uint8_t>(0);
	}
	response.channelcount = 1;
	response.channelid = 1;
	response.currentplayers = 1;
	response.maximumplayers = 1234;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::EnterChannel(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_ENTER_CHANNEL* packet)
{
	SG_Logger::instance().log(Session->m_Player->charname + " is joining Channel " + std::to_string(packet->channelid),SG_Logger::kLogLevelMMO);
	BM_SC_ENTER_CHANNEL_RESP response;
	BM_SC_ENTER_CHANNEL_RESP::initMessage<BM_SC_ENTER_CHANNEL_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.uk3 = 1;
	response.uk4 = 1;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::LeaveChannel(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_LEAVE_CHANNEL_RESP response;
	BM_SC_LEAVE_CHANNEL_RESP::initMessage<BM_SC_LEAVE_CHANNEL_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.uk3 = 1;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::HandlePositionUpdate(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_MMO_GAME_MESSAGE* packet)
{
	//TODO: send this to other players.
	Session->m_Player->coord_x = packet->coord_x;
	Session->m_Player->coord_y = packet->coord_y;
	Session->m_Player->coord_angle_z = packet->coord_angle_z;
	Session->m_Player->coord_z = packet->coord_z;
}

void SG_MMOHandler::HandleChatMessage(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_CHAT_MESSAGE* packet)
{
	//std::cout << packet << std::endl;
}

void SG_MMOHandler::EnterLobby(const boost::shared_ptr<SG_ClientSession> Session)
{
	ID_BZ_SC_ENTER_LOBBY_RESP response;
	ID_BZ_SC_ENTER_LOBBY_RESP::initMessage<ID_BZ_SC_ENTER_LOBBY_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.uk3 = 1;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SetSessionMessage(const boost::shared_ptr<SG_ClientSession> Session, const BM_SC_SET_SESSION_MESSAGE* packet)
{
	SG_Logger::instance().log(Session->m_Player->charname + "'s sessionmessage: " + packet->message, SG_Logger::kLogLevelMMO);
	BM_SC_SET_SESSION_MESSAGE_RESP response;
	BM_SC_SET_SESSION_MESSAGE_RESP::initMessage<BM_SC_SET_SESSION_MESSAGE_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.length = 21;
	strcpy_s(response.sessionmessage, static_cast<std::string>("Test_Session_Message").c_str());
	response.sessionmessage[20] = static_cast<uint8_t>(0);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::SendMissionList(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_MISSION_LIST_RESP response;
	BM_SC_MISSION_LIST_RESP::initMessage<BM_SC_MISSION_LIST_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.uk3 = 1;
	response.count = static_cast<uint16_t>(Session->m_Player->missions.size());
	for (const auto &iter : Session->m_Player->missions)
	{
		response.missions.push_back(iter);
	}
	BM_SC_MISSION_LIST_RESP::recalcheader<BM_SC_MISSION_LIST_RESP>(&response);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::StartMission(const boost::shared_ptr<SG_ClientSession> Session)
{
	BM_SC_START_MISSION_RESP response;
	BM_SC_START_MISSION_RESP::initMessage<BM_SC_START_MISSION_RESP>(&response);	
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	response.missionid = 55002;
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::EnterOX(const boost::shared_ptr<SG_ClientSession> Session)
{
	Session->m_Player->IsInOX = true;
	BM_SC_MMO_OX_ENTER_RESP response;
	BM_SC_MMO_OX_ENTER_RESP::initMessage<BM_SC_MMO_OX_ENTER_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	Session->SendPacketStruct(&response);
}

void SG_MMOHandler::LeaveOX(const boost::shared_ptr<SG_ClientSession> Session)
{
	Session->m_Player->IsInOX = false;
	BM_SC_MMO_OX_LEAVE_RESP response;
	BM_SC_MMO_OX_LEAVE_RESP::initMessage<BM_SC_MMO_OX_LEAVE_RESP>(&response);
	strcpy_s(response.successmessage, static_cast<std::string>("SUCCESS").c_str());
	response.successmessage[7] = static_cast<uint8_t>(0);
	Session->SendPacketStruct(&response);
}