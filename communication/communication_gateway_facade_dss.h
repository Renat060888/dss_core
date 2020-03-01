#ifndef COMMUNICATION_GAREWAY_FACADE_DSS_H
#define COMMUNICATION_GAREWAY_FACADE_DSS_H

#include <microservice_common/communication/communication_gateway.h>
#include <dss_common/common/common_types.h>

class CommunicationGatewayFacadeDSS : public CommunicationGatewayFacade
                                    , public common_types::IServiceInternalCommunication
                                    , public common_types::IServiceExternalCommunication
{
public:
    struct SInitSettings : CommunicationGatewayFacade::SInitSettings {
        common_types::SIncomingCommandServices services;
    };

    CommunicationGatewayFacadeDSS();
    ~CommunicationGatewayFacadeDSS();

    bool init( const SInitSettings & _settings );

    common_types::IServiceExternalCommunication * serviceForExternalCommunication();
    common_types::IServiceInternalCommunication * serviceForInternalCommunication();


private:
    // internal
    virtual PNetworkClient getNodeAgentCommunicator( const std::string & _uniqueId ) override;
    virtual PNetworkClient getNodeWorkerCommunicator( const std::string & _uniqueId ) override;
    virtual PNetworkClient getPlayerAgentCommunicator() override;
    virtual PNetworkClient getPlayerWorkerCommunicator( const std::string & _uniqueId ) override;

    // external
    virtual PNetworkClient getUserCommunicator( const std::string & _uniqueId ) override;


    // data
    SInitSettings m_settings;
    std::string m_lastError;


    // service
    std::map<std::string, PNetworkClient> m_nodeAgentCommunicatorsById;
    std::map<std::string, PNetworkClient> m_nodeWorkerCommunicatorsById;
    // TODO: playerAgent ?
    std::map<std::string, PNetworkClient> m_playerWorkerCommunicatorsById;

    std::map<std::string, PNetworkClient> m_userCommunicatorsById;

};

#endif // COMMUNICATION_GAREWAY_FACADE_DSS_H
