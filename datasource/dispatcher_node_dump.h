#ifndef NODE_DISPATCHER_DUMP_H
#define NODE_DISPATCHER_DUMP_H

#include "i_node_worker_service.h"
#include "node_worker_service_dump.h"

class DispatcherNodeDump
{
    friend class CommandNodeAgentPing;
public:
    struct SNodeRequestForm {
        SNodeRequestForm()
            : contextId(0)
            , missionId(0)
        {}
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
    void runSystemClock();

    void addObserver( INodeDumpObserver * _observer );
    void removeObserver( INodeDumpObserver * _observer );

    bool requestNode( const SNodeRequestForm & _form );
    PNodeMirror getNode( const common_types::TNodeId & _id );
    std::vector<PNodeMirror> getNodes();
    void releaseNode( PNodeMirror _nodeMirror );



private:




};

#endif // NODE_DISPATCHER_DUMP_H
