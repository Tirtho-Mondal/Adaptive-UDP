#include "AccessPointNode.h"

Define_Module(AccessPointNode);

void AccessPointNode::handleMessage(cMessage *msg)
{
    simtime_t delay = exponential(0.005);
    sendDelayed(msg, delay, "out", 0);
}
