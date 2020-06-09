#ifndef NODE_DISPATCHER_SIMULATION_H
#define NODE_DISPATCHER_SIMULATION_H

#include <vector>
#include <map>

#include "i_node_worker_service.h"
#include "node_agent_simulation_proxy.h"

class DispatcherNodeSimulation
{
    friend class CommandNodePing;
public:
    struct SNodeRequestForm {
        int contextId;
        int missionId;
    };

    struct SInitSettings {
        common_types::IServiceInternalCommunication * internalCommunication;
    };

    struct SState {
        SInitSettings settings;
        std::string m_lastError;
    };

    DispatcherNodeSimulation();

    bool init( const SInitSettings & _settings );
    void addObserver( INodeDispatcherObserver * _observer );
    void removeObserver( INodeDispatcherObserver * _observer );
    void runSystemClock();
    const SState & getState(){ return m_state; }

    bool requestNode( const SNodeRequestForm & _form );
    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();
    void releaseNode( PNodeMirror _nodeMirror );



private:
    void updateNodeAgentState( const common_types::SNodeAgentSimulateState & _state );

    // data
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodeById;
    std::vector<INodeDispatcherObserver *> m_observers;
    std::map<common_types::TNodeId, PNodeAgentSimulationProxy> m_nodeAgentById;
    SState m_state;


    // service




};

#endif // NODE_DISPATCHER_SIMULATION_H
