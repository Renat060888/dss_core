#ifndef I_NODE_MIRROR_H
#define I_NODE_MIRROR_H

#include <memory>
#include <string>

#include <dss_common/common/common_types.h>
#include <dss_common/common/common_vars.h>

class INodeDispatcherObserver {
public:
    virtual ~INodeDispatcherObserver(){}

    virtual void callbackNodeDetected( const common_types::TNodeId & _id ) = 0;
    virtual void callbackNodeDisappeared( const common_types::TNodeId & _id ) = 0;
};

class INodeMirror
{
public:        
    struct SState {
        SState()
            : id(common_vars::INVALID_NODE_ID)
            , alive(false)
            , busy(false)
            , status(common_types::ENodeStatus::UNDEFINED)
        {}
        common_types::TNodeId id;
        bool alive;
        bool busy;
        std::string lastError;
        common_types::ENodeStatus status;
    };

    struct SConfiguration {

    };

    INodeMirror();
    virtual ~INodeMirror(){}

    virtual bool configure( SConfiguration * _conf ) = 0;

    virtual bool start() = 0;
    virtual bool pause() = 0;
    virtual bool reset() = 0;

    virtual void switchLivePlaying( bool _live ) = 0;

    virtual void resetTime() = 0;
};
using PNodeMirror = std::shared_ptr<INodeMirror>;

#endif // I_NODE_MIRROR_H
