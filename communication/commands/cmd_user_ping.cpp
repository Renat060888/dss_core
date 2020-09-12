
#include <boost/property_tree/json_parser.hpp>
#include <dss_common/common/common_utils.h>

#include "datasource/source_manager_facade.h"
#include "cmd_user_ping.h"

using namespace std;

static boost::property_tree::ptree serializeContextState( const PContext & _ctx ){

    // nodes
    boost::property_tree::ptree simulaNodeArray;
    for( const PNodeWorkerServiceSimula & node : _ctx->getNodesSimula() ){
        boost::property_tree::ptree simulaNodeElem;

        simulaNodeElem.put( "id", node->getState().id );
        simulaNodeElem.put( "status", common_utils::convertNodeStatusToStr(node->getState().status) );

        simulaNodeArray.push_back( boost::property_tree::ptree::value_type("", simulaNodeElem) );
    }

    boost::property_tree::ptree realNodeArray;
    for( const PNodeWorkerServiceReal & node : _ctx->getNodesReal() ){
        boost::property_tree::ptree realNodeElem;

        realNodeElem.put( "id", node->getState().id );
        realNodeElem.put( "status", common_utils::convertNodeStatusToStr(node->getState().status) );

        realNodeArray.push_back( boost::property_tree::ptree::value_type("", realNodeElem) );
    }

    // general
    boost::property_tree::ptree serializedContext;
    serializedContext.add_child( "simula_nodes", simulaNodeArray );
    serializedContext.add_child( "real_nodes", realNodeArray );
    serializedContext.add( "ctx_id", _ctx->getState().settings.ctxId );
    serializedContext.add( "processing_interval_millisec", _ctx->getState().processingIntervalMillisec );

    return serializedContext;
}

CommandUserPing::CommandUserPing( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandUserPing::exec(){

    SourceManagerFacade * sourceManager = ((common_types::SIncomingCommandServices *)m_services)->sourceManager;

    PContext context = sourceManager->getContext( m_userId );
    if( context ){
        boost::property_tree::ptree contextState = serializeContextState( context );

        boost::property_tree::ptree response;
        response.add_child( "ctx_state", contextState );
        response.add( "error_msg", "no_errors" );
        response.add( "result", true );

        // send response
        stringstream ss;
        boost::property_tree::json_parser::write_json( ss, response );

        sendResponse( ss.str() );
        return true;
    }
    else{
        boost::property_tree::ptree response;
        response.add( "error_msg", sourceManager->getState().lastError );
        response.add( "result", false );

        // send response
        stringstream ss;
        boost::property_tree::json_parser::write_json( ss, response );

        sendResponse( ss.str() );
        return false;
    }
}









