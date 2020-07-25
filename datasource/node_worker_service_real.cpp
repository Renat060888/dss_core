
#include <dss_common/common/common_utils.h>

#include "node_worker_service_real.h"

using namespace std;
using namespace common_types;

NodeWorkerServiceReal::NodeWorkerServiceReal()
{

}

bool NodeWorkerServiceReal::configure( const SConfigReal & _cfg ){



    return true;
}

const SNodeWorkerRealState & NodeWorkerServiceReal::getState(){
    return m_state;
}

const SNodeState & NodeWorkerServiceReal::getBaseState(){
    return m_state;
}

void NodeWorkerServiceReal::updateState( const common_types::SNodeWorkerRealState & _workerState ){

    m_state = _workerState;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

bool NodeWorkerServiceReal::start(){

}

bool NodeWorkerServiceReal::pause(){

}

bool NodeWorkerServiceReal::reset(){

}

void NodeWorkerServiceReal::switchLivePlaying( bool _live ){

}

void NodeWorkerServiceReal::resetTime(){

}
