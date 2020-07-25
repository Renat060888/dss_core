
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include "source_manager_facade.h"

using namespace std;

// IMPROVES:

// hide network operations
// hide protocol type
// part of functional moved to microservice-common
// remove recorder

// disable user authorization
// disable task distribution to nodes
// RTI is not always needed

static constexpr const char * PRINT_HEADER = "SrcManager:";

// > functors
struct FContextEqual {
    FContextEqual( common_types::TContextId _ctxId )
        : m_ctxId(_ctxId)
    {}

    bool operator()( PContext _ctx ){
        return ( _ctx->getState().settings.ctxId == m_ctxId );
    }

    common_types::TContextId m_ctxId;
};
// < functors

SourceManagerFacade::SourceManagerFacade()
    : m_shutdownCalled(false)
    , m_threadMaintenance(nullptr)
{

}

SourceManagerFacade::~SourceManagerFacade(){

    shutdown();
}

void SourceManagerFacade::callbackUserOnline( const common_types::TUserId & _id, bool _online ){

    SCallbackUserOnlineArgs args;
    args.id = _id;
    args.online = _online;

    m_muUserOnlineCallbacks.lock();
    m_userOnlineCallbacks.push( args );
    m_muUserOnlineCallbacks.unlock();
}

bool SourceManagerFacade::init( const SInitSettings & _settings ){

    m_state.settings = _settings;

    // 'limbo' context
    Context::SInitSettings settingsCtx;
    settingsCtx.limboContext = true;

    m_limboContext = std::make_shared<Context>();
    if( ! m_limboContext->init(settingsCtx) ){
        m_state.lastError = m_limboContext->getState().lastError;
        return false;
    }

    m_nodeDispatcherSimula.addObserver( m_limboContext.get() );
    m_nodeDispatcherReal.addObserver( m_limboContext.get() );

    // users
    m_userDispatcher.addObserver( this );

    // maintenance thread
    m_threadMaintenance = new std::thread( & SourceManagerFacade::threadMaintenance, this );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void SourceManagerFacade::shutdown(){

    if( m_shutdownCalled ){
        return;
    }

    VS_LOG_INFO << PRINT_HEADER << " begin shutdown" << endl;

    m_shutdownCalled = true;
    common_utils::threadShutdown( m_threadMaintenance );






    VS_LOG_INFO << PRINT_HEADER << " shutdown success" << endl;
}

void SourceManagerFacade::threadMaintenance(){

    VS_LOG_INFO << PRINT_HEADER << " start maintenance THREAD" << endl;

    while( ! m_shutdownCalled ){
        m_nodeDispatcherReal.runSystemClock();
        m_nodeDispatcherSimula.runSystemClock();
        m_nodeDispatcherDump.runSystemClock();

        m_userDispatcher.runSystemClock();

        processUserOnlineCallbacks();
    }

    VS_LOG_INFO << PRINT_HEADER << " maintenance THREAD stopped" << endl;
}

void SourceManagerFacade::processUserOnlineCallbacks(){

    m_muUserOnlineCallbacks.lock();
    while( ! m_userOnlineCallbacks.empty() ){
        const SCallbackUserOnlineArgs callbackArgs = m_userOnlineCallbacks.front();
        m_userOnlineCallbacks.pop();

        // remove from context
        if( ! callbackArgs.online ){
            auto iterCtx = m_contextsByUserId.find( callbackArgs.id );
            if( iterCtx != m_contextsByUserId.end() ){
                PContext ctx = iterCtx->second;
                ctx->excludeUser( callbackArgs.id );
            }
        }
        // besides normal context opening:
        // initially the user is considered as 'without contexted'
        // dss client must notify about context, or restore from WAL
        else{            
            m_limboContext->includeUser( callbackArgs.id );
        }
    }
    m_muUserOnlineCallbacks.unlock();
}

PContext SourceManagerFacade::getContext( const common_types::TUserId & _userId ){

    // check user
    if( ! m_userDispatcher.isRegistered(_userId) ){
        m_state.lastError = m_userDispatcher.getLastError();
        return nullptr;
    }

    // if context not yet opened
    if( m_limboContext->isHasUser(_userId) ){
        return m_limboContext;
    }

    // check context
    auto iterCtx = m_contextsByUserId.find( _userId );
    if( iterCtx == m_contextsByUserId.end() ){
        m_state.lastError = PRINT_HEADER + string(" context not found");
        return nullptr;
    }

    return iterCtx->second;
}

bool SourceManagerFacade::openContext( common_types::TUserId _userId, common_types::TContextId _ctxId ){

    // check user
    if( ! m_userDispatcher.isRegistered(_userId) ){
        m_state.lastError = m_userDispatcher.getLastError();
        return false;
    }

    // check context exist
    PContext context;
    auto iter = m_contextsById.find( _ctxId );
    if( iter != m_contextsById.end() ){
        context = iter->second;
    }

    // create if not
    if( ! context ){
        Context::SInitSettings settingsCtx;
        settingsCtx.ctxId = _ctxId;

        context = std::make_shared<Context>();
        if( ! context->init(settingsCtx) ){
            m_state.lastError = context->getState().lastError;
            return false;
        }

        m_nodeDispatcherSimula.addObserver( context.get() );
        m_nodeDispatcherReal.addObserver( context.get() );

        // request nodes for this context
        DispatcherNodeSimulation::SNodeFilter nodesRequest;
        nodesRequest.contextId = _ctxId;
        const bool rt = m_nodeDispatcherSimula.requestNode( nodesRequest );
        if( ! rt ){
            m_state.lastError = m_nodeDispatcherSimula.getState().lastError;
            return false;;
        }

        // collect
        m_contexts.push_back( context );
        m_contextsById.insert( {context->getState().settings.ctxId, context} );
    }

    // move user from limbo to context
    context->includeUser( _userId );
    m_contextsByUserId.insert( {_userId, context} );
    m_limboContext->excludeUser( _userId );

    return true;
}

void SourceManagerFacade::closeContext( common_types::TUserId _userId ){

    // check user
    if( ! m_userDispatcher.isRegistered(_userId) ){
        m_state.lastError = m_userDispatcher.getState().lastError;
        return;
    }

    // check context
    auto iterCtx = m_contextsByUserId.find( _userId );
    if( iterCtx != m_contextsByUserId.end() ){
        m_state.lastError = PRINT_HEADER + string(" context to close not found for user ") + _userId;
        return;
    }
    PContext context = iterCtx->second;
    const common_types::TContextId ctxId = context->getState().settings.ctxId;

    // move user from context to limbo
    context->excludeUser( _userId );
    m_limboContext->includeUser( _userId );

    // don't touch active nodes & users
    if( context->isHasActiveNodes() || context->isHasUsers() ){
        return;
    }

    // release nodes related to context
    DispatcherNodeSimulation::SNodeFilter nodesRequest;
    nodesRequest.contextId = ctxId;
    m_nodeDispatcherSimula.releaseNode( nodesRequest );
    m_nodeDispatcherSimula.removeObserver( context.get() );
    m_nodeDispatcherReal.removeObserver( context.get() );

    // destroy context
    m_contextsById.erase( ctxId );
    m_contextsByUserId.erase( _userId );
    m_contexts.erase( std::remove_if(m_contexts.begin(), m_contexts.end(), FContextEqual(ctxId)) );
}

UserDispatcher * SourceManagerFacade::getDispatcherUser(){
    return & m_userDispatcher;
}

DispatcherNodeSimulation * SourceManagerFacade::getDispatcherNodeSimulation(){
    return & m_nodeDispatcherSimula;
}

DispatcherNodeReal * SourceManagerFacade::getDispatcherNodeReal(){
    return & m_nodeDispatcherReal;
}














