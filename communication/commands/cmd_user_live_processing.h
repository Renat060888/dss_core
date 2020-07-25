#ifndef CMD_USER_LIVE_PROCESSING_H
#define CMD_USER_LIVE_PROCESSING_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserLiveProcessing : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserLiveProcessing( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;



private:



};
using PCommandUserLiveProcessing = std::shared_ptr<CommandUserLiveProcessing>;

#endif // CMD_USER_LIVE_PROCESSING_H
