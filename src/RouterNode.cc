#include "RouterNode.h"

Define_Module(RouterNode);

void RouterNode::initialize()
{
    capacity = 100;
    droppedPackets = 0;
    ciSignal = registerSignal("ci");
    buffer.setName("outBuffer");
    sendNextEvent = new cMessage("sendNext");
    nextOutGate = 0;

    // GA initial
    mutationRate = 0.05;
    crossoverRate = 0.8;
    initializeGA();
}

void RouterNode::initializeGA()
{
    int popSize = 10;
    population.clear();
    for (int i = 0; i < popSize; ++i) {
        GAIndividual ind;
        ind.path = {0};
        ind.fitness = 1.0;
        population.push_back(ind);
    }
}

void RouterNode::handleMessage(cMessage *msg)
{
    if (msg == sendNextEvent) {
        trySend();
        return;
    }

    if (buffer.getLength() >= capacity) {
        droppedPackets++;
        EV << getFullName() << ": Packet dropped (queue full)\n";
        delete msg;
        return;
    }

    buffer.insert(msg);

    if (!sendNextEvent->isScheduled())
        scheduleAt(simTime(), sendNextEvent);
}

void RouterNode::trySend()
{
    if (buffer.isEmpty())
        return;

    // Evolve population using GA
    evolvePopulation();

    int numOutGates = gateSize("out");
    if (numOutGates == 0)
        return;

    int bestIdx = 0;
    for (size_t i = 1; i < population.size(); ++i)
        if (population[i].fitness > population[bestIdx].fitness)
            bestIdx = i;

    // Packet size suggestion from GA: Example mapping
    int suggestedPacketSize = 800 + int(700 * population[bestIdx].fitness);

    cGate *outGate = gate("out", population[bestIdx].path[0]);

    cChannel *channel = outGate->getTransmissionChannel();
    simtime_t readyAt = simTime();

    if (channel)
        readyAt = channel->getTransmissionFinishTime();

    if (readyAt <= simTime()) {
        cMessage *pkt = (cMessage *)buffer.pop();
        send(pkt, outGate);

        double ci = computeCI();
        emit(ciSignal, ci);

        // Always send ADAPTIVE_GA feedback with suggested packet size
        for (int i = 1; i <= 3; ++i) {
            std::string senderName = "sender" + std::to_string(i);
            cModule *senderModule = getParentModule()->getSubmodule(senderName.c_str());
            if (senderModule) {
                cPacket *feedback = new cPacket("adaptive_ga_feedback");
                feedback->setByteLength(32);
                feedback->addPar("SuggestedPacketSize") = suggestedPacketSize;
                feedback->addPar("CI") = ci; // for compatibility/logs
                sendDirect(feedback, senderModule, "in");
            }
        }

        if (!buffer.isEmpty() && channel)
            scheduleAt(channel->getTransmissionFinishTime(), sendNextEvent);
    } else {
        scheduleAt(readyAt, sendNextEvent);
    }
}

void RouterNode::evolvePopulation()
{
    double lastBest = population[0].fitness;
    for (auto &ind : population)
        ind.fitness = computeFitness(ind);

    double bestFitness = population[0].fitness;
    for (const auto &ind : population)
        if (ind.fitness > bestFitness)
            bestFitness = ind.fitness;

    double improvement = bestFitness - lastBest;
    double diversity = computePopulationDiversity();

    adjustGAParameters(improvement, diversity);

    if (uniform(0, 1) < crossoverRate) {
        int i1 = intuniform(0, population.size()-1);
        int i2 = intuniform(0, population.size()-1);
        if (i1 != i2) {
            GAIndividual child = population[i1];
            child.path[0] = population[i2].path[0];
            if (uniform(0, 1) < mutationRate)
                child.path[0] = intuniform(0, gateSize("out") - 1);
            child.fitness = computeFitness(child);
            population.push_back(child);
        }
    }
    while (population.size() > 10)
        population.pop_back();
}

void RouterNode::adjustGAParameters(double improvement, double diversity)
{
    if (improvement < 0.01)
        mutationRate *= 1.1;
    else
        mutationRate *= 0.95;

    if (diversity < 0.2) {
        mutationRate *= 1.2;
        crossoverRate *= 0.9;
    } else {
        crossoverRate *= 1.1;
    }
    mutationRate = std::max(0.01, std::min(0.3, mutationRate));
    crossoverRate = std::max(0.5, std::min(1.0, crossoverRate));
}

double RouterNode::computeFitness(const GAIndividual& ind)
{
    double q = ((double)buffer.getLength()) / capacity;
    double l = (double)droppedPackets / (droppedPackets + 1);
    return 1.0 - (0.7 * q + 0.3 * l);
}

double RouterNode::computePopulationDiversity()
{
    std::vector<int> vals;
    for (auto &ind : population)
        vals.push_back(ind.path[0]);
    if (vals.empty()) return 0;
    double mean = 0;
    for (int v : vals)
        mean += v;
    mean /= vals.size();
    double var = 0;
    for (int v : vals)
        var += (v-mean)*(v-mean);
    var /= vals.size();
    return var;
}

double RouterNode::computeCI()
{
    double q = ((double)buffer.getLength()) / capacity;
    double l = (double)droppedPackets / (droppedPackets + 1);
    return 0.7 * q + 0.3 * l;
}

void RouterNode::finish()
{
    cancelAndDelete(sendNextEvent);
}
