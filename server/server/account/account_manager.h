#ifndef _ACCOUNT_MANAGER_H_
#define _ACCOUNT_MANAGER_H_

#include "singleton.h"

#include "network/packet/packet_handler.h"
#include "network/client_session.h"

class AccountManager : public net::Singleton<AccountManager>
{
public:

    void Start();

private:
    void CreateAccountAck(PacketHandler::session::Shared session, PacketHandler::message& msg);
};

#endif