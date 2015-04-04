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
        nextposx = 10;
        nextposy = 9;
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
    ActionType Think_randomly();
    ActionType Think_walls();

    void update();
    void updateDir();
    void updatePos();
    void updateMap();

 private:
    int trufa_size_;
    bool bump_;

    int posx;
    int posy;
    int nextposx;
    int nextposy;
    Direction dir;

    ActionType currAction;

    Map mapa;

    Direction gira(Direction,ActionType);
};

string ActionStr(Agent::ActionType);

#endif
