#include "pqueue.hpp"
#include <random>
#include <iostream>
#include <time.h>


enum ACTION {EXECUTE_ATTACK=4, DEPLOY_ATTACK=3, EXECUTE_REPAIR=2, DEPLOY_REPAIR=1, NOTIFY=0};

struct Event {
  int action;
  int source;
  int target;
};

struct SysAdmin {
  // ****, we're dealing with a sysadmin (htttps://xkcd.com/705/)
  int nextFixTime;
};

bool tiebreaker(Event& x1, int p1, Event& x2, int p2) {
  return x1.action > x2.action;
}

enum END_CONDITIONS {QUEUE_EMPTY, NETWORK_CONQUERED, NETWORK_DEFENDED, TIMED_OUT};
class Simulator {
  private:
    int time = 0;
    long long maxTime = 8640000000; 
    int numComputers;
    int attackProbability;
    int detectProbability;
    PriorityQueue<Event, tiebreaker> q;
    SysAdmin sysadmin;
    bool *computers;
    std::mt19937 mt;
    std::uniform_int_distribution<float> prob_distribution{0, 100};
    std::uniform_int_distribution<int> comp_distribution;
    bool hasInfected = false;

    int computersInfected() {
      int infected = 0;
      for (int i = 0; i < this->numComputers; i++) {
        if (this->computers[i] == true) {  infected++;  }
      }
      return infected;
    }

    Event fetch(); 
    void process(Event& e);

    bool attempt(int prob) {  return this->prob_distribution(this->mt) < prob;  }
    int randomComputer(int computer) {  
      int randComp = this->comp_distribution(this->mt);  
      return (randComp != computer) ? randComp : this->randomComputer(computer);
    }
    bool detectedByIDS(int source, int target);

    void processDeployAttack(Event& e);
    void processExecuteAttack(Event& e);
    void processDeployRepair(Event& e);
    void processExecuteRepair(Event& e);
    void processNotify(Event& e);

    void deploy_attack(int source);
    void execute_attack(int source, int target);
    void deploy_repair(int target);
    void execute_repair(int target);
    void notify(int source);
  public:
    Simulator(int numComputers, int attackProbability, int detectProbability);
    void run();
};

Simulator::Simulator(int numComputers, int attackProbability, int detectProbability)
  : numComputers(numComputers), attackProbability(attackProbability), detectProbability(detectProbability), comp_distribution{0, numComputers - 1} {
  this->computers = new bool[this->numComputers];
  for (int i = 0; i < this->numComputers; i++) {
    this->computers[i] = false;
  }
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  this->mt = std::mt19937(seed);
  this->comp_distribution = std::uniform_int_distribution<int>(0, numComputers - 1);
  this->deploy_attack(-1);
}

void Simulator::run() {
  std::cout << "STARTING SIMULATION" << std::endl;
  try {
    while (true) {
      Event fetched = this->fetch();
      this->process(fetched);
    }
  } catch (END_CONDITIONS e) {
    switch (e) {
      case NETWORK_CONQUERED:
        std::cout << "Attacker wins" << std::endl;
        break;
      case NETWORK_DEFENDED:
        std::cout << "Sysadmin wins" << std::endl;
        break;
      case TIMED_OUT:
        std::cout << "Draw" << std::endl;
        break;
    }
  }
}


void Simulator::notify(int source) {
  if (source != -1) {
    Event e;
    e.action = NOTIFY;
    e.source = source;
    int time = this->time + 100;
    this->q.push(e, this->time);
    std::cout << "Notify(" << time << ", " << e.source << ")" << std::endl;
  }
}

void Simulator::deploy_attack(int source) {
  Event e;
  e.action = DEPLOY_ATTACK;
  e.source = source;
  e.target = this->randomComputer(e.source);
  int time = this->time + 1000;
  this->q.push(e, time);
  std::cout << "Deploy_Attack(" << time << ", " << e.source << ", " << e.target << ")" << std::endl;
}

void Simulator::execute_attack(int source, int target) {
  Event e;
  e.action = EXECUTE_ATTACK;
  e.source = source;
  e.target = target;
  int time = this->time + 100;
  this->q.push(e, this->time + 100);
  std::cout << "Execute_Attack(" << time << ", " << e.source << ", " << e.target << ")" << std::endl;
}

void Simulator::deploy_repair(int target) {
  Event e;
  e.action = DEPLOY_REPAIR;
  e.target = target;
  this->sysadmin.nextFixTime += 10000;
  int time = this->sysadmin.nextFixTime;
  this->q.push(e, time);
  std::cout << "Deploy_Repair(" << time << ", " << e.target << ")" << std::endl;
}

void Simulator::execute_repair(int target) {
  Event e;
  e.action = EXECUTE_REPAIR;
  e.target = target;
  int time = this->time + 100;
  this->q.push(e, this->time + 100);
  std::cout << "Execute_Repair(" << time << ", " << e.target << ")" << std::endl;
}

bool Simulator::detectedByIDS(int source, int target) {
  if (source == -1) {
    return this->attempt(this->detectProbability);
  } else {
    bool sourceSide = (source > (this->numComputers / 2));
    bool targetSide = (target > (this->numComputers / 2));
    bool crossesIDS = (sourceSide != targetSide);
    return crossesIDS && this->attempt(this->detectProbability);
  }
}

Event Simulator::fetch() {
  if (q.isEmpty()) throw QUEUE_EMPTY; 
  if (this->computersInfected() > (numComputers + 1) / 2) throw NETWORK_CONQUERED;
  if (this->computersInfected() == 0 && this->hasInfected) throw NETWORK_DEFENDED;

  auto next = q.pop();
  this->time = next.priority;
  if (this->time > maxTime) throw TIMED_OUT;
  
  return next.content;
}
        
void Simulator::processDeployAttack(Event& e) {
  if (e.source == -1 || this->computers[e.source]) {
    this->execute_attack(e.source, e.target);
    this->deploy_attack(e.source);
  }
}
void Simulator::processExecuteAttack(Event& e) {
  if (this->attempt(this->attackProbability)) {
    this->hasInfected = true;
    this->computers[e.target] = true;
  }
  if (this->detectedByIDS(e.source, e.target)) {
    this->notify(e.source);
    this->notify(e.target);
  }
}

void Simulator::processDeployRepair(Event &e) {
  this->execute_repair(e.target);
}

void Simulator::processExecuteRepair(Event &e) {
  this->computers[e.target] = false;
}

void Simulator::processNotify(Event &e) {
  this->deploy_repair(e.source);
}

void Simulator::process(Event& e) {
  switch (e.action) {
    case EXECUTE_ATTACK:
      this->processExecuteAttack(e);
      break;
    case DEPLOY_ATTACK:
      this->processDeployAttack(e);
      break;
    case EXECUTE_REPAIR:
      this->processExecuteRepair(e);
      break;
    case DEPLOY_REPAIR:
      this->processDeployRepair(e);
      break;
    case NOTIFY:
      this->processNotify(e);
      break;
  }
}

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "Usage: simulator [num_computers] [percent_success] [percent_detect]" << std::endl;
    exit(1);
  }
  Simulator simulator(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
  simulator.run();
}
