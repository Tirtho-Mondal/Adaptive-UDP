#ifndef __ROUTER_NODE_H
#define __ROUTER_NODE_H

#include <omnetpp.h>
using namespace omnetpp;

class RouterNode : public cSimpleModule
{
  private:
    cQueue buffer;
    cMessage *sendNextEvent;
    int capacity;
    int droppedPackets;
    simsignal_t ciSignal;
    int nextOutGate; // For round-robin output

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void trySend();
    void sendFeedback(double ci);
    double computeCI();
    virtual void finish() override;
};

#endif
