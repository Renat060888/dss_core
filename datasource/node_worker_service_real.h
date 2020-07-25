#ifndef NODE_REAL_H
#define NODE_REAL_H

#include "i_node_worker_service.h"

class NodeWorkerServiceReal : public INodeMirror
{
    friend class DispatcherNodeReal;
public:
    struct SConfigReal : INodeMirror::SConfiguration {
        // bool realtime; // only for simula/dump node
        std::string capability;
        int64_t pollIntervalMillisec;
    };

    NodeWorkerServiceReal();

    bool configure( const SConfigReal & _cfg );
    const common_types::SNodeWorkerRealState & getState();
    virtual const common_types::SNodeState & getBaseState() override;


    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:
    void updateState( const common_types::SNodeWorkerRealState & _workerState );

    // data
    common_types::SNodeWorkerRealState m_state;


};
using PNodeWorkerServiceReal = std::shared_ptr<NodeWorkerServiceReal>;


// observer
class INodeRealObserver {
public:
    ~INodeRealObserver(){}

    virtual void callbackNodeReal( PNodeWorkerServiceReal _node, bool _online ) = 0;
};

#endif // NODE_REAL_H
