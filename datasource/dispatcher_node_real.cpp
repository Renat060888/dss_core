
#include "dispatcher_node_real.h"
#include "node_worker_service_real.h"

using namespace std;

DispatcherNodeReal::DispatcherNodeReal()
{

}

bool DispatcherNodeReal::init( const SInitSettings & _settings ){



}

void DispatcherNodeReal::addObserver( INodeDispatcherObserver * _observer ){

    // add observer

    // immediately notify about already existing nodes
}

void DispatcherNodeReal::removeObserver( INodeDispatcherObserver * _observer ){

}

void DispatcherNodeReal::runSystemClock(){

}

bool DispatcherNodeReal::requestNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

PNodeMirror DispatcherNodeReal::getNode( const common_types::TNodeId & _id ){


}

std::vector<PNodeMirror> DispatcherNodeReal::getNodes(){


    return std::vector<PNodeMirror>();
}

void DispatcherNodeReal::releaseNode( PNodeMirror _nodeMirror ){

}

void DispatcherNodeReal::updateNodeState( void * _state ){





    PNodeWorkerServiceReal node = std::make_shared<NodeWorkerServiceReal>();







    m_nodes.push_back( node );


}
