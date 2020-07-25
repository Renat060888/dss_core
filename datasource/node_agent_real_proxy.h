#ifndef NODE_AGENT_REAL_H
#define NODE_AGENT_REAL_H

#include <microservice_common/communication/network_interface.h>

class NodeAgentRealProxy
{
    friend class DispatcherNodeReal;
public:
    struct SInitSettings {
        PNetworkClient network;
    };

    NodeAgentRealProxy();


private:
    void forwardNodeWorkerRequest( void * _serializedRequest );


};
using PNodeAgentRealProxy = std::shared_ptr<NodeAgentRealProxy>;

#endif // NODE_AGENT_REAL_H
