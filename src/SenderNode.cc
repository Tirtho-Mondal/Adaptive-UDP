#include "SenderNode.h"

Define_Module(SenderNode);

void SenderNode::initialize()
{
    packetSize = 800;
    sendInterval = 0.2;
    seqNum = 0;
    sendTimer = new cMessage("sendTimer");
    scheduleAt(simTime(), sendTimer);
}

void SenderNode::handleMessage(cMessage *msg)
{
    if (msg == sendTimer) {
        cPacket *pkt = new cPacket(("UDP_Packet_" + std::to_string(seqNum++)).c_str());
        pkt->setByteLength(packetSize);
        send(pkt, "out");
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
    else if (msg->hasPar("CI")) {
        adjustPacketSize(msg->par("CI").doubleValue());
        delete msg;
    }
}

void SenderNode::adjustPacketSize(double ci)
{
    if (ci < 0.3)
        packetSize = std::min(1500.0, packetSize + 100);
    else if (ci < 0.6)
        packetSize = packetSize;
    else if (ci < 0.8)
        packetSize = std::max(400.0, packetSize - 150);
    else
        packetSize = std::max(200.0, packetSize - 300);

    EV << getFullName() << ": Adaptive packet size is now " << packetSize << " bytes (CI=" << ci << ")\n";
}

void SenderNode::finish()
{
    cancelAndDelete(sendTimer);
}
