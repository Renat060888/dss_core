
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

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceSimula::SConfigSimulation & _cfg ){

    // ( for real time check, processing interval check, RTI sync launch / stop, etc )

    auto iter = std::find_if( m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_id) );
    if( iter != m_nodesSimula.end() ){
        PNodeWorkerServiceSimula node = ( * iter );

        const bool rt = node->configure( _cfg );

        // watch for configs of other nodes
    }
}

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceReal::SConfigReal & _cfg ){

    auto iter = std::find_if( m_nodesReal.begin(), m_nodesReal.end(), FNodeEqual(_id) );
    if( iter != m_nodesReal.end() ){
        PNodeWorkerServiceReal node = ( * iter );

        const bool rt = node->configure( _cfg );

        // watch for configs of other nodes
    }
}

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceDump::SConfigDump & _cfg ){

    // TODO: do



}

void Context::startNode( const common_types::TNodeId & _id ){

    // realtime sync
    if( ! m_nodesReal.empty() && ! m_realTimeSynchronizer.getState().inSync ){
        m_realTimeSynchronizer.run();
    }

    // if only one simulation node - don't use RTI at all
    if( 1 == m_nodesSimula.size() ){
//    node->useRTI( false );
    }

    // start node
    auto iter = m_nodesById.find( _id );
    PNodeMirror node = iter->second;
    node->start();
}

void Context::pauseNode( const common_types::TNodeId & _id ){

    auto iter = m_nodesById.find( _id );
    PNodeMirror node = iter->second;
    node->pause();
}

void Context::stopNode( const common_types::TNodeId & _id ){

    // stop node
    auto iter = m_nodesById.find( _id );
    PNodeMirror node = iter->second;
    node->reset();

    // interrupt realtime sync
    if( ! m_nodesReal.empty() && m_realTimeSynchronizer.getState().inSync ){
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
        if( node->getBaseState().busy ){
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

    // NOTE: "Static" -> inside system there are two nodes - simula/dump or real/dump. But for user it is the same node


}

void Context::callbackNodeSimulation( PNodeWorkerServiceSimula _node, bool _online ){

    // limbo context always without simula
    if( _node->getBaseState().ctxId != m_state.settings.ctxId || m_state.settings.limboContext ){
        return;
    }

    if( _online ){
        assert( std::find_if(m_nodesSimula.begin(), m_nodesSimula.end(), FNodeEqual(_node->getBaseState().id)) == m_nodesSimula.end() );
        m_nodesSimula.push_back( _node );
        m_nodesById.insert( {_node->getBaseState().id, _node} );
    }
    else{
        m_nodesById.erase( _node->getBaseState().id );
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












