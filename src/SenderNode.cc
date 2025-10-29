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
        std::string pktName = "UDP_Packet_" + std::to_string(seqNum) + "_" + std::to_string((int)packetSize) + "B";
        cPacket *pkt = new cPacket(pktName.c_str());
        pkt->setByteLength(packetSize);
        EV << "[SEND] Time: " << simTime()
           << " | Packet #" << seqNum
           << " | Sent size: " << packetSize << " bytes" << endl;
        send(pkt, "out");
        seqNum++;
        scheduleAt(simTime() + sendInterval, sendTimer);
    }
    else if (msg->hasPar("SuggestedPacketSize")) {
        double suggested = msg->par("SuggestedPacketSize").doubleValue();
        EV << "[GA FEEDBACK] Time: " << simTime()
           << " | Feedback size: " << static_cast<cPacket*>(msg)->getByteLength()
           << " bytes | SuggestedPacketSize=" << suggested << endl;
        double oldSize = packetSize;
        packetSize = suggested;
        EV << "[ADAPTATION] Updated packet size: " << packetSize
           << " bytes (was " << oldSize << " bytes)" << endl;
        delete msg;
    }
    else if (msg->hasPar("CI")) {
        EV << "[FEEDBACK] Time: " << simTime()
           << " | Feedback size: " << static_cast<cPacket*>(msg)->getByteLength()
           << " bytes | CI=" << msg->par("CI").doubleValue() << endl;
        double oldSize = packetSize;
        adjustPacketSize(msg->par("CI").doubleValue());
        EV << "[ADAPTATION] Updated packet size: " << packetSize
           << " bytes (was " << oldSize << " bytes)" << endl;
        delete msg;
    }
    else {
        delete msg;
    }
}

void SenderNode::adjustPacketSize(double ci)
{
    if (ci < 0.3)
        packetSize = std::min(1500.0, packetSize + 100);
    else if (ci < 0.6)
        ; // No change
    else if (ci < 0.8)
        packetSize = std::max(400.0, packetSize - 150);
    else
        packetSize = std::max(200.0, packetSize - 300);
}

void SenderNode::finish()
{
    cancelAndDelete(sendTimer);
}
