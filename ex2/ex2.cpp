#include <string.h>
#include <omnetpp.h>

class Sender : public cSimpleModule {
protected:
  virtual void initialize() override {
    // Get delay time as random value from exponential distribution as set in omnetpp.ini.
    simtime_t delay = par("delayTime");
    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }
  virtual void handleMessage(cMessage *msg) override {
    delete msg;
    simtime_t delay = par("delayTime");
    EV << "Delay expired, sending another message. New delay is: " << delay << endl;
    // Send out a message to the reciever.
    send(new cMessage("msg"), "out");
    // Schedule a self message after delay.
    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }
};

Define_Module(Sender);

class Reciever : public cSimpleModule {
protected:
  virtual void initialize() override {
    lastArrivalTime = 0.0;
    interarrivalTimeAccumulated = 0.0;
    messageCount = 0;
  }
  virtual void handleMessage(cMessage *msg) override {
    delete msg;
    messageCount++;
    interarrivalTimeAccumulated += simTime().dbl() - lastArrivalTime;
    lastArrivalTime = simTime().dbl();
    EV << "Recieved message. Current interarrival time is: " << interarrivalTimeAccumulated / messageCount << "s" <<  endl;
  }
private:
  double lastArrivalTime;
  double interarrivalTimeAccumulated;
  int messageCount;
};

Define_Module(Reciever);
