#ifndef NODE_SIMULATION_H
#define NODE_SIMULATION_H

#include "i_node_mirror.h"

class NodeMirrorSimulation : public INodeMirror
{
public:
    struct SStateSimulation : INodeMirror::SState {

    };

    struct SConfigSimulation : INodeMirror::SConfiguration {


    };

    NodeMirrorSimulation();

    const SStateSimulation & getState();

    virtual bool configure( SConfiguration * _conf ) override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:



};
using PNodeMirrorSimulation = std::shared_ptr<NodeMirrorSimulation>;

#endif // NODE_SIMULATION_H
