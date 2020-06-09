#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <thread>

#include <dss_common/communication/rti/federate.h>

#include "node_worker_service_simulation.h"
#include "node_worker_service_real.h"
#include "node_worker_service_dump.h"
#include "realtime_synchronizer.h"


class IEditableContext {
public:
    virtual bool addNodeWorkerServiceSimula( PNodeWorkerServiceSimula _node ) = 0;
    virtual bool addNodeWorkerServiceReal( PNodeWorkerServiceReal _node ) = 0;
    virtual void removeNodeWorkerService( const common_types::TNodeId & _id ) = 0;
};
using PEditableContext = std::shared_ptr<IEditableContext>;


class Context : public IEditableContext
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
            : active(false)
            , realTimeSync(false)
        {}
        bool active;
        bool realTimeSync;
        SInitSettings settings;
        std::string lastError;
    };

    Context();
    ~Context();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }

    // ( for real time check, processing interval check, RTI sync launch / stop, etc )
    void configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceSimula::SConfigSimulation & _conf );
    void configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceReal::SConfigReal & _conf );
    void configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceDump::SConfigDump & _conf );
    void makeNodeStatic( const common_types::TNodeId & _id, bool _static );
    void startNode( const common_types::TNodeId & _id );
    void pauseNode( const common_types::TNodeId & _id );
    void stopNode( const common_types::TNodeId & _id );

    void setLiveProcessing( bool _live );

    // for status response
    const std::vector<PNodeWorkerServiceSimula> & getNodesSimula(){ return m_nodesSimula; }
    const std::vector<PNodeWorkerServiceReal> & getNodesReal(){ return m_nodesReal; }


private:    
    virtual bool addNodeWorkerServiceSimula( PNodeWorkerServiceSimula _node ) override;
    virtual bool addNodeWorkerServiceReal( PNodeWorkerServiceReal _node ) override;
    virtual void removeNodeWorkerService( const common_types::TNodeId & _id ) override;

    void threadRealTimeSync();

    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodesById;
    std::vector<PNodeWorkerServiceSimula> m_nodesSimula;
    std::vector<PNodeWorkerServiceReal> m_nodesReal;


    // service
    RealtimeSynchronizer m_realTimeSynchronizer;
    std::thread * m_trRealTimeSync;



};
using PContext = std::shared_ptr<Context>;

#endif // CONTEXT_H
