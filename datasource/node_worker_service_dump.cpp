
#include "node_worker_service_dump.h"

using namespace std;
using namespace common_types;

NodeWorkerServiceDump::NodeWorkerServiceDump()
{

}

bool NodeWorkerServiceDump::configure( const SConfigDump & _cfg ){




    return true;
}

const NodeWorkerServiceDump::SStateDump & NodeWorkerServiceDump::getState(){
    return m_state;
}

const SNodeState & NodeWorkerServiceDump::getBaseState(){
    return m_state;
}

bool NodeWorkerServiceDump::start(){

}

bool NodeWorkerServiceDump::pause(){

}

bool NodeWorkerServiceDump::reset(){

}

void NodeWorkerServiceDump::switchLivePlaying( bool _live ){

}

void NodeWorkerServiceDump::resetTime(){

}
