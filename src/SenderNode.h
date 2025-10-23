#ifndef __SENDER_NODE_H
#define __SENDER_NODE_H

#include <omnetpp.h>
using namespace omnetpp;

class SenderNode : public cSimpleModule
{
  private:
    double packetSize;
    double sendInterval;
    cMessage *sendTimer;
    int seqNum;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void adjustPacketSize(double ci);
    virtual void finish() override;
};

#endif
