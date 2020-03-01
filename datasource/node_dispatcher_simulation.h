#ifndef NODE_DISPATCHER_SIMULATION_H
#define NODE_DISPATCHER_SIMULATION_H

#include <vector>
#include <map>

#include "i_node_mirror.h"

class NodeDispatcherSimulation
{
    friend class CommandNodePing;
public:
    struct SNodeRequestForm {
        int contextId;
        int missionId;
    };

    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string m_lastError;
    };

    NodeDispatcherSimulation();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }
    void addObserver( INodeDispatcherObserver * _observer );
    void removeObserver( INodeDispatcherObserver * _observer );
    void runSystemClock();

    bool createNode( const SNodeRequestForm & _form );
    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();

    void releaseNode( PNodeMirror _nodeMirror );



private:
    void updateNodeState( void * _state );

    // data
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodesById;
    std::vector<INodeDispatcherObserver *> m_observers;
    SState m_state;


    // service




};

#endif // NODE_DISPATCHER_SIMULATION_H
