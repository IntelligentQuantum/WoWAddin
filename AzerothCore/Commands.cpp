#include "stdafx.h"

UINT64 GetTargetGUID()
{
    UINT64 guid = -1;

    if (CGObject_C* player = ObjectMgr::GetObjectPtr(ObjectMgr::GetActivePlayerGuid(), TYPEMASK_PLAYER))
        if (CGObject_C* target = ObjectMgr::GetObjectPtr(player->GetValue<UINT64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT))
            guid = target->GetValue<UINT64>(OBJECT_FIELD_GUID);

    return guid;
}

BOOL GuildLeave(char const*, char const*)
{
	CDataStore Data(CMSG_GUILD_DECLINE);
	Data.Finalize();
	ClientServices::SendPacket(&Data);

	Console::Write("Packet Sent", ECHO_COLOR);
	return true;
}

BOOL Inventory(char const*, char const*)
{
    CDataStore Packet(CMSG_GOSSIP_HELLO);
    Packet.PutInt64(GetTargetGUID());
    Packet.Finalize();
    ClientServices::SendPacket(&Packet);

    CDataStore Packet2(CMSG_LIST_INVENTORY);
    Packet2.PutInt64(GetTargetGUID());
    Packet2.Finalize();
    ClientServices::SendPacket(&Packet2);

    Console::Write("Packet Sent", ECHO_COLOR);
    return true;
}

BOOL UpdateTicket(char const*, char const* args)
{
    int Count = atoi(args);

    for (int i = 0; i <= Count; i++)
    {
        CDataStore Data(CMSG_GMTICKET_UPDATETEXT);
        Data.PutString("|cFFDDD000|Hquest:|hAzerothCorer|h|r");
        Data.Finalize();
        ClientServices::SendPacket(&Data);
    }

    Console::Write("Packet Sent", ECHO_COLOR);
    return true;
}

BOOL Away(char const*, char const* args)
{
    CDataStore Packet(CMSG_MESSAGECHAT);
    Packet.PutInt32(23);
    Packet.PutInt32(0);
    Packet.PutString(args);
    Packet.Finalize();
    ClientServices::SendPacket(&Packet);

    Console::Write("Packet Sent", ECHO_COLOR);
    return true;
}

BOOL CharacterCreate(char const*, char const* args)
{
    CDataStore Packet(CMSG_CHAR_CREATE);
    Packet.PutString(args);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.PutInt8(1);
    Packet.Finalize();
    ClientServices::SendPacket(&Packet);

    Console::Write("Packet Sent", ECHO_COLOR);
    return true;
}

BOOL SpellClick(char const*, char const*)
{
    CDataStore Data(CMSG_SPELLCLICK);
    Data.PutInt64(GetTargetGUID());
    Data.Finalize();
    ClientServices::SendPacket(&Data);

    Console::Write("Packet Sent, Target guid %u", ECHO_COLOR, GetTargetGUID());
    return true;
}

BOOL DuelAccept(char const*, char const* String)
{
	long Count = atoi(String);

	if (Count <= 0)
		Count += 1;

	for (long A = 0; A < Count; ++A)
	{
		if (Delay > 1)
			Sleep(Delay);

		CDataStore Data(CMSG_DUEL_ACCEPTED);
		Data.PutInt64(GetTargetGuid());
		Data.Finalize();
		ClientServices::SendPacket(&Data);
	}

	Console::Write("Packet Sent %u Times", ECHO_COLOR, Count);
	return true;
}
