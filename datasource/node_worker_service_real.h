#ifndef NODE_REAL_H
#define NODE_REAL_H

#include "i_node_worker_service.h"

class NodeWorkerServiceReal : public INodeMirror
{
    friend class DispatcherNodeReal;
public:
    NodeWorkerServiceReal( common_types::PNodeAgentFacilityForWorker _nodeAgent );
    ~NodeWorkerServiceReal();

    bool configure( const common_types::SConfigReal & _cfg );
    const common_types::SNodeWorkerRealState & getState();
    virtual const common_types::SNodeState & getBaseState() override;


    virtual bool start() override;
    virtual bool pause() override;
    virtual bool stop() override;

    virtual void switchLivePlaying( bool _live ) override;
    virtual void useRTI( bool _use ) override;


private:
    void updateState( const common_types::SNodeWorkerRealState & _workerState );

    // data
    common_types::SNodeWorkerRealState m_state;

    // service
    common_types::PNodeAgentFacilityForWorker m_nodeAgentFacility;
};
using PNodeWorkerServiceReal = std::shared_ptr<NodeWorkerServiceReal>;


// observer
class INodeRealObserver {
public:
    ~INodeRealObserver(){}

    virtual void callbackNodeReal( PNodeWorkerServiceReal _node, bool _online ) = 0;
};

#endif // NODE_REAL_H
