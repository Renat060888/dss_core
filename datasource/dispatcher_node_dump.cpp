
#include "dispatcher_node_dump.h"

using namespace std;

DispatcherNodeDump::DispatcherNodeDump()
{

}

bool DispatcherNodeDump::init( const SInitSettings & _settings ){

}

void DispatcherNodeDump::addObserver( INodeDispatcherObserver * _observer ){


}

void DispatcherNodeDump::removeObserver( INodeDispatcherObserver * _observer ){

}

void DispatcherNodeDump::runSystemClock(){

}

bool DispatcherNodeDump::requestNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

PNodeMirror DispatcherNodeDump::getNode( const common_types::TNodeId & _id ){

}

std::vector<PNodeMirror> DispatcherNodeDump::getNodes(){

}

void DispatcherNodeDump::releaseNode( PNodeMirror _nodeMirror ){


}
