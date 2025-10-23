#ifndef __ACCESS_POINT_NODE_H
#define __ACCESS_POINT_NODE_H

#include <omnetpp.h>
using namespace omnetpp;

class AccessPointNode : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

#endif
