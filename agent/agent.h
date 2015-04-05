#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
using namespace std;
typedef vector< vector<int> > Map;

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
        
        mapa = Map(20, vector<int>(20, 2000));
        currAction = actIDLE;
        pastAction = actIDLE;
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
    void updateAct();

 private:
    int trufa_size_;
    bool bump_;

    int posx;
    int posy;
    int nextposx;
    int nextposy;
    Direction dir;

    ActionType currAction;
    ActionType pastAction;

    Map mapa;

    Direction gira(Direction,ActionType);
    ActionType aleatoriza(int,int,int,int,int,int);
};

string ActionStr(Agent::ActionType);

#endif
