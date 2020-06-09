#ifndef NODE_DISPATCHER_DUMP_H
#define NODE_DISPATCHER_DUMP_H

#include "i_node_worker_service.h"

class DispatcherNodeDump
{
public:
    struct SNodeRequestForm {
        int contextId;
        int missionId;
    };

    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string m_lastError;
    };

    DispatcherNodeDump();

    bool init( const SInitSettings & _settings );
    void addObserver( INodeDispatcherObserver * _observer );
    void removeObserver( INodeDispatcherObserver * _observer );
    void runSystemClock();

    bool requestNode( const SNodeRequestForm & _form );
    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();
    void releaseNode( PNodeMirror _nodeMirror );



private:




};

#endif // NODE_DISPATCHER_DUMP_H
