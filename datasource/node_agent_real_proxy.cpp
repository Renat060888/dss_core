
#include <dss_common/communication/protocols/dss_core.pb.h>
#include <dss_common/common/common_utils.h>

#include "node_agent_real_proxy.h"

using namespace std;

NodeAgentRealProxy::NodeAgentRealProxy()
{

}

void NodeAgentRealProxy::forwardNodeWorkerRequest( const std::string & _serializedRequest ){

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
