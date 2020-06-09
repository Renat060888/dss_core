#ifndef NODE_REAL_H
#define NODE_REAL_H

#include "i_node_worker_service.h"

class NodeWorkerServiceReal : public INodeMirror
{
public:
    struct SStateReal : INodeMirror::SState {

    };

    struct SConfigReal : INodeMirror::SConfiguration {


    };

    NodeWorkerServiceReal();

    const SStateReal & getState();

    virtual bool configure( SConfiguration * _conf ) override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:



};
using PNodeWorkerServiceReal = std::shared_ptr<NodeWorkerServiceReal>;

#endif // NODE_REAL_H
