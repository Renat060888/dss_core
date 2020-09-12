
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include <boost/property_tree/json_parser.hpp>
#include <microservice_common/system/logger.h>
#include <dss_common/communication/protocols/dss_node_agent_simulation.pb.h>
#include <dss_common/communication/protocols/dss_node_agent_real.pb.h>
#include <dss_common/common/common_utils.h>
#include <dss_common/common/common_vars.h>

#include "command_factory.h"
#include "commands/cmd_user_ping.h"
#include "commands/cmd_user_register.h"
#include "commands/cmd_context_open.h"
#include "commands/cmd_context_close.h"
#include "commands/cmd_node_agent_ping.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "CommandFactory:";

static common_types::ENodeStatus convertWorkerStatus( const ::google_protobuf_dss_worker_simula::EWorkerStatus & _status ){
    switch( _status ){
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::INITED : { return common_types::ENodeStatus::INITED; }
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::PREPARING : { return common_types::ENodeStatus::PREPARING; }
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::READY : { return common_types::ENodeStatus::READY; }
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::ACTIVE : { return common_types::ENodeStatus::ACTIVE; }
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::IDLE : { return common_types::ENodeStatus::IDLE; }
    case ::google_protobuf_dss_worker_simula::EWorkerStatus::CRASHED : { return common_types::ENodeStatus::CRASHED; }
    default : {

    }
    }
}

static common_types::ENodeStatus convertWorkerStatus( const ::google_protobuf_dss_worker_real::EWorkerStatus & _status ){
    switch( _status ){
    case ::google_protobuf_dss_worker_real::EWorkerStatus::INITED : { return common_types::ENodeStatus::INITED; }
    case ::google_protobuf_dss_worker_real::EWorkerStatus::PREPARING : { return common_types::ENodeStatus::PREPARING; }
    case ::google_protobuf_dss_worker_real::EWorkerStatus::READY : { return common_types::ENodeStatus::READY; }
    case ::google_protobuf_dss_worker_real::EWorkerStatus::ACTIVE : { return common_types::ENodeStatus::ACTIVE; }
    case ::google_protobuf_dss_worker_real::EWorkerStatus::IDLE : { return common_types::ENodeStatus::IDLE; }
    case ::google_protobuf_dss_worker_real::EWorkerStatus::CRASHED : { return common_types::ENodeStatus::CRASHED; }
    default : {

    }
    }
}

CommandFactory::CommandFactory( common_types::SIncomingCommandServices & _commandServices )
    : m_commandServices(_commandServices)
{

}

PCommand CommandFactory::createCommand( PEnvironmentRequest _request ){

    // empty request
    if( _request->getIncomingMessage().empty() ){
        sendFailToExternal( _request, "I don't see the command (-_-)" );
        return nullptr;
    }

    // try to parse by various protocols
    const string & msg = _request->getIncomingMessage();
    if( '{' == msg[ 0 ] && '}' == msg[ msg.size() - 1 ] ){
        return parseExternalRequest( _request );
    }
    else if( false ){
        return parseInternalRequest( _request );
    }
    else{
        return nullptr;
    }
}

PCommand CommandFactory::parseExternalRequest( const PEnvironmentRequest & _request ){

    boost::property_tree::ptree parsedJson;
    try{
        istringstream contentStream( _request->getIncomingMessage() );
        boost::property_tree::json_parser::read_json( contentStream, parsedJson );
    }
    catch( boost::property_tree::json_parser::json_parser_error & _ex ){
        VS_LOG_ERROR << PRINT_HEADER
                     << " parse failed of [" << _request->getIncomingMessage() << "]" << endl
                     << " reason: [" << _ex.what() << "]" << endl;
        return nullptr;
    }

    PCommand cmd;

    // -------------------------------------------------------------------------------------
    // service
    // -------------------------------------------------------------------------------------
    if( "service" == parsedJson.get<string>(common_vars::cmd::COMMAND_TYPE) ){
        if( "ping" == parsedJson.get<string>(common_vars::cmd::COMMAND_NAME) ){
            PCommandUserPing cmd1 = std::make_shared<CommandUserPing>( & m_commandServices );
            cmd1->m_userId = parsedJson.get<string>(common_vars::cmd::USER_ID);
            cmd = cmd1;
        }
        else if( "register" == parsedJson.get<string>(common_vars::cmd::COMMAND_NAME) ){
            PCommandUserRegister cmd1 = std::make_shared<CommandUserRegister>( & m_commandServices );
            cmd1->m_userIpStr = parsedJson.get<string>(common_vars::cmd::USER_IP);
            cmd1->m_userPid = parsedJson.get<common_types::TPid>(common_vars::cmd::USER_PID);
            cmd = cmd1;
        }
        else if( "context_open" == parsedJson.get<string>(common_vars::cmd::COMMAND_NAME) ){
            PCommandContextOpen cmd1 = std::make_shared<CommandContextOpen>( & m_commandServices );
            cmd1->m_userId = parsedJson.get<string>(common_vars::cmd::USER_ID);
            cmd1->m_contextName = parsedJson.get<string>(common_vars::cmd::CONTEXT_NAME);
            cmd = cmd1;
        }
        else if( "context_close" == parsedJson.get<string>(common_vars::cmd::COMMAND_NAME) ){
            PCommandContextClose cmd1 = std::make_shared<CommandContextClose>( & m_commandServices );
            cmd1->m_userId = parsedJson.get<string>(common_vars::cmd::USER_ID);
            cmd = cmd1;
        }
        else{
            VS_LOG_WARN << PRINT_HEADER << " unknown command name [" << parsedJson.get<string>(common_vars::cmd::COMMAND_NAME) << "]" << endl;
            sendFailToExternal( _request, "I don't know such command name of 'service' command type (-_-)" );
            return nullptr;
        }
    }
    else{
        VS_LOG_WARN << PRINT_HEADER << " unknown command type [" << parsedJson.get<string>(common_vars::cmd::COMMAND_TYPE) << "]" << endl;
        sendFailToExternal( _request, "I don't know such command type (-_-)" );
        return nullptr;
    }

    cmd->m_request = _request;
    return cmd;
}

