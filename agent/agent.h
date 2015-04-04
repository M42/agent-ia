#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
using namespace std;
typedef vector< vector<bool> > Map;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent {
 public:
    Agent() {
        trufa_size_=-1;
        bump_=false;

        posx = 10;
        posy = 10;
        dir  = up;
        
        mapa = Map(20, vector<bool>(20, false));
        currAction = actIDLE;
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
    ActionType Think_walls();

    Direction updateDir(Direction, ActionType);
    void updatePos(Direction);
    void updateMap();

 private:
    int trufa_size_;
    bool bump_;

    int posx;
    int posy;
    Direction dir;

    ActionType currAction;

    Map mapa;
};

string ActionStr(Agent::ActionType);

#endif
