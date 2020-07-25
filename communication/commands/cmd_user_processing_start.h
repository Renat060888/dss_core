#ifndef CMD_USER_START_PROCESSING_H
#define CMD_USER_START_PROCESSING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserProcessingStart : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserProcessingStart( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:


};
using PCommandUserProcessingStart = std::shared_ptr<CommandUserProcessingStart>;

#endif // CMD_USER_START_PROCESSING_H
