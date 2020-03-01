
#include <boost/property_tree/json_parser.hpp>
#include <microservice_common/system/logger.h>
#include <microservice_common/system/daemonizator.h>
#include <microservice_common/communication/shell.h>
#include <dss_common/system/args_parser.h>
#include <dss_common/system/config_reader.h>
#include <dss_common/system/path_locator.h>

#include "distributed_simulation_system.h"
#include "communication/unified_command_convertor_dss.h"

using namespace std;

static bool initSingletons( int _argc, char ** _argv, char ** _env ){

    // cmd line arguments
    ArgsParser::SInitSettings settings;
    settings.argc = _argc;
    settings.argv = _argv;
    settings.printConfigExample = std::bind( & AConfigReader::printToStdoutConfigExample, & CONFIG_READER );
    settings.commandConvertor = & UNIFIED_COMMAND_CONVERTOR;
    if( ! ARGS_PARSER.init(settings) ){
        return false;
    }

    // configs
    ConfigReader::SIninSettings settings3;
    settings3.mainConfigPath = ARGS_PARSER.getVal(EDssArguments::MAIN_CONFIG_PATH_FROM_CONSOLE);
    settings3.commandConvertor = & UNIFIED_COMMAND_CONVERTOR;
    settings3.env = _env;
    settings3.projectName = "dss";
    if( ! CONFIG_READER.init(settings3) ){
        return false;
    }

    // logger
    logger_common::SInitSettings settings2;
    settings2.loggerName = "DssCore";
    settings2.unilogConfigPath = CONFIG_PARAMS.SYSTEM_UNILOG_CONFIG_PATH;

    if( CONFIG_PARAMS.SYSTEM_LOG_TO_STDOUT ){
        settings2.logEndpoints = (logger_common::ELogEndpoints)( (int)settings2.logEndpoints | (int)logger_common::ELogEndpoints::Stdout );
    }
    if( CONFIG_PARAMS.SYSTEM_LOG_TO_FILE ){
        settings2.logEndpoints = (logger_common::ELogEndpoints)( (int)settings2.logEndpoints | (int)logger_common::ELogEndpoints::File );
        settings2.fileName = CONFIG_PARAMS.SYSTEM_LOGFILE_NAME;
        settings2.filePath = CONFIG_PARAMS.SYSTEM_REGULAR_LOGFILE_PATH;
        settings2.rotationSizeMb = CONFIG_PARAMS.SYSTEM_LOGFILE_ROTATION_SIZE_MB;
    }

    Logger::singleton().initGlobal( settings2 );

    return true;
}

static void parseResponse( const std::string & _msg ){

    // parse base part
    boost::property_tree::ptree parsedRepsonseJson;
    try{
        istringstream contentStream( _msg );
        boost::property_tree::json_parser::read_json( contentStream, parsedRepsonseJson );
    }
    catch( boost::property_tree::json_parser::json_parser_error & _ex ){
        VS_LOG_ERROR    << " parse failed of [" << _msg << "]" << endl
                        << "Reason: [" << _ex.what() << "]" << endl;
        return;
    }

    VS_LOG_INFO << "response from unix-socket-dss [" <<  _msg << "]" << endl;

    // TODO: print values from parsedRepsonseJson ?
}

static bool executeShellCommand(){

    if( ! ARGS_PARSER.getVal(EDssArguments::SHELL_COMMAND_START_SERVER).empty() ){

        // deamonize
        if( ! ARGS_PARSER.getVal(EDssArguments::AS_DAEMON).empty() ){
            if( ! Daemonizator::turnIntoDaemon() ){
                return false;
            }

            // reinit logger for daemon
            logger_common::SInitSettings settings2;
            settings2.loggerName = "DssCoreDaemon";
            settings2.unilogConfigPath = ConfigReader::singleton().get().SYSTEM_UNILOG_CONFIG_PATH;
            settings2.logEndpoints = logger_common::ELogEndpoints::File;
            settings2.fileName = CONFIG_PARAMS.SYSTEM_LOGFILE_NAME;
            settings2.filePath = CONFIG_PARAMS.SYSTEM_REGULAR_LOGFILE_PATH;
            settings2.rotationSizeMb = CONFIG_PARAMS.SYSTEM_LOGFILE_ROTATION_SIZE_MB;

            Logger::singleton().initGlobal( settings2 );

            VS_LOG_INFO << "============================ DSS CORE AS DAEMON ========================" << endl;
        }

        // launch
        {
            DistributedSimulationSystem::SInitSettings settings;
            DistributedSimulationSystem dss;
            if( dss.init(settings) ){
                dss.launch();
            }
            else{
                return false;
            }
        }
    }
    else if( ! ARGS_PARSER.getVal(EDssArguments::SHELL_COMMAND_TO_SERVER).empty() ){

        // reinit logger for client side
        logger_common::SInitSettings settings;
        settings.loggerName = "DssCoreShellClient";
        settings.unilogConfigPath = ConfigReader::singleton().get().SYSTEM_UNILOG_CONFIG_PATH;
        Logger::singleton().initGlobal( settings );

        // shell client
        Shell::SInitSettings settings2;
        settings2.shellMode = Shell::EShellMode::CLIENT;
        settings2.messageMode = Shell::EMessageMode::WITHOUT_SIZE;
        settings2.socketFileName = PATH_LOCATOR.getShellImitationDomainSocket();
        Shell shell( INetworkEntity::INVALID_CONN_ID );
        if( ! shell.init(settings2) ){
            return false;
        }

        // send message to server
        const string message =  ARGS_PARSER.getVal(EDssArguments::SHELL_COMMAND_TO_SERVER);
        const string response = shell.makeBlockedRequest( message );
        parseResponse( response );
    }

    return true;
}

int main( int argc, char ** argv, char ** env ){

    if( ! initSingletons(argc, argv, env) ){
        PRELOG_ERR << "============================ DSS CORE START FAILED (singletons area) ============================" << endl;
        return -1;
    }

#if 1
    VS_LOG_INFO << endl;
    VS_LOG_INFO << endl;
    if( ! executeShellCommand() ){
        VS_LOG_ERROR << "============================ DSS CORE START FAILED ============================" << endl;
        return -1;
    }
#else
    UnitTests tests;
    tests.run();
#endif

    VS_LOG_INFO << endl;
    VS_LOG_INFO << endl;
    VS_LOG_INFO << "============================ DSS CORE PROCESS RETURNS '0' ============================" << endl;
    return 0;
}


