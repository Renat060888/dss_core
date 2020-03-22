#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <thread>
#include <vector>
#include <unordered_map>

#include "node_dispatcher_simulation.h"
#include "node_dispatcher_real.h"
#include "node_dispatcher_dump.h"
#include "user_dispatcher.h"
#include "context.h"

// User <-> Context <-> NodeDispatcher <-> Node

// TODO: if server crashed - restore Users & Contexts. In order to client side don't have to know about failure

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

    // TODO: may be redundant ?
    struct SContextUsers {
        PContext context;
        std::vector<PUser> users;
    };

    SourceManagerFacade();
    ~SourceManagerFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();
    const std::string & getLastError(){ return m_lastError; }

    PContext getContext( common_types::TUserId _userId, common_types::TContextId _ctxId );
    void releaseContext( common_types::TUserId _userId, common_types::TContextId _ctxId );


private:
    virtual void callbackUserOnline( const common_types::TUserId & _id, bool _online ) override;

    void threadMaintenance();

    std::vector<common_types::TMissionId> getContextMission( common_types::TContextId _ctxId );

    void addRealNodesToContext( PEditableContext _ctx );
    void addSimulaNodesToContext( PEditableContext _ctx );


    // data
    PContext m_limboContext;
    std::vector<PContext> m_contexts;
    std::unordered_map<common_types::TContextId, PContext> m_contextsById;
    std::unordered_map<common_types::TContextId, SContextUsers> m_contextsUsers;

    std::string m_lastError;
    SInitSettings m_settings;


    // service
    NodeDispatcherSimulation m_nodeDispatcherSimula;
    NodeDispatcherReal m_nodeDispatcherReal;
    NodeDispatcherDump m_nodeDispatcherDump;
    UserDispatcher m_userDispatcher;

    std::thread * m_threadMaintenance;




};

#endif // SOURCE_MANAGER_H
