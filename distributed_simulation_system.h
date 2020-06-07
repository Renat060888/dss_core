#ifndef DISTRIBUTED_SIMULATION_SYSTEM_H
#define DISTRIBUTED_SIMULATION_SYSTEM_H

#include <string>
#include <future>

#include <boost/signals2.hpp>
#include <microservice_common/system/system_environment_facade.h>
#include <dss_common/common/common_types.h>

#include "communication/communication_gateway_facade_dss.h"
#include "datasource/source_manager_facade.h"
#include "storage/storage_engine_facade.h"
#include "analyze/analytic_manager_facade.h"

class DistributedSimulationSystem
{
    static void callbackUnixInterruptSignal();
    static boost::signals2::signal<void()> m_unixInterruptSignal;
public:
    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string lastError;
    };

    DistributedSimulationSystem();
    ~DistributedSimulationSystem();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }

    void launch();


private:
    void shutdown();
    void shutdownByUnixInterruptSignal();
    void checkForSelfShutdown();

    // data
    SState m_state;
    common_types::SIncomingCommandServices m_commandServices;
    std::atomic<bool> m_shutdownCalled;
    std::future<void> m_selfShutdownFuture;

    // service
    CommunicationGatewayFacadeDSS * m_communicateGateway;
    SourceManagerFacade * m_sourceManager;
    AnalyticManagerFacade * m_analyticManager;
    StorageEngineFacade * m_storageEngine;
    SystemEnvironmentFacade * m_systemEnvironment;
};

#endif // DISTRIBUTED_SIMULATION_SYSTEM_H