PCommand CommandFactory::parseInternalRequest( const PEnvironmentRequest & _request ){

    static google_protobuf_dss_agent_real::MessageAgentRealToCoreRequest realAgent;
    static google_protobuf_dss_agent_simula::MessageAgentSimulaToCoreRequest simulaAgent;

    // from real agent(s)
    if( realAgent.ParseFromString(_request->getIncomingMessage()) ){
        PCommand cmd;

        if( realAgent.has_msg_ping() ){
            PCommandNodeAgentPing cmd1 = std::make_shared<CommandNodeAgentPing>( & m_commandServices );

            const ::google_protobuf_dss_agent_real::MessageRequestPing & msgPing = realAgent.msg_ping();

            const ::google_protobuf_dss_agent_real::MessageAgentPluginsInfo & pluginsInfo = msgPing.msg_plugins_info();
            const ::google_protobuf_dss_agent_real::MessageAgentSystemInfo & sysInfo = msgPing.msg_system_info();
            const ::google_protobuf_dss_agent_real::MessageAgentRunnedWorkers & runnedWorkers = msgPing.msg_runned_workers();

            const int ramTotalMb = sysInfo.ram_total_mb();

            for( int i = 0; i < pluginsInfo.real_plugins_info_size(); i++ ){
                const ::google_protobuf_dss_agent_real::MessageAgentRealPlugin & realPluginInfo = pluginsInfo.real_plugins_info( i );

                cmd1->m_stateFromRealAgent.physicModelNameAndCaps.insert( {realPluginInfo.real_plugin_name(), realPluginInfo.available_caps()} );
            }

            for( int i = 0; i < runnedWorkers.worker_states_size(); i++ ){
                const ::google_protobuf_dss_worker_real::MessageWorkerState & workerStateFrom = runnedWorkers.worker_states( i );

                common_types::SNodeWorkerRealState workerStateTo;
                workerStateTo.id = "";
                workerStateTo.agentId = workerStateFrom.agent_id();
                workerStateTo.ctxId = workerStateFrom.ctx_id();
                workerStateTo.availableCaps = workerStateFrom.available_caps();
                workerStateTo.realObjectName = workerStateFrom.real_object_name();
                workerStateTo.status = convertWorkerStatus( workerStateFrom.status() );
                cmd1->m_stateFromRealAgent.nodeWorkers.push_back( workerStateTo );
            }
        }
        else{
            VS_LOG_WARN << PRINT_HEADER << " unknown msg type [" << realAgent.DebugString() << "]" << endl;
            sendFailToExternal( _request, "I don't know such message type (-_-)" );
            return nullptr;
        }

        cmd->m_request = _request;
        return cmd;
    }
    // from simulation agent(s)
    else if( simulaAgent.ParseFromString(_request->getIncomingMessage()) ){
        PCommand cmd;

        if( realAgent.has_msg_ping() ){
            PCommandNodeAgentPing cmd1 = std::make_shared<CommandNodeAgentPing>( & m_commandServices );

            const ::google_protobuf_dss_agent_simula::MessageRequestPing & msgPing = simulaAgent.msg_ping();

            const ::google_protobuf_dss_agent_simula::MessageAgentPluginsInfo & pluginsInfo = msgPing.msg_plugins_info();
            const ::google_protobuf_dss_agent_simula::MessageAgentSystemInfo & sysInfo = msgPing.msg_system_info();
            const ::google_protobuf_dss_agent_simula::MessageAgentRunnedWorkers & runnedWorkers = msgPing.msg_runned_workers();

            const int ramTotalMb = sysInfo.ram_total_mb();

            for( int i = 0; i < pluginsInfo.physic_model_names_size(); i++ ){
                const string & phmNameFrom = pluginsInfo.physic_model_names( i );

                cmd1->m_stateFromSimulationAgent.physicModelNames.push_back( phmNameFrom );
            }

            for( int i = 0; i < runnedWorkers.worker_states_size(); i++ ){
                const ::google_protobuf_dss_worker_simula::MessageWorkerState & workerStateFrom = runnedWorkers.worker_states( i );

                common_types::SNodeWorkerSimulationState workerStateTo;
                workerStateTo.id = "";
                workerStateTo.agentId = workerStateFrom.agent_id();
                workerStateTo.ctxId = workerStateFrom.ctx_id();
                workerStateTo.realtime = workerStateFrom.in_realtime();
                workerStateTo.status = convertWorkerStatus( workerStateFrom.status() );
                cmd1->m_stateFromSimulationAgent.nodeWorkers.push_back( workerStateTo );
            }
        }
        else{
            VS_LOG_WARN << PRINT_HEADER << " unknown msg type [" << simulaAgent.DebugString() << "]" << endl;
            sendFailToExternal( _request, "I don't know such message type (-_-)" );
            return nullptr;
        }

        cmd->m_request = _request;
        return cmd;
    }
    else{
        VS_LOG_WARN << PRINT_HEADER << " unknown msg type [" << _request->getIncomingMessage() << "]" << endl;
        sendFailToExternal( _request, "I don't know such msg type (-_-)" );
        return nullptr;
    }
}

void CommandFactory::sendFailToExternal( PEnvironmentRequest _request, const string _msg ){

    Json::Value root;
    root["response"] = "fail";
    root["body"] = _msg;

    Json::FastWriter writer;

    _request->setOutcomingMessage( writer.write( root ) );
}














