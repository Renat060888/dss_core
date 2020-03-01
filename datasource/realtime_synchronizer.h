#ifndef REALTIME_SYNCHRONIZER_H
#define REALTIME_SYNCHRONIZER_H

#include <dss_common/communication/rti/federate.h>

class RealtimeSynchronizer
{
public:
    RealtimeSynchronizer();




    const std::string & getLastError(){ return m_lastError; }



private:



    std::string m_lastError;


};

#endif // REALTIME_SYNCHRONIZER_H
