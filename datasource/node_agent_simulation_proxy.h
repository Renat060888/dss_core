#ifndef NODE_AGENT_SIMULATION_H
#define NODE_AGENT_SIMULATION_H

#include <microservice_common/communication/network_interface.h>
#include <dss_common/common/common_types.h>


class NodeAgentSimulationProxy : public common_types::INodeAgentFacilityForWorker
{
public:
    struct SInitSettings {
        PNetworkClient network;
    };

    NodeAgentSimulationProxy();
    ~NodeAgentSimulationProxy();

    bool init( const SInitSettings & _settings );
    const common_types::SNodeAgentSimulateState & getState(){ return m_state; }

    bool requestWorkers( common_types::TContextId _ctxId );
    void releaseWorker( common_types::TNodeId _nodeId );

    void updateState( const common_types::SNodeAgentSimulateState & _state );


private:
    // from worker
    virtual void forwardNodeWorkerRequest( const std::string & _serializedRequest ) override;




    // data
    common_types::SNodeAgentSimulateState m_state;

    // service
    PNetworkClient m_networkWithAgent;
};
using PNodeAgentSimulationProxy = std::shared_ptr<NodeAgentSimulationProxy>;

#endif // NODE_AGENT_SIMULATION_H
