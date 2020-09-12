#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <thread>
#include <vector>
#include <unordered_map>

#include "dispatcher_node_simulation.h"
#include "dispatcher_node_real.h"
#include "dispatcher_node_dump.h"
#include "dispatcher_user.h"
#include "context.h"

// TODO: if server crashed - restore Users & Contexts. In order to client side don't have to know about any failure

class SourceManagerFacade : public common_types::IUserDispatcherObserver
{
public:
    struct SServiceLocator {
        common_types::IServiceInternalCommunication * internalCommunication;
        common_types::IServiceExternalCommunication * externalCommunication;
        common_types::IServiceUserAuthorization * authorizator;
    };

    struct SInitSettings {
        SServiceLocator services;
    };

    struct SState {
        SInitSettings settings;
        std::string lastError;
    };

    SourceManagerFacade();
    ~SourceManagerFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();
    const SState & getState(){ return m_state; }

    // REQUEST FROM USER
    bool openContext( common_types::TUserId _userId, common_types::TContextId _ctxId );
    void closeContext( common_types::TUserId _userId );
    PContext getContext( const common_types::TUserId & _userId );

    // RESPONSE FROM AGENT
    DispatcherNodeSimulation * getDispatcherNodeSimulation();
    DispatcherNodeReal * getDispatcherNodeReal();


private:
    struct SCallbackUserOnlineArgs {
        common_types::TUserId id;
        bool online;
    };

    virtual void callbackUserOnline( const common_types::TUserId & _id, bool _online ) override;    
    void processUserOnlineCallbacks();

    void threadMaintenance();

    // data
    SState m_state;
    bool m_shutdownCalled;
    std::queue<SCallbackUserOnlineArgs> m_userOnlineCallbacks;

    PContext m_limboContext;
    std::vector<PContext> m_contexts;
    std::unordered_map<common_types::TContextId, PContext> m_contextsById;
    std::map<common_types::TUserId, PContext> m_contextsByUserId;

    // service
    DispatcherNodeSimulation m_nodeDispatcherSimula;
    DispatcherNodeReal m_nodeDispatcherReal;
    DispatcherNodeDump m_nodeDispatcherDump;
    UserDispatcher m_userDispatcher;

    std::thread * m_trMaintenance;
    std::mutex m_muUserOnlineCallbacks;
};

#endif // SOURCE_MANAGER_H
