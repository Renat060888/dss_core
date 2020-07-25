#ifndef CMD_NODE_PING_H
#define CMD_NODE_PING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandNodeAgentPing : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandNodeAgentPing( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:
    common_types::SNodeAgentSimulateState m_stateFromSimulationAgent;
    common_types::SNodeAgentRealState m_stateFromRealAgent;

};
using PCommandNodeAgentPing = std::shared_ptr<CommandNodeAgentPing>;

#endif // CMD_NODE_PING_H
