#ifndef NODE_DUMP_H
#define NODE_DUMP_H

#include "i_node_worker_service.h"
#include "node_agent_dump_proxy.h"

class NodeWorkerServiceDump : public INodeMirror
{
    friend class DispatcherNodeDump;
public:
    struct SStateDump : common_types::SNodeState {

    };

    struct SConfigDump : INodeMirror::SConfiguration {
        bool realtime;
        std::string capability;
        // int64_t pollIntervalMillisec; // only for real/simula node
    };

    NodeWorkerServiceDump();

    bool configure( const SConfigDump & _cfg );
    const SStateDump & getState();
    virtual const common_types::SNodeState & getBaseState() override;


    virtual bool start() override;
    virtual bool pause() override;
    virtual bool reset() override;

    virtual void switchLivePlaying( bool _live ) override;

    virtual void resetTime() override;


private:
    // data
    SStateDump m_state;




};
using PNodeWorkerServiceDump = std::shared_ptr<NodeWorkerServiceDump>;


// observer
class INodeDumpObserver {
public:
    ~INodeDumpObserver(){}

    virtual void callbackNodeDump( PNodeWorkerServiceDump _node, bool _online ) = 0;
};

#endif // NODE_DUMP_H
