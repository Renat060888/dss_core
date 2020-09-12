#ifndef I_NODE_MIRROR_H
#define I_NODE_MIRROR_H

#include <memory>
#include <string>

#include <dss_common/common/common_types.h>
#include <dss_common/common/common_vars.h>

class INodeMirror
{
public:            
    INodeMirror();
    virtual ~INodeMirror(){}

    virtual const common_types::SNodeState & getBaseState() = 0;

    virtual bool start() = 0;
    virtual bool pause() = 0;
    virtual bool stop() = 0;

    virtual void switchLivePlaying( bool _live ) = 0;
    virtual void useRTI( bool _use ) = 0;
};
using PNodeMirror = std::shared_ptr<INodeMirror>;

// > functors
class FNodeEqual {
public:
    FNodeEqual( common_types::TNodeId _nodeId )
        : m_nodeId(_nodeId)
    {}

    bool operator()( PNodeMirror _node ){
        return ( _node->getBaseState().id == m_nodeId );
    }
private:
    common_types::TNodeId m_nodeId;
};
// < functors

#endif // I_NODE_MIRROR_H



