#include <string.h>
#include <omnetpp.h>
#include <cmath>
#include <vector>

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

/**
* Applies inverse transform sampling method.
*/
class Exponential {
public:
  double draw() {
    // 1) Draw u uniformly from [0,1]
    // 2) Find x s.t. F(x)=u where F is the CDF of the exponential distribution
    // or equivalently solve the inverse CDF for u: x=F^(-1)(u) where F^(-1)(u)=-log(1-u).
    return -std::log(1 - distribution.draw(0, 1));
  }
private:
  Uniform distribution;
};

class Sender : public cSimpleModule {
protected:

  virtual void initialize() override {
    useLCG = par("useLCG");
    EV << "LCG Mode is: " << useLCG << std::endl;
    simtime_t delay;
    if (useLCG) {
      delay = SimTime(distribution_exponential.draw());
      EV << "LCG-drawn delay: " << delay << endl;
    } else {
      delay = par("delayTime");
      EV << "Mersenne-Twister delay: " << delay << endl;
    }

    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }

  virtual void handleMessage(cMessage *msg) override {
    delete msg;
    simtime_t delay;
    if (useLCG) {
      delay = SimTime(distribution_exponential.draw());
      EV << "LCG-drawn delay: " << delay << endl;
    } else {
      delay = par("delayTime");
      EV << "Mersenne-Twister delay: " << delay << endl;
    }
    // EV << "Delay expired, sending another message. New delay is: " << delay << endl;
    // Send out a message to the reciever.
    send(new cMessage("msg"), "out");
    // Schedule a self message after delay.
    scheduleAt(simTime() + delay, new cMessage("selfmsg"));
  }

private:
  Uniform distribution_uniform;
  Exponential distribution_exponential;
  bool useLCG;
};

Define_Module(Sender);

class Reciever : public cSimpleModule {
protected:

  virtual void initialize() override {
    lastArrivalTime = 0;
    interarrivalTimeAccumulated = 0;
    messageCount = 0;
    meanInterarrivalTime = 0;
    WATCH(meanInterarrivalTime);
    interarrivalTimesVector.setName("Interarrival Times");
  }

  virtual void handleMessage(cMessage *msg) override {
    delete msg;
    // Remember all arrival times.
    double currentInterarrivalTime = simTime().dbl() - lastArrivalTime;
    interarrivalTimesVector.record(currentInterarrivalTime);
    interarrivalTimesStats.collect(currentInterarrivalTime);
    EV << currentInterarrivalTime << endl;
    // Calculate the mean interarrival time, too.
    messageCount++;
    interarrivalTimeAccumulated += currentInterarrivalTime;
    meanInterarrivalTime = interarrivalTimeAccumulated / messageCount;
    // Set last arrival time to current one for next arrival.
    lastArrivalTime = simTime().dbl();
    // EV << "Recieved message. Mean interarrival time: " << meanInterarrivalTime << endl;
  }

private:
  cOutVector interarrivalTimesVector;
  cLongHistogram interarrivalTimesStats;
  double lastArrivalTime;
  double interarrivalTimeAccumulated;
  double meanInterarrivalTime;
  int messageCount;
};

Define_Module(Reciever);
