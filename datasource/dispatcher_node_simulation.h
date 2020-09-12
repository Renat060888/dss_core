#ifndef NODE_DISPATCHER_SIMULATION_H
#define NODE_DISPATCHER_SIMULATION_H

#include <vector>
#include <map>

#include "node_agent_simulation_proxy.h"
#include "node_worker_service_simulation.h"

class DispatcherNodeSimulation
{
    friend class CommandNodeAgentPing;
public:
    struct SNodeFilter {
        SNodeFilter()
            : contextId(0)
            , missionId(0)
        {}
        common_types::TContextId contextId;
        common_types::TMissionId missionId;
    };

    struct SInitSettings {
        common_types::IServiceInternalCommunication * internalCommunication;
    };

    struct SState {
        SInitSettings settings;
        std::string lastError;
    };

    DispatcherNodeSimulation();
    ~DispatcherNodeSimulation();

    bool init( const SInitSettings & _settings );
    void runSystemClock();
    const SState & getState(){ return m_state; }

    void addObserver( INodeSimulationObserver * _observer );
    void removeObserver( INodeSimulationObserver * _observer );

    bool requestNode( const SNodeFilter & _form );
    void releaseNode( const SNodeFilter & _form );
    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();



private:
    // from ping
    void updateNodeAgentState( const common_types::SNodeAgentSimulateState & _state );

    void updateAgentWorkers( PNodeAgentSimulationProxy & _agent, const std::vector<common_types::SNodeWorkerSimulationState> & _nodeWorkers );



    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodesWorkers;
    std::map<common_types::TNodeId, PNodeWorkerServiceSimula> m_nodeWorkersById;
    std::map<common_types::TNodeId, PNodeAgentSimulationProxy> m_nodeAgentsById;
    std::vector<INodeSimulationObserver *> m_observers;


    // service


};

#endif // NODE_DISPATCHER_SIMULATION_H
