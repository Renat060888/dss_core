#ifndef REALTIME_SYNCHRONIZER_H
#define REALTIME_SYNCHRONIZER_H

#include <thread>

#include <dss_common/communication/rti/federate.h>
#include <dss_common/common/common_types.h>

class RealtimeSynchronizer
{
public:
    struct SInitSettings {
        SInitSettings()
            : ctxId(0)
            , quantIntervalMillisec(1000)
        {}
        common_types::TContextId ctxId;
        int64_t quantIntervalMillisec;
    };

    struct SState {
        SState()
            : inSync(false)
        {}
        bool inSync;
        SInitSettings settings;
        std::string lastError;
    };

    RealtimeSynchronizer();
    ~RealtimeSynchronizer();

    bool init( const SInitSettings & _settings );
    const SState & getState();

    void run();
    void interrupt();


private:
    void threadRealtimeSync();

    rti_client_vega::RTIFederate * createAndJoinFederate( const std::string & _federationName );

    // data
    SState m_state;
    bool m_shutdownCalled;


    // service
    rti_client_vega::RTIFederate * m_federate;
    std::thread * m_trRealtimeSync;
    std::condition_variable m_cvRealtimeSync;
};

#endif // REALTIME_SYNCHRONIZER_H
