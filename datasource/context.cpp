
#include <algorithm>

#include <microservice_common/system/logger.h>

#include "context.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "Context:";

Context::Context()
    : m_shutdownCalled(false)
{

}

Context::~Context(){

    VS_LOG_INFO << PRINT_HEADER << " begin shutdown" << endl;

    m_shutdownCalled = true;    




    VS_LOG_INFO << PRINT_HEADER << " shutdown success" << endl;
}

bool Context::init( const SInitSettings & _settings ){

    m_state.settings = _settings;





    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void Context::configureNode( const common_types::TNodeId & _id, const common_types::SConfigSimulation & _cfg ){

    // NOTE: main purpose of configuration is set worker to READY state




    // TODO: cancel configure if some of this conditions are failed:
    // 1 for real time check
    // 2 processing interVal check
    // 3 RTI sync launch / stop, etc
    // 4 check available physic models(1) on node with context objects(2)

    auto iter = std::find_if( m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_id) );
    if( iter != m_nodesSimula.end() ){
        PNodeWorkerServiceSimula node = ( * iter );

        const bool rt = node->configure( _cfg );

        // watch for configs of other nodes
    }
}

void Context::configureNode( const common_types::TNodeId & _id, const common_types::SConfigReal & _cfg ){

    // NOTE: main purpose of configuration is set worker to READY state

    auto iter = std::find_if( m_nodesReal.begin(), m_nodesReal.end(), FNodeEqual(_id) );
    if( iter != m_nodesReal.end() ){
        PNodeWorkerServiceReal node = ( * iter );

        const bool rt = node->configure( _cfg );

        // watch for configs of other nodes
    }
}

void Context::configureNode( const common_types::TNodeId & _id, const common_types::SConfigDump & _cfg ){

    // NOTE: main purpose of configuration is set worker to READY state

    // ...



}

bool Context::startNode( const common_types::TNodeId & _id ){

    // check node
    PNodeMirror node;
    auto iter = m_nodesById.find( _id );
    if( iter != m_nodesById.end() ){
        node = iter->second;
    }
    else{
        m_state.lastError = "node not found with id: " + _id;
        return false;
    }

    if( node->getBaseState().status != common_types::ENodeStatus::READY ){
        m_state.lastError = "node is not in READY state, id: " + _id;
        return false;
    }

    // realtime sync
    bool simulaNodeInRealtime = false;
    auto iterSimulaNode = std::find_if( m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(node->getBaseState().id) );
    if( iterSimulaNode != m_nodesSimula.end() ){
        const PNodeWorkerServiceSimula & nodeWorker = ( * iterSimulaNode );
        simulaNodeInRealtime = nodeWorker->getState().realtime;
    }

    // TODO: existence of real node doesn't mean than it in READY state
    if( (! m_nodesReal.empty() || simulaNodeInRealtime) && ! m_realTimeSynchronizer.getState().inSync ){
        m_realTimeSynchronizer.run();
    }

    // if there is only one simulation node - don't use RTI at all
    if( (1 == m_nodesSimula.size() && ! simulaNodeInRealtime) && m_nodesReal.empty() ){
        node->useRTI( false );
    }
    else{
        node->useRTI( true );
    }

    node->start();
    return true;
}

void Context::pauseNode( const common_types::TNodeId & _id ){

    auto iter = m_nodesById.find( _id );
    if( iter != m_nodesById.end() ){
        PNodeMirror node = iter->second;
        node->pause();
    }
}

void Context::stopNode( const common_types::TNodeId & _id ){

    // stop node
    auto iter = m_nodesById.find( _id );
    if( iter != m_nodesById.end() ){
        PNodeMirror node = iter->second;
        node->stop();
    }

    // interrupt realtime sync
    bool allNodeStopped = true;
    for( const PNodeMirror & node : m_nodes ){
        if( node->getBaseState().status != common_types::ENodeStatus::READY ){
            allNodeStopped = false;
        }
    }

    if( allNodeStopped && m_realTimeSynchronizer.getState().inSync ){
        m_realTimeSynchronizer.interrupt();
    }
}

void Context::setLiveProcessing( bool _live ){

    for( PNodeMirror & node : m_nodes ){
        node->switchLivePlaying( _live );
    }
}

void Context::includeUser( common_types::TUserId _userId ){
    m_users.insert( _userId );
}

void Context::excludeUser( common_types::TUserId _userId ){
    m_users.erase( _userId );
}

bool Context::isHasActiveNodes() const {

    for( const PNodeMirror & node : m_nodes ){
        if( common_types::ENodeStatus::ACTIVE == node->getBaseState().status ){
            return true;
        }
    }

    return false;
}

bool Context::isHasUsers() const {
    return ( ! m_users.empty() );
}

bool Context::isHasUser( common_types::TUserId _userId ) const {
    return ( m_users.find(_userId) != m_users.end() );
}

void Context::makeNodeStatic( const common_types::TNodeId & _id, bool _static ){

    // NOTE: inside system there are only two nodes - simula/dump(static) or real/dump(static). But for user it is the same node





}

void Context::callbackNodeSimulation( PNodeWorkerServiceSimula _node, bool _online ){

    // limbo context always without simula
    if( _node->getBaseState().ctxId != m_state.settings.ctxId || m_state.settings.limboContext ){
        return;
    }

    if( _online ){
        assert( std::find_if(m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_node->getBaseState().id)) == m_nodesSimula.end() );
        m_nodesSimula.push_back( _node );
        assert( std::find_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) == m_nodes.end() );
        m_nodes.push_back( _node );
        m_nodesById.insert( {_node->getBaseState().id, _node} );
    }
    else{
        m_nodesById.erase( _node->getBaseState().id );
        m_nodes.erase( std::remove_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) );
        assert( std::find_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) == m_nodes.end() );
        m_nodesSimula.erase( std::remove_if(m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_node->getBaseState().id)) );
        assert( std::find_if(m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_node->getBaseState().id)) == m_nodesSimula.end() );
    }
}

void Context::callbackNodeReal( PNodeWorkerServiceReal _node, bool _online ){

    // NOTE: all contexts see all real nodes

    if( _online ){
        assert( std::find_if(m_nodesReal.begin(), m_nodesReal.end(), FNodeEqual(_node->getBaseState().id)) == m_nodesReal.end() );
        m_nodesReal.push_back( _node );
        assert( std::find_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) == m_nodes.end() );
        m_nodes.push_back( _node );
        m_nodesById.insert( {_node->getBaseState().id, _node} );
    }
    else{
        m_nodesById.erase( _node->getBaseState().id );
        m_nodes.erase( std::remove_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) );
        assert( std::find_if(m_nodes.begin(), m_nodes.end(), FNodeEqual(_node->getBaseState().id)) == m_nodes.end() );
        m_nodesReal.erase( std::remove_if(m_nodesReal.begin(), m_nodesReal.end(), FNodeEqual(_node->getBaseState().id)) );
        assert( std::find_if(m_nodesReal.begin(), m_nodesReal.end(), FNodeEqual(_node->getBaseState().id)) == m_nodesReal.end() );
    }
}












