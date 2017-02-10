enum ACTION {EXECUTE_ATTACK=4, DEPLOY_ATTACK=3, EXECUTE_REPAIR=2, DEPLOY_REPAIR=1, NOTIFY=0};

struct Event {
  signed char source;
  signed char target;
  signed char action;
}

struct SysAdmin {
  // ****, we're dealing with a sysadmin (htttps://xkcd.com/705/)
  int nextFixTime;
}

bool tiebreaker(Event& x1, int p1, Event& x2, int p2) {
  return x1.action > x2.action;
}

enum END_CONDITIONS {QUEUE_EMPTY, NETWORK_CONQUERED, NETWORK_DEFENDED, TIMED_OUT}
class Simulator {
  private:
    int time = 0;
    int maxTime;
    int numComputers;
    int attackProbability;
    int detectProbability;
    PriorityQueue<Event, tiebreaker> q;
    SysAdmin sysadmin;
    bool computers[this->numComputers];
    std::mt19937 mt(time(0));
    std::uniform_int_distribution<int> prob_distribution(1, 100);
    std::uniform_int_distribution<int> comp_distribution(0, numComputers - 1);

    Simulator(int numComputers, int attackProbability, int detectProbability);

    
    int computersInfected() {
      int infected = 0;
      for (int i = 0; i < this->numComputers; i++) {
        if (computers[i]) infected++;
      }
      return infected;
    }

    Event fetch(); 
      
    void process(Event& e);
    bool attempt(int prob) {  return this->prob_distribution(this->mt) > prob;  }
    int randomComputer(int computer) {  
      int randComp = this->comp_distribution(this->mt);  
      return (randComp != computer) ? randComp : this->randomComputer(computer);
    }
    bool detectedByIDS(int source, int target);
  public:
    void run();
}

Simulator::Simulator(int numComputers, int attackProbability, int detectProbability)
  : numComputers(numComputers), attackProbability(attackProbability), detectProbability(detectProbability), mt(time(0)), comp_distribution(0, numComputers - 1) {
  bool *computers = new bool[this->numComputers];
  for (int i = 0; i < this->numComputers; i++) {
    computers[i] = false;
  }
}

void Simulator::run() {
  try {
    while (True) {
      this->process(this->fetch());
    }
  } catch NETWORK_CONQUERED {
    std::cout << "Attacker wins" << std::endl;
  } catch NETWORK_DEFENDED {
    std::cout << "Sysadmin wins" << std::endl;
  } catch TIMED_OUT {
    std::cout << "Draw" << std::endl;
  }
}


void Simulator::notify(int source) {
  Event e;
  e.action = NOTIFY;
  e.source = source;
  this->q.push(e, this->this + 100);
}

void Simulator::deploy_attack(int source) {
  Event e;
  e.action = DEPLOY_ATTACK;
  e.source = source;
  e.target = this->randomComputer(e.source);
  this->q.push(e, this->time + 1000);
}

void Simulator::execute_attack(int source, int target) {
  Event e;
  e.action = EXECUTE_ATTACK;
  e.source = source;
  this->q.push(e, this->time + 100);
}

void Simulator::deploy_repair(int target) {
  Event e;
  e.action = DEPLOY_REPAIR;
  e.target = target;
  this->sysadmin.nextFixTime += 10000;
  this->q.push(e, this->sysadmin.nextFixTime);
}

void Simulator::execute_repair(int target) {
  Event e;
  e.action = EXECUTE_REPAIR;
  e.target = target;
  this->q.push(e, this->time + 100);
}

bool Simulator::detectedByIDS(int source, int target) {
  if (source == -1) {
    return true;
  } else {
    bool sourceSide = (source > (this->numComputers / 2));
    bool targetSide = (target > (this->numComputers / 2));
    bool crossesIDS = (sourceSide != targetSide);
    return crossesIDS && this->attempt(this->detectProbability);
  }
}

void Simulator::fetch() {
  if (q.isEmpty()) throw QUEUE_EMPTY; 
  if (computersInfected() > (numComputers + 1) / 2) throw NETWORK_CONQUERED;
  if (computersInfected() == 0) throw NETWORK_DEFENDED;

  auto next = q.pop();
  this->time = next.time;
  if (this->time > maxTime) throw TIMED_OUT;
  
  process(next.content);
}
        
void Simulator::processDeployAttack(Event& e) {
  if (e.source == -1 || this->computers[e.source]) {
    this->execute_attack(e.source, e.target);
    this->deploy_attack(e.source);
  }
}
void Simulator::processExecuteAttack(Event& e) {
  if (this->attempt(this->attackProbability)) {
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
  this->deploy_repair(e.target);
}

void Simulator::process(Event& e) {
  switch e.action {
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
  Simulator(atoi(argv[0]), atoi(argv[1]), atoi(argv[2])) simulator;
  simulator.run();
}
