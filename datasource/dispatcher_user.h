#ifndef USER_DISPATCHER_H
#define USER_DISPATCHER_H

#include <map>

#include <dss_common/common/common_types.h>

#include "user.h"

class UserDispatcher : public common_types::IServiceUserAuthorization
{
    friend class CommandUserPing;
public:
    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string lastError;
    };

    UserDispatcher();
    ~UserDispatcher();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }
    virtual const std::string & getLastError() override { return m_state.lastError; }

    void runSystemClock();

    virtual void addObserver( common_types::IUserDispatcherObserver * _observer ) override;
    virtual void removeObserver( common_types::IUserDispatcherObserver * _observer ) override;

    common_types::PUserState getUser( const common_types::TUserId & _userId );
    common_types::TUserId registerUser( const std::string & _userIp, common_types::TPid _userPid );
    virtual bool isRegistered( const common_types::TUserId & _userId ) override;


private:
    // from ping
    void updateUserState( const common_types::SUserState & _state );

    void loadPreviousSessionActiveUsers();





    // data
    SState m_state;
    std::vector<common_types::PUserState> m_users;
    std::map<common_types::TUserId, common_types::PUserState> m_usersById;
    std::map<common_types::TContextId, common_types::PUserState> m_usersByContextId;
    std::vector<common_types::IUserDispatcherObserver *> m_observers;


    // service


};

#endif // USER_DISPATCHER_H
