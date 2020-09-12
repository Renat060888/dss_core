
#include <microservice_common/system/logger.h>
#include <dss_common/communication/protocols/dss_node_agent_real.pb.h>
#include <dss_common/common/common_utils.h>

#include "node_worker_service_real.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "NodeWorkerReal:";

NodeWorkerServiceReal::NodeWorkerServiceReal( PNodeAgentFacilityForWorker _nodeAgent )
    : m_nodeAgentFacility(_nodeAgent)
{

}

NodeWorkerServiceReal::~NodeWorkerServiceReal(){

    VS_LOG_INFO << PRINT_HEADER << " DTOR id: " << m_state.id << endl;
}

bool NodeWorkerServiceReal::configure( const SConfigReal & _cfg ){

    // create request
    google_protobuf_dss_agent_real::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_real::MessageRequestConfigure messageRequestConfigure;
    messageRequestConfigure.set_ctx_id( _cfg.ctxId );
    messageRequestConfigure.set_capability( _cfg.targetCaps );
    messageRequestConfigure.set_processing_interval_millisec( _cfg.pollIntervalMillisec );

    google_protobuf_dss_agent_real::MessageAgentRealToWorkerRequest messageAgentRealToWorkerRequest;
    messageAgentRealToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentRealToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_real::EAgentToWorkerRequestType::AWCT_CONFIGURE );
    messageAgentRealToWorkerRequest.mutable_msg_configure()->CopyFrom( messageRequestConfigure );

    // forward
    const string & requestToWorkerStr = messageAgentRealToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

const SNodeWorkerRealState & NodeWorkerServiceReal::getState(){
    return m_state;
}

const SNodeState & NodeWorkerServiceReal::getBaseState(){
    return m_state;
}

void NodeWorkerServiceReal::updateState( const common_types::SNodeWorkerRealState & _workerState ){

    m_state = _workerState;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

bool NodeWorkerServiceReal::start(){

    // create request
    google_protobuf_dss_agent_real::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_real::MessageRequestStart messageRequestStart;

    google_protobuf_dss_agent_real::MessageAgentRealToWorkerRequest messageAgentRealToWorkerRequest;
    messageAgentRealToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentRealToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_real::EAgentToWorkerRequestType::AWCT_START );
    messageAgentRealToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestStart );

    // forward
    const string & requestToWorkerStr = messageAgentRealToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

bool NodeWorkerServiceReal::pause(){

    // create request
    google_protobuf_dss_agent_real::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_real::MessageRequestPause messageRequestPause;

    google_protobuf_dss_agent_real::MessageAgentRealToWorkerRequest messageAgentRealToWorkerRequest;
    messageAgentRealToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentRealToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_real::EAgentToWorkerRequestType::AWCT_PAUSE );
    messageAgentRealToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestPause );

    // forward
    const string & requestToWorkerStr = messageAgentRealToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

bool NodeWorkerServiceReal::stop(){

    // create request
    google_protobuf_dss_agent_real::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_real::MessageRequestStop messageRequestStop;

    google_protobuf_dss_agent_real::MessageAgentRealToWorkerRequest messageAgentRealToWorkerRequest;
    messageAgentRealToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentRealToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_real::EAgentToWorkerRequestType::AWCT_STOP );
    messageAgentRealToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestStop );

    // forward
    const string & requestToWorkerStr = messageAgentRealToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

void NodeWorkerServiceReal::switchLivePlaying( bool _live ){

    // create request
    google_protobuf_dss_agent_real::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_real::MessageRequestLiveProcessing messageRequestLP;
    messageRequestLP.set_live( _live );

    google_protobuf_dss_agent_real::MessageAgentRealToWorkerRequest messageAgentRealToWorkerRequest;
    messageAgentRealToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentRealToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_real::EAgentToWorkerRequestType::AWCT_LIVE_PROCESSING );
    messageAgentRealToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestLP );

    // forward
    const string & requestToWorkerStr = messageAgentRealToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );
}

void NodeWorkerServiceReal::useRTI( bool ){

    // NOTE: real workers always use RTI because they always in realtime
}





