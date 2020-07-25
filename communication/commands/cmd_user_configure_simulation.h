#ifndef CMD_USER_CONFIGURE_SIMULATION_H
#define CMD_USER_CONFIGURE_SIMULATION_H

#include <microservice_common/communication/i_command_external.h>
#include <dss_common/common/common_types.h>

class CommandUserConfigureSimulation : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandUserConfigureSimulation( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:



};
using PCommandUserConfigureSimulation = std::shared_ptr<CommandUserConfigureSimulation>;

#endif // CMD_USER_CONFIGURE_SIMULATION_H
