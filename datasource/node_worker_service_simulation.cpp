
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include "node_worker_service_simulation.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "NodeWorkerSimula:";

NodeWorkerServiceSimula::NodeWorkerServiceSimula( PNodeAgentFacilityForWorker _nodeAgent )
    : m_nodeAgentFacility(_nodeAgent)
{

}

NodeWorkerServiceSimula::~NodeWorkerServiceSimula(){

    VS_LOG_INFO << PRINT_HEADER << " DTOR id: " << m_state.id << endl;
}

bool NodeWorkerServiceSimula::configure( const SConfigSimulation & _cfg ){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );


    return true;
}

const SNodeWorkerSimulationState & NodeWorkerServiceSimula::getState(){
    return m_state;
}

const common_types::SNodeState & NodeWorkerServiceSimula::getBaseState(){
    return m_state;
}

void NodeWorkerServiceSimula::updateState( const common_types::SNodeWorkerSimulationState & _workerState ){

    m_state = _workerState;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

bool NodeWorkerServiceSimula::start(){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );
}

bool NodeWorkerServiceSimula::pause(){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );
}

bool NodeWorkerServiceSimula::reset(){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );
}

void NodeWorkerServiceSimula::switchLivePlaying( bool _live ){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );
}

void NodeWorkerServiceSimula::resetTime(){

    // create serialized request & "forward" to Agent

    m_nodeAgentFacility->forwardNodeWorkerRequest( nullptr );
}






