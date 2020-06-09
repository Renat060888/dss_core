#ifndef NODE_DISPATCHER_REAL_H
#define NODE_DISPATCHER_REAL_H

#include <vector>
#include <map>

#include "i_node_worker_service.h"

class DispatcherNodeReal
{
    friend class CommandNodePing;
public:
    struct SNodeRequestForm {
        std::string serviceName; // for example: FlightRadar24, MarineTraffic, OpenWeatherMap, etc...
    };

    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string m_lastError;
    };

    DispatcherNodeReal();

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
    void updateNodeState( void * _state );

    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeMirror> m_nodesById;
    std::vector<INodeDispatcherObserver *> m_observers;


    // service



};

#endif // NODE_DISPATCHER_REAL_H
