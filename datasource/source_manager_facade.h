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

// User <-> Context <-> NodeWorkerService <- NodeDispatcher -> NodeAgentProxy

// TODO: if server crashed - restore Users & Contexts. In order to client side don't have to know about failure

class SourceManagerFacade : public common_types::IUserDispatcherObserver,
                            public INodeDispatcherObserver
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

    PContext getContext( common_types::TUserId _userId, common_types::TContextId _ctxId );
    void releaseContext( common_types::TUserId _userId, common_types::TContextId _ctxId );


private:
    virtual void callbackUserOnline( const common_types::TUserId & _id, bool _online ) override;    
    virtual void callbackNodeOnline( const common_types::TNodeId & _id, bool _online ) override;

    void threadMaintenance();

    void addRealNodesToContext( PEditableContext _ctx );
    void addSimulaNodesToContext( PEditableContext _ctx );

    // data
    SState m_state;
    PContext m_limboContext;
    std::vector<PContext> m_contexts;
    std::unordered_map<common_types::TContextId, PContext> m_contextsById;
    bool m_shutdownCalled;

    // service
    DispatcherNodeSimulation m_nodeDispatcherSimula;
    DispatcherNodeReal m_nodeDispatcherReal;
    DispatcherNodeDump m_nodeDispatcherDump;
    UserDispatcher m_userDispatcher;
    std::thread * m_threadMaintenance;
};

#endif // SOURCE_MANAGER_H
