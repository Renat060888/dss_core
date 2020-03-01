#ifndef CMD_NODE_PING_H
#define CMD_NODE_PING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandNodePing : public ICommandExternal
{
public:
    CommandNodePing( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:



};
using PCommandNodePing = std::shared_ptr<CommandNodePing>;

#endif // CMD_NODE_PING_H
