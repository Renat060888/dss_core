
#include "source_manager_facade.h"

using namespace std;

SourceManagerFacade::SourceManagerFacade()
    : m_threadMaintenance(nullptr)
    , m_shutdownCalled(false)
{

}

SourceManagerFacade::~SourceManagerFacade()
{

}

void SourceManagerFacade::callbackUserOnline( const common_types::TUserId & _id, bool _online ){

}

void SourceManagerFacade::callbackNodeOnline( const common_types::TNodeId & _id, bool _online ){

    if( _online ){
        PNodeMirror newNode;
        if( newNode = m_nodeDispatcherSimula.getNode(_id) ){
            // add to particular context
        }
        else if( newNode = m_nodeDispatcherReal.getNode(_id) ){
            // add to all contexts
        }
        else if( newNode = m_nodeDispatcherDump.getNode(_id) ){
            // add to particular context
        }
        else{
            assert( false && "" );
        }
    }
    else{

    }
}

bool SourceManagerFacade::init( const SInitSettings & _settings ){

    m_state.settings = _settings;

    // create 'limbo'-context
    m_limboContext = std::make_shared<Context>();
    Context::SInitSettings settings;
    settings.limboContext = true;

//    m_nodeDispatcherReal.addObserver( m_limboContext.get() );

    // users
    m_userDispatcher.addObserver( this );

    // nodes

    m_nodeDispatcherSimula.addObserver( this );

    m_nodeDispatcherReal.addObserver( this );



    return true;
}

void SourceManagerFacade::shutdown(){

}

void SourceManagerFacade::threadMaintenance(){

    while( ! m_shutdownCalled ){

        m_nodeDispatcherReal.runSystemClock();
        m_nodeDispatcherSimula.runSystemClock();
        m_nodeDispatcherDump.runSystemClock();

        m_userDispatcher.runSystemClock();
    }
}

void SourceManagerFacade::addRealNodesToContext( PEditableContext _ctx ){

    m_nodeDispatcherReal.addObserver( (INodeDispatcherObserver *)_ctx.get() );

    for( PNodeMirror & nodeMirror : m_nodeDispatcherReal.getNodes() ){
//        _ctx->addRealNodeMirror( nodeMirror );
    }
}

void SourceManagerFacade::addSimulaNodesToContext( PEditableContext _ctx ){

    // (step 1 - subscribe for event) create simula & dump nodes for this context
    m_nodeDispatcherSimula.addObserver( (INodeDispatcherObserver *)_ctx.get() );

    // (step 2 - initiate event) create simula & dump nodes for this context
    DispatcherNodeSimulation::SNodeRequestForm nodesRequestForm1;
    nodesRequestForm1.contextId = 0;
    nodesRequestForm1.missionId = 0;

    const bool rt = m_nodeDispatcherSimula.requestNode( nodesRequestForm1 );
    if( ! rt ){
        m_nodeDispatcherReal.removeObserver( (INodeDispatcherObserver *)_ctx.get() );
        m_nodeDispatcherSimula.removeObserver( (INodeDispatcherObserver *)_ctx.get() );
        m_state.lastError = m_nodeDispatcherSimula.getState().m_lastError;
        return;
    }
}

PContext SourceManagerFacade::getContext( common_types::TUserId _userId, common_types::TContextId _ctxId ){

    // check user
    if( ! m_userDispatcher.isRegistered(_userId) ){
        m_state.lastError = m_userDispatcher.getLastError();
        return nullptr;
    }

    // check context exist
    auto iter = m_contextsById.find( _ctxId );
    if( iter != m_contextsById.end() ){
        return iter->second;
    }

    // create if not
    PContext context = std::make_shared<Context>();
    Context::SInitSettings settings;
    settings.ctxId = _ctxId;

    if( ! context->init(settings) ){
        m_state.lastError = context->getState().lastError;
        return nullptr;
    }

    // add nodes
    addRealNodesToContext( context );
    addSimulaNodesToContext( context );

    // collect
    m_contexts.push_back( context );
    m_contextsById.insert( {context->getState().settings.ctxId, context} );

    return context;
}

void SourceManagerFacade::releaseContext( common_types::TUserId _userId, common_types::TContextId _ctxId ){

    if( ! m_userDispatcher.isRegistered(_userId) ){
        // TODO: set error
        return;
    }

    // 1 destroy only if all nodes were stopped ( context activity is false ) while user closing it
    // 2 or if all nodes were stopped also and no users have opened this context



}
















