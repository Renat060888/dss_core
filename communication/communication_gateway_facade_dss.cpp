
#include <microservice_common/common/ms_common_utils.h>
#include <microservice_common/communication/amqp_client_c.h>

#include "communication_gateway_facade_dss.h"
#include "command_factory.h"

using namespace std;

CommunicationGatewayFacadeDSS::CommunicationGatewayFacadeDSS()
    : CommunicationGatewayFacade()
{

}

CommunicationGatewayFacadeDSS::~CommunicationGatewayFacadeDSS(){

}

bool CommunicationGatewayFacadeDSS::init( const SInitSettings & _settings ){

    SAmqpRouteParameters initialRoute;
    initialRoute.predatorExchangePointName = "dss_dx_core";
    initialRoute.predatorQueueName = "dss_q_core_mailbox";
    initialRoute.predatorRoutingKeyName = "dss_rk_to_core";

    m_settings = _settings;
    m_settings.paramsForInitialAmqp.route = initialRoute;
    m_settings.specParams.factory = new CommandFactory( m_settings.services );

    if( ! CommunicationGatewayFacade::init(_settings) ){
        return false;
    }

    // configure communicators by amqp controllers





    const string nodeWorkerName = "simula_ctx0_mis0";
    const string nodeAgentName = "simula_ip0_pid0";

    // player agent & workers...




    // node worker -> core
    SAmqpRouteParameters routes6;
    routes6.predatorExchangePointName = "dss_dx_node_workers";
    routes6.predatorQueueName = "dss_q_node_worker_mailbox_" + nodeWorkerName;
    routes6.predatorRoutingKeyName = "dss_rk_to_node_worker_" + nodeWorkerName;
    routes6.targetExchangePointName = "dss_dx_core";
    routes6.targetRoutingKeyName = "dss_rk_to_core";

    // node agent -> core
    SAmqpRouteParameters routes5;
    routes5.predatorExchangePointName = "dss_dx_node_agents";
    routes5.predatorQueueName = "dss_q_node_agent_mailbox_" + nodeAgentName;
    routes5.predatorRoutingKeyName = "dss_rk_to_node_agent_" + nodeAgentName;
    routes5.targetExchangePointName = "dss_dx_core";
    routes5.targetRoutingKeyName = "dss_rk_to_core";





    return true;
}

common_types::IServiceExternalCommunication * CommunicationGatewayFacadeDSS::serviceForExternalCommunication(){
    return this;
}

common_types::IServiceInternalCommunication * CommunicationGatewayFacadeDSS::serviceForInternalCommunication(){
    return this;
}

PNetworkClient CommunicationGatewayFacadeDSS::getNodeAgentCommunicator( const std::string & _uniqueId ){

    auto iter = m_nodeAgentCommunicatorsById.find( _uniqueId );
    if( iter != m_nodeAgentCommunicatorsById.end() ){
        return iter->second;
    }

    // core -> node agent
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_core";
    route.predatorQueueName = "dss_q_core_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_core";
    route.targetExchangePointName = "dss_dx_node_agents";
    route.targetRoutingKeyName = "dss_rk_to_node_agent_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_nodeAgentCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}

PNetworkClient CommunicationGatewayFacadeDSS::getNodeWorkerCommunicator( const std::string & _uniqueId ){

    auto iter = m_nodeWorkerCommunicatorsById.find( _uniqueId );
    if( iter != m_nodeWorkerCommunicatorsById.end() ){
        return iter->second;
    }

    // core -> node worker
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_core";
    route.predatorQueueName = "dss_q_core_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_core";
    route.targetExchangePointName = "dss_dx_node_workers";
    route.targetRoutingKeyName = "dss_rk_to_node_worker_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_nodeWorkerCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}

PNetworkClient CommunicationGatewayFacadeDSS::getPlayerAgentCommunicator(){

    // TODO: ?


    return nullptr;
}

PNetworkClient CommunicationGatewayFacadeDSS::getPlayerWorkerCommunicator( const std::string & _uniqueId ){

    auto iter = m_playerWorkerCommunicatorsById.find( _uniqueId );
    if( iter != m_playerWorkerCommunicatorsById.end() ){
        return iter->second;
    }

    // core -> node agent
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_core";
    route.predatorQueueName = "dss_q_core_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_core";
    route.targetExchangePointName = "dss_dx_player_workers";
    route.targetRoutingKeyName = "dss_rk_to_player_worker_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_playerWorkerCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}

PNetworkClient CommunicationGatewayFacadeDSS::getUserCommunicator( const std::string & _uniqueId ){

    auto iter = m_userCommunicatorsById.find( _uniqueId );
    if( iter != m_userCommunicatorsById.end() ){
        return iter->second;
    }

    // core -> client
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_core";
    route.predatorQueueName = "dss_q_core_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_core";
    route.targetExchangePointName = "dss_dx_users";
    route.targetRoutingKeyName = "dss_rk_to_user_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_userCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}



















