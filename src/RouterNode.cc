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

    // Always send using out[0]
    int outIx = 0;
    cGate *outGate = gate("out", outIx);

    // Retrieve the channel
    cChannel *channel = outGate->getTransmissionChannel();

    // Default "readyAt" is now
    simtime_t readyAt = simTime();

    // If this gate has a channel, get its transmission finish time
    if (channel != nullptr)
        readyAt = channel->getTransmissionFinishTime();

    if (readyAt <= simTime()) {
        // Channel is free, send packet
        cMessage *pkt = (cMessage *)buffer.pop();
        send(pkt, outGate);

        // Emit congestion index signal
        double ci = computeCI();
        emit(ciSignal, ci);

        // Optionally send feedback
        if (uniform(0, 1) < 0.15)
            sendFeedback(ci);

        // Schedule next transmission if queue not empty
        if (!buffer.isEmpty() && channel != nullptr)
            scheduleAt(channel->getTransmissionFinishTime(), sendNextEvent);
    } else {
        // Still transmitting, reschedule when ready
        scheduleAt(readyAt, sendNextEvent);
    }
}

double RouterNode::computeCI()
{
    double q = ((double)buffer.getLength()) / capacity;
    double l = (double)droppedPackets / (droppedPackets + 1);
    // Simple queue+loss congestion indicator
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
