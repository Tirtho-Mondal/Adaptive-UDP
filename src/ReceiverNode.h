#ifndef __RECEIVER_NODE_H
#define __RECEIVER_NODE_H

#include <omnetpp.h>
using namespace omnetpp;

class ReceiverNode : public cSimpleModule
{
  private:
    int packetsReceived;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif
