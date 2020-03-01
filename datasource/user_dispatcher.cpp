
#include "user_dispatcher.h"

using namespace std;

UserDispatcher::UserDispatcher()
{

}

void UserDispatcher::addObserver( common_types::IUserDispatcherObserver * _observer ){

    m_observers.push_back( _observer );
}

void UserDispatcher::removeObserver( common_types::IUserDispatcherObserver * _observer ){



}

bool UserDispatcher::isRegistered( const common_types::TUserId & _userId ){

}

PUser UserDispatcher::getUser( const common_types::TUserId & _userId ){

}

bool UserDispatcher::registerUser( std::string _userIp, common_types::TPid _userPid ){

}
