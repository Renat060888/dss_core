#ifndef NODE_DUMP_H
#define NODE_DUMP_H

#include "i_node_worker_service.h"

class NodeWorkerServiceDump : public INodeMirror
{
public:
    struct SStateDump : INodeMirror::SState {

    };

    struct SConfigDump : INodeMirror::SConfiguration {


    };

    NodeWorkerServiceDump();

    const SStateDump & getState();

    virtual bool configure( SConfiguration * _conf ) override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:





};
using PNodeWorkerServiceDump = std::shared_ptr<NodeWorkerServiceDump>;

#endif // NODE_DUMP_H
