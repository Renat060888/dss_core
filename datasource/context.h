#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <set>

#include "node_worker_service_dump.h"
#include "node_worker_service_real.h"
#include "node_worker_service_simulation.h"
#include "realtime_synchronizer.h"

class Context : public INodeSimulationObserver
              , public INodeRealObserver
{
public:
    struct SInitSettings {
        SInitSettings()
            : limboContext(false)
            , ctxId(common_vars::INVALID_CONTEXT_ID)
        {}
        bool limboContext;
        common_types::TContextId ctxId;
    };

    struct SState {
        SState()
            : realTimeSync(false)
            , processingIntervalMillisec(0)
        {}
        bool realTimeSync;
        int64_t processingIntervalMillisec;
        SInitSettings settings;
        std::string lastError;
    };

    Context();
    ~Context();

    bool init( const SInitSettings & _settings );
    const SState & getState() const { return m_state; }

    // NOTE: define clear mechanisms & politics of jointly working nodes in order to avoid combinatorial explosion of work modes
    void configureNode( const common_types::TNodeId & _id, const common_types::SConfigSimulation & _cfg );
    void configureNode( const common_types::TNodeId & _id, const common_types::SConfigReal & _cfg );
    void configureNode( const common_types::TNodeId & _id, const common_types::SConfigDump & _cfg );

    bool startNode( const common_types::TNodeId & _id );
    void pauseNode( const common_types::TNodeId & _id );
    void stopNode( const common_types::TNodeId & _id );

    bool isHasActiveNodes() const;
    void setLiveProcessing( bool _live );

    bool isHasUsers() const;
    bool isHasUser( common_types::TUserId _userId ) const;
    void includeUser( common_types::TUserId _userId );
    void excludeUser( common_types::TUserId _userId );

    // for status response
    const std::vector<PNodeWorkerServiceSimula> & getNodesSimula() const { return m_nodesSimula; }
    const std::vector<PNodeWorkerServiceReal> & getNodesReal() const { return m_nodesReal; }


private:    
    virtual void callbackNodeSimulation( PNodeWorkerServiceSimula _node, bool _online ) override;
    virtual void callbackNodeReal( PNodeWorkerServiceReal _node, bool _online ) override;

    void makeNodeStatic( const common_types::TNodeId & _id, bool _static );


    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodesById;
    std::vector<PNodeWorkerServiceSimula> m_nodesSimula;
    std::vector<PNodeWorkerServiceReal> m_nodesReal;
    std::set<common_types::TUserId> m_users;
    bool m_shutdownCalled;

    // service
    RealtimeSynchronizer m_realTimeSynchronizer;
    class DatasourceDescriptor * dd;
    class DatasourceReader * dr;



};
using PContext = std::shared_ptr<Context>;

#endif // CONTEXT_H
