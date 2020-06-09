#ifndef CMD_NODE_PING_H
#define CMD_NODE_PING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandNodeAgentPing : public ICommandExternal
{
public:
    CommandNodeAgentPing( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:



};
using PCommandNodeAgentPing = std::shared_ptr<CommandNodeAgentPing>;

#endif // CMD_NODE_PING_H
