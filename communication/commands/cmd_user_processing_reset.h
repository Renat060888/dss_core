#ifndef CMD_USER_RESET_PROCESSING_H
#define CMD_USER_RESET_PROCESSING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserProcessingReset : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserProcessingReset( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;



private:



};
using PCommandUserProcessingReset = std::shared_ptr<CommandUserProcessingReset>;

#endif // CMD_USER_RESET_PROCESSING_H
