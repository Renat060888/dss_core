#ifndef NODE_SIMULATION_H
#define NODE_SIMULATION_H

#include "i_node_worker_service.h"
#include "node_agent_simulation_proxy.h"

class NodeWorkerServiceSimula : public INodeMirror
{
public:
    struct SStateSimulation : INodeMirror::SState {

    };

    struct SConfigSimulation : INodeMirror::SConfiguration {


    };

    NodeWorkerServiceSimula( NodeAgentSimulationProxy * _nodeAgent );

    const SStateSimulation & getState();

    virtual bool configure( SConfiguration * _conf ) override;

    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:
    // data


    // service
    NodeAgentSimulationProxy * m_nodeAgent;


};
using PNodeWorkerServiceSimula = std::shared_ptr<NodeWorkerServiceSimula>;

#endif // NODE_SIMULATION_H
