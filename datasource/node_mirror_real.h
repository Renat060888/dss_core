#ifndef NODE_REAL_H
#define NODE_REAL_H

#include "i_node_mirror.h"

class NodeMirrorReal : public INodeMirror
{
public:
    struct SStateReal : INodeMirror::SState {

    };

    struct SConfigReal : INodeMirror::SConfiguration {


    };

    NodeMirrorReal();

    const SStateReal & getState();

    virtual bool configure( SConfiguration * _conf ) override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:



};
using PNodeMirrorReal = std::shared_ptr<NodeMirrorReal>;

#endif // NODE_REAL_H
