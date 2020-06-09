
#include "dispatcher_node_simulation.h"
#include "node_worker_service_simulation.h"

using namespace std;

DispatcherNodeSimulation::DispatcherNodeSimulation()
{

}

bool DispatcherNodeSimulation::init( const SInitSettings & _settings ){



}

void DispatcherNodeSimulation::addObserver( INodeDispatcherObserver * _observer ){

}

void DispatcherNodeSimulation::removeObserver( INodeDispatcherObserver * _observer ){

}

void DispatcherNodeSimulation::runSystemClock(){

}

bool DispatcherNodeSimulation::requestNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

PNodeMirror DispatcherNodeSimulation::getNode( const common_types::TNodeId & _id ){

}

std::vector<PNodeMirror> DispatcherNodeSimulation::getNodes(){

}

void DispatcherNodeSimulation::releaseNode( PNodeMirror _nodeMirror ){

}

void DispatcherNodeSimulation::updateNodeAgentState( const common_types::SNodeAgentSimulateState & _state ){

    NodeAgentSimulationProxy::SInitSettings settingsAgent;
    settingsAgent.network = m_state.settings.internalCommunication->getNodeAgentCommunicator( _state.nodeAgentId );

    NodeAgentSimulationProxy nodeAgent;
//    nodeAgent.

    PNodeWorkerServiceSimula node = std::make_shared<NodeWorkerServiceSimula>( & nodeAgent );



    // track nodes snapshot inside agent pong





    m_nodes.push_back( node );


}
