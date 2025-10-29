#include "RouterNode.h"
#include <omnetpp.h>

using namespace omnetpp;

Define_Module(RouterNode);

void RouterNode::initialize()
{
    capacity = 100;
    droppedPackets = 0;
    ciSignal = registerSignal("ci");
    buffer.setName("outBuffer");
    sendNextEvent = new cMessage("sendNext");
    nextOutGate = 0; // initialize the output gate index
}

void RouterNode::handleMessage(cMessage *msg)
{
    if (msg == sendNextEvent) {
        trySend();
        return;
    }

    // Handle incoming packet
    if (buffer.getLength() >= capacity) {
        droppedPackets++;
        EV << getFullName() << ": Packet dropped (queue full)\n";
        delete msg;
        return;
    }

    buffer.insert(msg);

    // Try sending immediately if not already scheduled
    if (!sendNextEvent->isScheduled())
        scheduleAt(simTime(), sendNextEvent);
}

void RouterNode::trySend()
{
    if (buffer.isEmpty())
        return;

    int numOutGates = gateSize("out");
    if (numOutGates == 0)
        return; // safeguard

    // Select output gate in round-robin
    cGate *outGate = gate("out", nextOutGate);
    nextOutGate = (nextOutGate + 1) % numOutGates;

    cChannel *channel = outGate->getTransmissionChannel();
    simtime_t readyAt = simTime();

    if (channel != nullptr)
        readyAt = channel->getTransmissionFinishTime();

    if (readyAt <= simTime()) {
        cMessage *pkt = (cMessage *)buffer.pop();
        send(pkt, outGate);

        double ci = computeCI();
        emit(ciSignal, ci);

        if (uniform(0, 1) < 0.15)
            sendFeedback(ci);

        if (!buffer.isEmpty() && channel != nullptr)
            scheduleAt(channel->getTransmissionFinishTime(), sendNextEvent);
    } else {
        scheduleAt(readyAt, sendNextEvent);
    }
}

double RouterNode::computeCI()
{
    double q = ((double)buffer.getLength()) / capacity;
    double l = (double)droppedPackets / (droppedPackets + 1);
    return 0.7 * q + 0.3 * l;
}

void RouterNode::sendFeedback(double ci)
{
    cMessage *f = new cMessage("feedback");
    f->addPar("CI") = ci;
    sendDirect(f, getParentModule()->getSubmodule("sender1"), "in");
}

void RouterNode::finish()
{
    cancelAndDelete(sendNextEvent);
}
