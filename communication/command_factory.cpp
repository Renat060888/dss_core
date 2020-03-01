
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

#include <boost/property_tree/json_parser.hpp>
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_vars.h>

#include "command_factory.h"
#include "commands/cmd_user_ping.h"
#include "commands/cmd_user_register.h"
#include "commands/cmd_context_open.h"
#include "commands/cmd_context_close.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "CommandFactory:";

CommandFactory::CommandFactory( common_types::SIncomingCommandServices & _commandServices )
    : m_commandServices(_commandServices)
{

}

PCommand CommandFactory::createCommand( PEnvironmentRequest _request ){

    {
        VS_LOG_DBG << PRINT_HEADER << " incoming msg [" << _request->getIncomingMessage() << "]" << endl;

        if( _request->getIncomingMessage().empty() ){
            sendFailToExternal( _request, "I don't see the command (-_-)" );
            return nullptr;
        }

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
        // -------------------------------------------------------------------------------------
        // source
        // -------------------------------------------------------------------------------------
        else if( "source" == parsedJson.get<string>(common_vars::cmd::COMMAND_TYPE) ){



        }
        else{
            VS_LOG_WARN << PRINT_HEADER << " unknown command type [" << parsedJson.get<string>(common_vars::cmd::COMMAND_TYPE) << "]" << endl;
            sendFailToExternal( _request, "I don't know such command type (-_-)" );
            return nullptr;
        }

        cmd->m_request = _request;
        return cmd;
    }

    VS_LOG_DBG << PRINT_HEADER << " incoming msg [" << _request->getIncomingMessage() << "]" << endl;

    PCommand cmd;

    if( _request->getIncomingMessage().empty() ){
        sendFailToExternal( _request, "I don't see the command (-_-)" );
        return nullptr;
    }

//    cmd = messageFromInternalModule( _request );
    if( cmd ){
        cmd->m_request = _request;
        return cmd;
    }

    Json::Reader reader;
    Json::Value parsedRecord;
    if( ! reader.parse( _request->getIncomingMessage().c_str(), parsedRecord, false ) ){
       VS_LOG_ERROR << "CommandFactory: parse failed of [1] Reason: [2] ["
                 << _request->getIncomingMessage() << "] ["
                 << reader.getFormattedErrorMessages()  << "]"
                 << endl;

        sendFailToExternal( _request, "I can't parse the command (-_-)" );
        return nullptr;
    }

    // -------------------------------------------------------------------------------------
    // source commands
    // -------------------------------------------------------------------------------------
    if( "source" == parsedRecord[ common_vars::cmd::COMMAND_TYPE ].asString() ){



    }
    else{
        VS_LOG_WARN << "unknown command type [" << parsedRecord[ common_vars::cmd::COMMAND_TYPE ].asString() << "]" << endl;
        sendFailToExternal( _request, "I don't know such command type (-_-)" );
        return nullptr;
    }

    cmd->m_request = _request;
    return cmd;
}

void CommandFactory::sendFailToExternal( PEnvironmentRequest _request, const string _msg ){

    Json::Value root;

    root["response"] = "fail";
    root["body"] = _msg;

    Json::FastWriter writer;

    _request->setOutcomingMessage( writer.write( root ) );
}














