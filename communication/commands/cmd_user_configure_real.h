#ifndef CMD_USER_CONFIGURE_REAL_H
#define CMD_USER_CONFIGURE_REAL_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserConfigureReal : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserConfigureReal( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:



};
using PCommandUserConfigureReal = std::shared_ptr<CommandUserConfigureReal>;

#endif // CMD_USER_CONFIGURE_REAL_H
