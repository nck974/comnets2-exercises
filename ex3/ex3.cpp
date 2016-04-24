#include <string.h>
#include <omnetpp.h>
#include <cmath>

class LCG {
public:
  LCG(int seed) : lastValue(seed) {}
  int getRandom() {
    lastValue = 16807 * lastValue % ((int) std::pow(2, 31) - 1);
    return lastValue;
  }
private:
  int lastValue;
};

class Uniform {
public:
  double draw(int min, int max) {
    double random = generator.getRandom();
    if (random < min)
      random *= -1;
    random /= std::pow(2, 31);
    return min + (max - min) * random;
  }
private:
  LCG generator = LCG(1);
};

class Exponential {
public:
  double draw() {
    return -std::log(1 - distribution.draw(0, 1));
  }
private:
  Uniform distribution;
};

class Sender : public cSimpleModule {
protected:
  virtual void initialize() override {
    // Get delay time as random value from exponential distribution as set in omnetpp.ini.
    //simtime_t delay = par("delayTime");
    simtime_t delay = SimTime(distribution_exponential.draw());
    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }
  virtual void handleMessage(cMessage *msg) override {
    delete msg;
    //simtime_t delay = par("delayTime");
    simtime_t delay = SimTime(distribution_exponential.draw());
    EV << "Delay expired, sending another message. New delay is: " << delay << endl;
    // Send out a message to the reciever.
    send(new cMessage("msg"), "out");
    // Schedule a self message after delay.
    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }
private:
  Uniform distribution_uniform;
  Exponential distribution_exponential;
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
