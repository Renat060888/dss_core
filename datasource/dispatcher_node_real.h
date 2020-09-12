#ifndef NODE_DISPATCHER_REAL_H
#define NODE_DISPATCHER_REAL_H

#include <vector>
#include <map>

#include "i_node_worker_service.h"
#include "node_agent_real_proxy.h"
#include "node_worker_service_real.h"


class DispatcherNodeReal
{
    friend class CommandNodeAgentPing;
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
    void runSystemClock();
    const SState & getState(){ return m_state; }

    void addObserver( INodeRealObserver * _observer );
    void removeObserver( INodeRealObserver * _observer );

    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();


private:
    // from ping
    void updateNodeAgentState( const common_types::SNodeAgentRealState & _state );

    // data
    SState m_state;
    std::vector<PNodeMirror> m_nodes;
    std::map<common_types::TNodeId, PNodeWorkerServiceReal> m_nodeById;
    std::vector<INodeRealObserver *> m_observers;


    // service



};

#endif // NODE_DISPATCHER_REAL_H
