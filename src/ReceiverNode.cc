#include "ReceiverNode.h"

Define_Module(ReceiverNode);

void ReceiverNode::initialize()
{
    packetsReceived = 0;
}

void ReceiverNode::handleMessage(cMessage *msg)
{
    packetsReceived++;
    EV << getFullName() << " received packet #" << packetsReceived
       << " from " << msg->getSenderModule()->getFullName() << "\n";
    delete msg;
}

void ReceiverNode::finish()
{
    recordScalar("TotalPacketsReceived", packetsReceived);
}
