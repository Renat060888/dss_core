#ifndef CMD_USER_PAUSE_PROCESSING_H
#define CMD_USER_PAUSE_PROCESSING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserProcessingPause : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserProcessingPause( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;



private:

};
using PCommandUserProcessingPause = std::shared_ptr<CommandUserProcessingPause>;

#endif // CMD_USER_PAUSE_PROCESSING_H
