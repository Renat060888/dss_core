
#include <dss_common/communication/protocols/dss_core.pb.h>
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include "node_agent_simulation_proxy.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "NodeAgentSimula:";


NodeAgentSimulationProxy::NodeAgentSimulationProxy()
{

}

NodeAgentSimulationProxy::~NodeAgentSimulationProxy(){

    VS_LOG_INFO << PRINT_HEADER << " DTOR id: " << m_state.nodeAgentId << endl;

}

bool NodeAgentSimulationProxy::init( const SInitSettings & _settings ){

    m_networkWithAgent = _settings.network;



    return true;
}

bool NodeAgentSimulationProxy::requestWorkers( common_types::TContextId _ctxId ){

    // create request
    google_protobuf_dss_core::MessageHeader header;
    header.set_pid( ::getpid() );
    header.set_ip_octets( common_utils::ipAddressToOctets(common_utils::getIpAddressStr()) );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_core::MessageRequestRequestWorker messageRequestWorker;
    messageRequestWorker.set_context_id( _ctxId );

    google_protobuf_dss_core::MessageCoreRequest messageCoreRequest;
    messageCoreRequest.mutable_header()->CopyFrom( header );
    messageCoreRequest.set_req_type( ::google_protobuf_dss_core::ECoreRequestType::CRQT_REQUEST_WORKER );
    messageCoreRequest.mutable_msg_request_worker()->CopyFrom( messageRequestWorker );

    google_protobuf_dss_core::MessageCore messageCore;
    messageCore.set_msg_type( ::google_protobuf_dss_core::ECoreMessageType::CMT_REQUEST );
    messageCore.mutable_request()->CopyFrom( messageCoreRequest );

    // send
    PEnvironmentRequest request = m_networkWithAgent->getRequestInstance();
    request->sendMessageAsync( messageCore.SerializeAsString() );

    return true;
}

void NodeAgentSimulationProxy::releaseWorker( common_types::TNodeId _nodeId ){

    // create request
    google_protobuf_dss_core::MessageHeader header;
    header.set_pid( ::getpid() );
    header.set_ip_octets( common_utils::ipAddressToOctets(common_utils::getIpAddressStr()) );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_core::MessageRequestReleaseWorker messageReleaseWorker;
    messageReleaseWorker.set_worker_id( _nodeId );

    google_protobuf_dss_core::MessageCoreRequest messageCoreRequest;
    messageCoreRequest.mutable_header()->CopyFrom( header );
    messageCoreRequest.set_req_type( ::google_protobuf_dss_core::ECoreRequestType::CRQT_RELEASE_WORKER );
    messageCoreRequest.mutable_msg_release_worker()->CopyFrom( messageReleaseWorker );

    google_protobuf_dss_core::MessageCore messageCore;
    messageCore.set_msg_type( ::google_protobuf_dss_core::ECoreMessageType::CMT_REQUEST );
    messageCore.mutable_request()->CopyFrom( messageCoreRequest );

    // send
    PEnvironmentRequest request = m_networkWithAgent->getRequestInstance();
    request->sendMessageAsync( messageCore.SerializeAsString() );
}

void NodeAgentSimulationProxy::updateState( const common_types::SNodeAgentSimulateState & _state ){

    m_state = _state;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

void NodeAgentSimulationProxy::forwardNodeWorkerRequest( const string & _serializedRequest ){

    // create request
    google_protobuf_dss_core::MessageHeader header;
    header.set_pid( ::getpid() );
    header.set_ip_octets( common_utils::ipAddressToOctets(common_utils::getIpAddressStr()) );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_core::MessageRequestForwardCommandToWorker messageCmdToWorker;
    messageCmdToWorker.set_serialized_command_to_worker( _serializedRequest );

    google_protobuf_dss_core::MessageCoreRequest messageCoreRequest;
    messageCoreRequest.mutable_header()->CopyFrom( header );
    messageCoreRequest.set_req_type( ::google_protobuf_dss_core::ECoreRequestType::CRQT_FORWARD_COMMAND_TO_WORKER );
    messageCoreRequest.mutable_msg_forward_command_to_worker()->CopyFrom( messageCmdToWorker );

    google_protobuf_dss_core::MessageCore messageCore;
    messageCore.set_msg_type( ::google_protobuf_dss_core::ECoreMessageType::CMT_REQUEST );
    messageCore.mutable_request()->CopyFrom( messageCoreRequest );

    // send
    PEnvironmentRequest request = m_networkWithAgent->getRequestInstance();
    request->sendMessageAsync( messageCore.SerializeAsString() );
}









