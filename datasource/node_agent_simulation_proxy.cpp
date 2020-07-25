
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include "node_agent_simulation_proxy.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "NodeAgentSimula:";


NodeAgentSimulationProxy::NodeAgentSimulationProxy()
{

}

NodeAgentSimulationProxy::~NodeAgentSimulationProxy(){

    VS_LOG_INFO << PRINT_HEADER << " DTOR id: " << m_state.nodeAgentId << endl;

}

bool NodeAgentSimulationProxy::init( const SInitSettings & _settings ){

    m_networkWithAgent = _settings.network;



    return true;
}

bool NodeAgentSimulationProxy::requestWorkers( common_types::TContextId _ctxId ){




    return true;
}

void NodeAgentSimulationProxy::releaseWorker( common_types::TNodeId _nodeId ){

}

void NodeAgentSimulationProxy::updateState( const common_types::SNodeAgentSimulateState & _state ){

    m_state = _state;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

void NodeAgentSimulationProxy::forwardNodeWorkerRequest( void * _serializedRequest ){

    const string serializedMsg = "";


    PEnvironmentRequest netRequest = m_networkWithAgent->getRequestInstance();
    netRequest->setOutcomingMessage( serializedMsg );


}









