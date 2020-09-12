
#include <algorithm>

#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include "dispatcher_node_simulation.h"
#include "node_worker_service_simulation.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "DispatcherSimula:";

// > functors
struct FNodeWorkerSimulaEqual {
    FNodeWorkerSimulaEqual( TNodeId _nodeId )
        : workerId(_nodeId)
    {}

    bool operator()( PNodeWorkerServiceSimula & _worker ){
        return ( _worker->getState().id == workerId );
    }

    TNodeId workerId;
};

struct FNodeAgentSimulaEqual {
    FNodeAgentSimulaEqual( TNodeId _nodeId )
        : agentId(_nodeId)
    {}

    bool operator()( PNodeAgentSimulationProxy & _agent ){
        return ( _agent->getState().nodeAgentId == agentId );
    }

    TNodeId agentId;
};
// < functors

DispatcherNodeSimulation::DispatcherNodeSimulation()
{

}

DispatcherNodeSimulation::~DispatcherNodeSimulation()
{

}

bool DispatcherNodeSimulation::init( const SInitSettings & _settings ){

    m_state.settings = _settings;







    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void DispatcherNodeSimulation::addObserver( INodeSimulationObserver * _observer ){

    // add observer
    auto iter = std::find( m_observers.begin(), m_observers.end(), _observer );
    if( iter == m_observers.end() ){
        m_observers.push_back( _observer );
    }

    // immediately notify it about already existing nodes
    for( auto & valuePair : m_nodeWorkersById ){
        PNodeWorkerServiceSimula & node = valuePair.second;
        _observer->callbackNodeSimulation( node, true );
    }
}

void DispatcherNodeSimulation::removeObserver( INodeSimulationObserver * _observer ){

    auto iter = std::remove( m_observers.begin(), m_observers.end(), _observer );
    if( iter != m_observers.end() ){
        m_observers.erase( iter );
    }
}

void DispatcherNodeSimulation::runSystemClock(){

    static constexpr int64_t NODE_AGENT_DISAPPEAR_TIMEOUT_MILLISEC = 30000;
    static constexpr int64_t NODE_WORKER_DISAPPEAR_TIMEOUT_MILLISEC = 30000;

    // watch for disappeared agents... ( also their workers )
    for( auto iterA = m_nodeAgentsById.begin(); iterA != m_nodeAgentsById.end(); ){
        PNodeAgentSimulationProxy & agent = iterA->second;

        if( (common_utils::getCurrentTimeMillisec() - agent->getState().lastPingAtMillisec) > NODE_AGENT_DISAPPEAR_TIMEOUT_MILLISEC ){
            VS_LOG_WARN << PRINT_HEADER << " agent disappeared with id: " << agent->getState().nodeAgentId << endl;

            // first of all destroy it's workers
            for( auto iterW = m_nodeWorkersById.begin(); iterW != m_nodeWorkersById.end(); ){
                PNodeWorkerServiceSimula & worker = iterW->second;

                if( worker->getState().agentId == agent->getState().nodeAgentId ){
                    for( INodeSimulationObserver * observer : m_observers ){
                        observer->callbackNodeSimulation( worker, false );
                    }

                    iterW = m_nodeWorkersById.erase( iterW );
                }
                else{
                    ++iterW;
                }
            }

            iterA = m_nodeAgentsById.erase( iterA );
        }
        else{
            ++iterA;
        }
    }

    // ...and workers of existing agents
    for( auto iter = m_nodeWorkersById.begin(); iter != m_nodeWorkersById.end(); ){
        PNodeWorkerServiceSimula & worker = iter->second;

        if( (common_utils::getCurrentTimeMillisec() - worker->getState().lastPingAtMillisec) > NODE_WORKER_DISAPPEAR_TIMEOUT_MILLISEC ){
            VS_LOG_WARN << PRINT_HEADER << " worker disappeared for context: " << worker->getState().ctxId << endl;

            for( INodeSimulationObserver * observer : m_observers ){
                observer->callbackNodeSimulation( worker, false );
            }

            iter = m_nodeWorkersById.erase( iter );
        }
        else{
            ++iter;
        }
    }
}

bool DispatcherNodeSimulation::requestNode( const SNodeFilter & _form ){

    // TODO: what if there are multiple agents ? ( depends on politic )

    if( ! m_nodeAgentsById.empty() ){
        PNodeAgentSimulationProxy agent = m_nodeAgentsById.begin()->second;

        const bool rt = agent->requestWorkers( _form.contextId );
        return rt;
    }
    else{
        stringstream ss; ss << "no available agents for simulation node request for context: " << _form.contextId;
        m_state.lastError = ss.str();
        VS_LOG_ERROR << PRINT_HEADER << " " << ss.str() << endl;
        return false;
    }
}

void DispatcherNodeSimulation::releaseNode( const SNodeFilter & _form ){

    for( auto iterW = m_nodeWorkersById.begin(); iterW != m_nodeWorkersById.end(); ++iterW ){
        PNodeWorkerServiceSimula & worker = iterW->second;

        // workers related to this context
        if( worker->getState().ctxId == _form.contextId ){
            auto iterA = m_nodeAgentsById.find( worker->getState().agentId );

            assert( iterA == m_nodeAgentsById.end() && "worker without agent" );

            // destroy via it's agent
            if( iterA != m_nodeAgentsById.end() ){
                PNodeAgentSimulationProxy agent = iterA->second;
                agent->releaseWorker( worker->getState().id );
            }
        }
    }
}

PNodeMirror DispatcherNodeSimulation::getNode( const common_types::TNodeId & _id ){

    auto iter = m_nodeWorkersById.find( _id );
    if( iter != m_nodeWorkersById.end() ){
        return iter->second;
    }

    return nullptr;
}

std::vector<PNodeMirror> DispatcherNodeSimulation::getNodes(){
    return m_nodesWorkers;
}

void DispatcherNodeSimulation::updateAgentWorkers( PNodeAgentSimulationProxy & _agent, const vector<SNodeWorkerSimulationState> & _nodeWorkers ){

    for( const SNodeWorkerSimulationState & workerState : _nodeWorkers ){

        // update existing worker
        auto iterWorker = m_nodeWorkersById.find( workerState.id );
        if( iterWorker != m_nodeWorkersById.end() ){
            PNodeWorkerServiceSimula node = iterWorker->second;
            node->updateState( workerState );
        }
        // new worker
        else{
            PNodeWorkerServiceSimula node = std::make_shared<NodeWorkerServiceSimula>( _agent );
            node->updateState( workerState );
            m_nodesWorkers.push_back( node );

            for( INodeSimulationObserver * observer : m_observers ){
                observer->callbackNodeSimulation( node, true );
            }
        }
    }
}

void DispatcherNodeSimulation::updateNodeAgentState( const common_types::SNodeAgentSimulateState & _state ){

    auto iter = m_nodeAgentsById.find( _state.nodeAgentId );
    if( iter != m_nodeAgentsById.end() ){
        // update existing agent
        PNodeAgentSimulationProxy & agent = iter->second;
        agent->updateState( _state );

        // update workers
        updateAgentWorkers( agent, _state.nodeWorkers );
    }
    else{
        // new agent
        NodeAgentSimulationProxy::SInitSettings settingsAgent;
        settingsAgent.network = m_state.settings.internalCommunication->getNodeAgentCommunicator( _state.nodeAgentId );

        PNodeAgentSimulationProxy agent = std::make_shared<NodeAgentSimulationProxy>();
        agent->updateState( _state );

        if( agent->init( settingsAgent ) ){
            m_nodeAgentsById.insert( {_state.nodeAgentId, agent} );

            // it's workers
            updateAgentWorkers( agent, _state.nodeWorkers );
        }
    }
}






