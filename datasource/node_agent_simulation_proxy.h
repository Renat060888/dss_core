#ifndef NODE_AGENT_SIMULATION_H
#define NODE_AGENT_SIMULATION_H

#include <microservice_common/communication/network_interface.h>

class NodeAgentSimulationProxy
{
public:
    struct SInitSettings {
        PNetworkClient network;
    };

    NodeAgentSimulationProxy();




private:
    void forwardNodeWorkerRequest( void * _serializedRequest );


};
using PNodeAgentSimulationProxy = std::shared_ptr<NodeAgentSimulationProxy>;

#endif // NODE_AGENT_SIMULATION_H
