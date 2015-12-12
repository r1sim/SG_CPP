#include "SG_MmoServer.h"
#include <string>
#include "Constructors/SG_Packets.h"
#include "Tools/SG_Logger.h"
#include "Handlers/MMO/SG_MMOHandler.h"
#include <Packets/MMO/MMOPackets.h>


bool SG_MmoServer::OnClientConnected(const boost::shared_ptr<SG_ClientSession> pSession)
{
	SG_Logger::instance().log("[" + pSession->m_Player->SessionKey + "] connected from: " + pSession->getSocket().remote_endpoint().address().to_string(), SG_Logger::kLogLevelMMO);
	return true;
}

void SG_MmoServer::OnClientDisconnect(const boost::shared_ptr<SG_ClientSession> pSession)
{
	SG_Logger::instance().log("[" + pSession->m_Player->SessionKey + "] disconnected!", SG_Logger::kLogLevelMMO);
}

bool SG_MmoServer::OnPacketReceived(const boost::shared_ptr<SG_ClientSession> pSession, const TS_MESSAGE* packet)
{
	switch (packet->id)
	{
	case SG_Packets::Recv::XX_SC_KEEP_ALIVE_recv:
		pSession->m_Player->UpdateLastKeepAlive();
		return true;
	case BM_SC_LOGIN::packetID:
		SG_MMOHandler::HandleLogin(pSession, static_cast<const BM_SC_LOGIN*>(packet));
		SG_MMOHandler::SendCharList(pSession, static_cast<const BM_SC_CHAR_LIST*>(packet));
		break;
	case BM_SC_CHAR_LIST::packetID:
		if(pSession->m_Player->charcreated == 0)
		{
			SG_MMOHandler::SendCharList(pSession, static_cast<const BM_SC_CHAR_LIST*>(packet));
			SG_MMOHandler::SendPlayerInfo(pSession);
			SG_MMOHandler::SendTrickList(pSession);
		}
		else
		{
			SG_MMOHandler::SendCharList(pSession, static_cast<const BM_SC_CHAR_LIST*>(packet));
		}
		break;
	case BM_SC_SELECT_CHAR::packetID:
		SG_MMOHandler::SelectChar(pSession);
		//Sometimes the client wont request this data, so we send it after the char selection without a request.
		SG_MMOHandler::SendPlayerInfo(pSession);
		SG_MMOHandler::SendTrickList(pSession);
		break;
	case BM_SC_PLAYER_INFO::packetID:
		SG_MMOHandler::SendPlayerInfo(pSession);
		break;
	case BM_SC_TRICK_LIST::packetID:
		SG_MMOHandler::SendTrickList(pSession);
		break;
	case BM_SC_CHANNEL_LIST::packetID:
		SG_MMOHandler::SendChannellist(pSession);
		break;
	case BM_SC_BALANCE_INFO::packetID:
		SG_MMOHandler::SendBalanceInfo(pSession);
		break;
	case BM_SC_ENTER_CHANNEL::packetID:
		SG_MMOHandler::EnterChannel(pSession, static_cast<const BM_SC_ENTER_CHANNEL*>(packet));
		break;
	case BM_SC_LEAVE_CHANNEL::packetID:
		SG_MMOHandler::LeaveChannel(pSession);
		break;
	case ID_BZ_SC_ENTER_LOBBY::packetID:
		SG_MMOHandler::EnterLobby(pSession);
		break;
	case BM_SC_SET_SESSION_MESSAGE::packetID:
		SG_MMOHandler::SetSessionMessage(pSession, static_cast<const BM_SC_SET_SESSION_MESSAGE*>(packet));
		break;
	case BM_SC_MMO_GAME_MESSAGE::packetID:
		SG_MMOHandler::HandlePositionUpdate(pSession, static_cast<const BM_SC_MMO_GAME_MESSAGE*>(packet));
		break;
	case BM_SC_MMO_TICK_MESSAGE::packetID:
		break;
	case BM_SC_STATUS_MESSAGE::packetID:
		break;
	case BM_SC_CHAT_MESSAGE::packetID:
		SG_MMOHandler::HandleChatMessage(pSession, static_cast<const BM_SC_CHAT_MESSAGE*>(packet));
		break;
	case BM_SC_INVENTORY::packetID:
		SG_MMOHandler::SendInventory(pSession);
		break;
	case BM_SC_MISSION_LIST::packetID:
		SG_MMOHandler::SendMissionList(pSession);
		break;
	case BM_SC_START_MISSION::packetID:
		SG_MMOHandler::StartMission(pSession);
		break;
	case BM_SC_MMO_OX_ENTER::packetID:
		SG_MMOHandler::EnterOX(pSession);
		break;
	case BM_SC_MMO_OX_LEAVE::packetID:
		SG_MMOHandler::LeaveOX(pSession);
		break;
	case BM_SC_CREATE_CHAR::packetID:
		SG_MMOHandler::HandleCharCreation(pSession, static_cast<const BM_SC_CREATE_CHAR*>(packet));
		break;
	default:
		SG_Logger::instance().log("Unknown Packet ID[" + std::to_string(packet->id) + "] Size[" + std::to_string(packet->size) + "]",SG_Logger::kLogLevelPacket);
	}

	return true;
}