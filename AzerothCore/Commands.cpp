#include "stdafx.h"
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>

BOOL GuildLeave(char const*, char const* /* args */)
{
	CDataStore Data(CMSG_GUILD_DECLINE);
	Data.Finalize();
	ClientServices::SendPacket(&Data);

	Console::Write("Packet Sent", ECHO_COLOR);
	return true;
}
