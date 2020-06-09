
#include "node_worker_service_simulation.h"

using namespace std;

NodeWorkerServiceSimula::NodeWorkerServiceSimula( NodeAgentSimulationProxy * _nodeAgent )
    : m_nodeAgent(_nodeAgent)
{

}

bool NodeWorkerServiceSimula::configure( SConfiguration * _conf ){

}

bool NodeWorkerServiceSimula::start(){

    // create serialized request & "forward" to Agent
}

bool NodeWorkerServiceSimula::pause(){

}

bool NodeWorkerServiceSimula::reset(){

}

void NodeWorkerServiceSimula::switchLivePlaying( bool _live ){

}

void NodeWorkerServiceSimula::resetTime(){

}
