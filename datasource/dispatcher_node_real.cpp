
#include <algorithm>

#include <microservice_common/system/logger.h>

#include "dispatcher_node_real.h"
#include "node_worker_service_real.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "DispatcherReal:";

DispatcherNodeReal::DispatcherNodeReal()
{

}

bool DispatcherNodeReal::init( const SInitSettings & _settings ){




    return true;
}

void DispatcherNodeReal::addObserver( INodeRealObserver * _observer ){

    // add observer
    auto iter = std::find( m_observers.begin(), m_observers.end(), _observer );
    if( iter == m_observers.end() ){
        m_observers.push_back( _observer );
    }

    // immediately notify about already existing nodes
    for( auto & valuePair : m_nodeById ){
        PNodeWorkerServiceReal & node = valuePair.second;
        _observer->callbackNodeReal( node, true );
    }
}

void DispatcherNodeReal::removeObserver( INodeRealObserver * _observer ){

    m_observers.erase( std::remove(m_observers.begin(), m_observers.end(), _observer) );
}

void DispatcherNodeReal::runSystemClock(){

}

bool DispatcherNodeReal::requestNode( const SNodeRequestForm & _form ){


    // check NodeAgent existing



    return true;
}

void DispatcherNodeReal::releaseNode( common_types::TContextId _ctx ){

}

PNodeMirror DispatcherNodeReal::getNode( const common_types::TNodeId & _id ){



    return nullptr;
}

std::vector<PNodeMirror> DispatcherNodeReal::getNodes(){


    return std::vector<PNodeMirror>();
}


void DispatcherNodeReal::updateNodeAgentState( const common_types::SNodeAgentRealState & _state ){





    PNodeWorkerServiceReal node = std::make_shared<NodeWorkerServiceReal>();







    m_nodes.push_back( node );


}
