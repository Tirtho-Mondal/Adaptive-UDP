#ifndef __ROUTER_NODE_H
#define __ROUTER_NODE_H

#include <omnetpp.h>
#include <vector>
using namespace omnetpp;

// Genetic Algorithm individual definition
struct GAIndividual {
    std::vector<int> path;
    double fitness;
};

class RouterNode : public cSimpleModule
{
  private:
    cQueue buffer;
    cMessage *sendNextEvent;
    int capacity;
    int droppedPackets;
    simsignal_t ciSignal;
    int nextOutGate;

    // GA members
    double mutationRate;
    double crossoverRate;
    std::vector<GAIndividual> population;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void trySend();
    virtual void finish() override;

    // GA functions
    void initializeGA();
    void evolvePopulation();
    void adjustGAParameters(double improvement, double diversity);
    double computeFitness(const GAIndividual& ind);
    double computePopulationDiversity();
    double computeCI();
};

#endif
