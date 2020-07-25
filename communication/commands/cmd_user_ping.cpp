
#include "cmd_user_ping.h"

using namespace std;

CommandUserPing::CommandUserPing( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandUserPing::exec(){

    const string resp = "{ \"bla\" : \"ble\" }";
    sendResponse( resp  );

    return true;
}

