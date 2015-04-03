#ifndef AGENT__
#define AGENT__

#include <string>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent {
 public:
    Agent() {
        trufa_size_=-1;
        bump_=false;

        posx = 0;
        posy = 0;
    }

    enum ActionType {
        actFORWARD,
        actTURN_L,
        actTURN_R,
        actSNIFF,
        actEXTRACT,
        actIDLE
    };

    enum Direction {
        up,
        right,
        down,
        left
    };

    void Perceive(const Environment &env);
    ActionType Think();
    ActionType Think_random();
    Direction updateDir(Direction, ActionType);
    void updatePos(Direction);

 private:
    int trufa_size_;
    bool bump_;

    int posx;
    int posy;
};

string ActionStr(Agent::ActionType);

#endif
