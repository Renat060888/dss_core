
#include <microservice_common/system/logger.h>

#include "context.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "Context:";

Context::Context()
    : m_trRealTimeSync(nullptr)
{

}

Context::~Context(){

    VS_LOG_INFO << PRINT_HEADER << " begin shutdown" << endl;



    VS_LOG_INFO << PRINT_HEADER << " shutdown success" << endl;
}

bool Context::init( const SInitSettings & _settings ){




    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
}

void Context::threadRealTimeSync(){

    VS_LOG_INFO << PRINT_HEADER << " start real time sync THREAD" << endl;




    VS_LOG_INFO << PRINT_HEADER << " stop real time sync THREAD" << endl;
}

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceSimula::SConfigSimulation & _conf ){

}

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceReal::SConfigReal & _conf ){

}

void Context::configureNode( const common_types::TNodeId & _id, const NodeWorkerServiceDump::SConfigDump & _conf ){

}

void Context::startNode( const common_types::TNodeId & _id ){

    // if only one simulation node - don't use RTI at all
}

void Context::pauseNode( const common_types::TNodeId & _id ){

}

void Context::stopNode( const common_types::TNodeId & _id ){

}

void Context::setLiveProcessing( bool _live ){

}

void Context::makeNodeStatic( const common_types::TNodeId & _id, bool _static ){

    // NOTE: "Static" -> inside system there are two nodes - simula/dump or real/dump. But for user it is the same node


}

bool Context::addNodeWorkerServiceSimula( PNodeWorkerServiceSimula _node ){


    // TODO: check for ctx & mission correctness


    return true;
}

bool Context::addNodeWorkerServiceReal( PNodeWorkerServiceReal _node ){


    // TODO: check for ctx & mission correctness



    return true;
}

void Context::removeNodeWorkerService( const common_types::TNodeId & _id ){

}




