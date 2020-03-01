
#include "node_dispatcher_real.h"
#include "node_mirror_real.h"

using namespace std;

NodeDispatcherReal::NodeDispatcherReal()
{

}

bool NodeDispatcherReal::init( const SInitSettings & _settings ){



}

void NodeDispatcherReal::addObserver( INodeDispatcherObserver * _observer ){

}

void NodeDispatcherReal::removeObserver( INodeDispatcherObserver * _observer ){

}

void NodeDispatcherReal::runSystemClock(){

}

bool NodeDispatcherReal::createNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

PNodeMirror NodeDispatcherReal::getNode( const common_types::TNodeId & _id ){


}

PNodeMirror NodeDispatcherReal::getNode( const SNodeRequestForm & _form ){

}

std::vector<PNodeMirror> NodeDispatcherReal::getNodes(){


    return std::vector<PNodeMirror>();
}

void NodeDispatcherReal::releaseNode( PNodeMirror _nodeMirror ){

}

void NodeDispatcherReal::updateNodeState( void * _state ){





    PNodeMirrorReal node = std::make_shared<NodeMirrorReal>();







    m_nodes.push_back( node );


}
