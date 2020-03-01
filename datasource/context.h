#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>

#include <dss_common/communication/rti/federate.h>

#include "node_mirror_simulation.h"
#include "node_mirror_real.h"
#include "realtime_synchronizer.h"


class IEditableContext {
public:
    virtual bool addSimulaNodeMirror( PNodeMirrorSimulation _node ) = 0;
    virtual bool addRealNodeMirror( PNodeMirrorReal _node ) = 0;
};
using PEditableContext = std::shared_ptr<IEditableContext>;


class Context : public INodeDispatcherObserver
              , public IEditableContext
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
            , settings(nullptr)
        {}
        bool active;
        SInitSettings * settings;
        std::string m_lastError;
    };

    Context();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }

    virtual bool addSimulaNodeMirror( PNodeMirrorSimulation _node ) override;
    virtual bool addRealNodeMirror( PNodeMirrorReal _node ) override;

    PNodeMirror getNode( const common_types::TNodeId & _id );
    const std::vector<PNodeMirror> & getNodes(){ return m_nodes; }

    // for status response
    const std::vector<PNodeMirrorSimulation> & getNodesSimula(){ return m_nodesSimula; }
    const std::vector<PNodeMirrorReal> & getNodesReal(){ return m_nodesReal; }




private:    
    virtual void callbackNodeDetected( const common_types::TNodeId & _id ) override;
    virtual void callbackNodeDisappeared( const common_types::TNodeId & _id ) override;




    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodesById;
    std::vector<PNodeMirrorSimulation> m_nodesSimula;
    std::vector<PNodeMirrorReal> m_nodesReal;


    // service
    RealtimeSynchronizer m_realTimeSynchronizer;
    // rti client



};
using PContext = std::shared_ptr<Context>;

#endif // CONTEXT_H
