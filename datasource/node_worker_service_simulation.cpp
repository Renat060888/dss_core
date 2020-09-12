
#include <microservice_common/system/logger.h>
#include <dss_common/communication/protocols/dss_node_agent_simulation.pb.h>
#include <dss_common/common/common_utils.h>

#include "node_worker_service_simulation.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "NodeWorkerSimula:";

NodeWorkerServiceSimula::NodeWorkerServiceSimula( PNodeAgentFacilityForWorker _nodeAgent )
    : m_nodeAgentFacility(_nodeAgent)
{

}

NodeWorkerServiceSimula::~NodeWorkerServiceSimula(){

    VS_LOG_INFO << PRINT_HEADER << " DTOR id: " << m_state.id << endl;
}

const SNodeWorkerSimulationState & NodeWorkerServiceSimula::getState(){
    return m_state;
}

const common_types::SNodeState & NodeWorkerServiceSimula::getBaseState(){
    return m_state;
}

void NodeWorkerServiceSimula::updateState( const common_types::SNodeWorkerSimulationState & _workerState ){

    m_state = _workerState;
    m_state.lastPingAtMillisec = common_utils::getCurrentTimeMillisec();
}

bool NodeWorkerServiceSimula::configure( const SConfigSimulation & _cfg ){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestConfigure messageRequestConfigure;
    messageRequestConfigure.set_realtime( _cfg.realtime );
    messageRequestConfigure.set_processing_interval_millisec( _cfg.pollIntervalMillisec );

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_CONFIGURE );
    messageAgentSimulaToWorkerRequest.mutable_msg_configure()->CopyFrom( messageRequestConfigure );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

bool NodeWorkerServiceSimula::start(){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestStart messageRequestStart;

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_START );
    messageAgentSimulaToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestStart );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

bool NodeWorkerServiceSimula::pause(){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestPause messageRequestPause;

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_PAUSE );
    messageAgentSimulaToWorkerRequest.mutable_msg_configure()->CopyFrom( messageRequestPause );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

bool NodeWorkerServiceSimula::stop(){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestStop messageRequestStop;

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_STOP );
    messageAgentSimulaToWorkerRequest.mutable_msg_start()->CopyFrom( messageRequestStop );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );

    return true;
}

void NodeWorkerServiceSimula::switchLivePlaying( bool _live ){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestLiveProcessing messageRequestLiveProcessing;
    messageRequestLiveProcessing.set_live( _live );

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_LIVE_PROCESSING );
    messageAgentSimulaToWorkerRequest.mutable_msg_configure()->CopyFrom( messageRequestLiveProcessing );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );
}

void NodeWorkerServiceSimula::useRTI( bool _use ){

    // create request
    google_protobuf_dss_agent_simula::MessageHeader header; // NOTE: header must be filled by agent before sending to target worker
    header.set_pid( -1 );
    header.set_ip_octets( -1 );
    header.set_last_error( "no_errors" );

    google_protobuf_dss_agent_simula::MessageRequestUseRTI messageRequestUseRTI;
    messageRequestUseRTI.set_use( _use );

    google_protobuf_dss_agent_simula::MessageAgentSimulaToWorkerRequest messageAgentSimulaToWorkerRequest;
    messageAgentSimulaToWorkerRequest.mutable_header()->CopyFrom( header );
    messageAgentSimulaToWorkerRequest.set_cmd_type( ::google_protobuf_dss_agent_simula::EAgentToWorkerRequestType::AWCT_USE_RTI );
    messageAgentSimulaToWorkerRequest.mutable_msg_configure()->CopyFrom( messageRequestUseRTI );

    // forward
    const string & requestToWorkerStr = messageAgentSimulaToWorkerRequest.SerializeAsString();
    m_nodeAgentFacility->forwardNodeWorkerRequest( requestToWorkerStr );
}






