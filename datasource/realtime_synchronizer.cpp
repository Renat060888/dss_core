
#include <microservice_common/system/logger.h>
#include <dss_common/common/common_utils.h>

#include <dss_common/system/objrepr_bus_dss.h>

#include "realtime_synchronizer.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "RealtimeSync:";

RealtimeSynchronizer::RealtimeSynchronizer()
    : m_shutdownCalled(false)
    , m_federate(nullptr)
    , m_trRealtimeSync(nullptr)
{

}

RealtimeSynchronizer::~RealtimeSynchronizer(){

    m_shutdownCalled = true;
    m_state.inSync = false;

    m_cvRealtimeSync.notify_one();
    common_utils::threadShutdown( m_trRealtimeSync );

    delete m_federate;
    m_federate = nullptr;
}

bool RealtimeSynchronizer::init( const SInitSettings & _settings ){

    m_state.settings = _settings;

    const string contextName = OBJREPR_BUS.getContextNameById( _settings.ctxId );
    m_federate = createAndJoinFederate( contextName );
    if( ! m_federate ){
        return false;
    }

    m_trRealtimeSync = new std::thread( & RealtimeSynchronizer::threadRealtimeSync, this );

    return true;
}

void RealtimeSynchronizer::threadRealtimeSync(){

    VS_LOG_INFO << PRINT_HEADER << " start real time sync THREAD" << endl;
    while( ! m_shutdownCalled ){
        std::mutex lockMutex;
        unique_lock<std::mutex> cvLock( lockMutex );
        m_cvRealtimeSync.wait( cvLock, [this]() -> bool { return m_state.inSync || m_shutdownCalled; } );

        if( m_shutdownCalled ){
            break;
        }

        VS_LOG_INFO << PRINT_HEADER << " start synchronize for context: " << m_state.settings.ctxId << endl;
        while( m_state.inSync ){
            m_federate->advanceTimeRequestBlocked();

            this_thread::sleep_for( chrono::milliseconds(m_state.settings.quantIntervalMillisec) );
        }
        VS_LOG_INFO << PRINT_HEADER << " stop synchronize for context: " << m_state.settings.ctxId << endl;
    }
    VS_LOG_INFO << PRINT_HEADER << " stop real time sync THREAD" << endl;
}

const RealtimeSynchronizer::SState & RealtimeSynchronizer::getState(){
    return m_state;
}

void RealtimeSynchronizer::run(){

    // time regulation
    if( ! m_federate->isTimeEnabled() ){
        m_federate->enableTimeRegulation();
        m_federate->enableTimeConstrain();
        while( ! m_federate->isTimeEnabled() ){
            m_federate->runRTICallbacks();
        }
    }

    m_state.inSync = true;
    m_cvRealtimeSync.notify_one();
}

void RealtimeSynchronizer::interrupt(){

    m_state.inSync = false;

    // time regulation
    if( m_federate->isTimeEnabled() ){
        m_federate->disableTimeRegulation();
        m_federate->disableTimeConstrain();
        while( m_federate->isTimeEnabled() ){
            m_federate->runRTICallbacks();
        }
    }
}

rti_client_vega::RTIFederate * RealtimeSynchronizer::createAndJoinFederate( const string & _federationName ){

    const string & rtiAddress = "";
    const string & fomPath = "";
    const string federateName = "REALTIME_SYNCHRONIZER";

    rti_client_vega::RTIFederate::SInitParameters parameters;
    parameters.RTIFOMFile = fomPath;
    parameters.RTIServerAddress = rtiAddress;
    parameters.federateName = federateName;
    parameters.enableAsyncDelivery = true;

    rti_client_vega::RTIFederate * newFederate = new rti_client_vega::RTIFederate();
    if( ! newFederate->init(parameters) ){
        return nullptr;
    }
    if( ! newFederate->connectToRTI() ){
        return nullptr;
    }
    if( ! newFederate->createFederation(_federationName) ){
        return nullptr;
    }
    if( ! newFederate->joinToFederation(_federationName) ){
        return nullptr;
    }

    newFederate->runRTICallbacks();

    return newFederate;
}






