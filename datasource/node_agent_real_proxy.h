#ifndef NODE_AGENT_REAL_H
#define NODE_AGENT_REAL_H

#include <microservice_common/communication/network_interface.h>
#include <dss_common/common/common_types.h>

class NodeAgentRealProxy : public common_types::INodeAgentFacilityForWorker
{
    friend class DispatcherNodeReal;
public:
    struct SInitSettings {
        PNetworkClient network;
    };

    NodeAgentRealProxy();


private:
    virtual void forwardNodeWorkerRequest( const std::string & _serializedRequest ) override;


    // data
    common_types::SNodeAgentRealState m_state;

    // service
    PNetworkClient m_networkWithAgent;
};
using PNodeAgentRealProxy = std::shared_ptr<NodeAgentRealProxy>;

#endif // NODE_AGENT_REAL_H
