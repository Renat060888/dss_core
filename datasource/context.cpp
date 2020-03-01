
#include "context.h"

using namespace std;

Context::Context()
{

}

bool Context::addSimulaNodeMirror( PNodeMirrorSimulation _node ){


    // TODO: check for ctx & mission correctness


    return true;
}

bool Context::addRealNodeMirror( PNodeMirrorReal _node ){


    // TODO: check for ctx & mission correctness



    return true;
}

bool Context::init( const SInitSettings & _settings ){

}

PNodeMirror Context::getNode( const common_types::TNodeId & _id ){

}

void Context::callbackNodeDetected( const common_types::TNodeId & _id ){

}

void Context::callbackNodeDisappeared( const common_types::TNodeId & _id ){

}
