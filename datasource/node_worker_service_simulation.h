#ifndef NODE_SIMULATION_H
#define NODE_SIMULATION_H

#include "i_node_worker_service.h"
#include "node_agent_simulation_proxy.h"

class NodeWorkerServiceSimula : public INodeMirror
{
    friend class DispatcherNodeSimulation;
public:
    NodeWorkerServiceSimula( common_types::PNodeAgentFacilityForWorker _nodeAgent );
    ~NodeWorkerServiceSimula();

    bool configure( const common_types::SConfigSimulation & _cfg );
    const common_types::SNodeWorkerSimulationState & getState();
    virtual const common_types::SNodeState & getBaseState() override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool stop() override;

    virtual void switchLivePlaying( bool _live ) override;
    virtual void useRTI( bool _use ) override;


private:
    void updateState( const common_types::SNodeWorkerSimulationState & _workerState );

    // data
    common_types::SNodeWorkerSimulationState m_state;

    // service
    common_types::PNodeAgentFacilityForWorker m_nodeAgentFacility;


};
using PNodeWorkerServiceSimula = std::shared_ptr<NodeWorkerServiceSimula>;


// observer
class INodeSimulationObserver {
public:
    ~INodeSimulationObserver(){}

    virtual void callbackNodeSimulation( PNodeWorkerServiceSimula _node, bool _online ) = 0;
};

#endif // NODE_SIMULATION_H
