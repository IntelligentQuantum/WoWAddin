#include "stdafx.h"
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>

UINT64 GetTargetGUID()
{
    UINT64 guid = -1;

    if (CGObject_C* player = ObjectMgr::GetObjectPtr(ObjectMgr::GetActivePlayerGuid(), TYPEMASK_PLAYER))
        if (CGObject_C* target = ObjectMgr::GetObjectPtr(player->GetValue<UINT64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT))
            guid = target->GetValue<UINT64>(OBJECT_FIELD_GUID);

    return guid;
}

BOOL GuildLeave(char const*, char const* /* args */)
{
	CDataStore Data(CMSG_GUILD_DECLINE);
	Data.Finalize();
	ClientServices::SendPacket(&Data);

	Console::Write("Packet Sent", ECHO_COLOR);
	return true;
}

BOOL Inventory(char const*, char const* /* args */)
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
