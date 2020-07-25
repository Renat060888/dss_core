
#include "datasource/source_manager_facade.h"
#include "cmd_node_agent_ping.h"

using namespace std;

CommandNodeAgentPing::CommandNodeAgentPing( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandNodeAgentPing::exec(){

    assert( ! (! m_stateFromSimulationAgent.nodeAgentId.empty() && ! m_stateFromRealAgent.nodeAgentId.empty()) &&
            "state from multiple agents at the same time" );

    SourceManagerFacade * srcManager = ((common_types::SIncomingCommandServices *)m_services)->sourceManager;

    if( ! m_stateFromSimulationAgent.nodeAgentId.empty() ){
        srcManager->getDispatcherNodeSimulation()->updateNodeAgentState( m_stateFromSimulationAgent );
    }
    else if( ! m_stateFromRealAgent.nodeAgentId.empty() ){
        srcManager->getDispatcherNodeReal()->updateNodeAgentState( m_stateFromRealAgent );
    }





    return true;
}
