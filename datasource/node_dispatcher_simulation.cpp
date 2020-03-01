
#include "node_dispatcher_simulation.h"
#include "node_mirror_simulation.h"

using namespace std;

NodeDispatcherSimulation::NodeDispatcherSimulation()
{

}

bool NodeDispatcherSimulation::init( const SInitSettings & _settings ){



}

void NodeDispatcherSimulation::addObserver( INodeDispatcherObserver * _observer ){

}

void NodeDispatcherSimulation::removeObserver( INodeDispatcherObserver * _observer ){

}

void NodeDispatcherSimulation::runSystemClock(){

}

bool NodeDispatcherSimulation::createNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

PNodeMirror NodeDispatcherSimulation::getNode( const common_types::TNodeId & _id ){

}

std::vector<PNodeMirror> NodeDispatcherSimulation::getNodes(){

}

void NodeDispatcherSimulation::releaseNode( PNodeMirror _nodeMirror ){

}

void NodeDispatcherSimulation::updateNodeState( void * _state ){





    PNodeMirrorSimulation node = std::make_shared<NodeMirrorSimulation>();







    m_nodes.push_back( node );


}
