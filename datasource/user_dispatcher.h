#ifndef USER_DISPATCHER_H
#define USER_DISPATCHER_H

#include <map>

#include <dss_common/common/common_types.h>

#include "user.h"

class UserDispatcher : public common_types::IServiceUserAuthorization
{
    friend class CommandUserPing;
public:
    UserDispatcher();

    virtual void addObserver( common_types::IUserDispatcherObserver * _observer ) override;
    virtual void removeObserver( common_types::IUserDispatcherObserver * _observer ) override;
    virtual bool isRegistered( const common_types::TUserId & _userId ) override;
    virtual const std::string & getLastError() override { return m_lastError; }

    PUser getUser( const common_types::TUserId & _userId );
    bool registerUser( std::string _userIp, common_types::TPid _userPid );


private:
    void updateUserState( void * _state );

    // data
    std::vector<PUser> m_users;
    std::map<common_types::TUserId, PUser> m_usersById;
    std::map<common_types::TContextId, PUser> m_usersByContextId;
    std::vector<common_types::IUserDispatcherObserver *> m_observers;
    std::string m_lastError;



    // service





};

#endif // USER_DISPATCHER_H
