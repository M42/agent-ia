#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
using namespace std;
typedef long long int lli;
typedef vector< vector<lli> > Map;

const int INITIAL_MAP  = 2000;
const int INITIAL_GROW = 15; 

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
        
        mapa = Map(20, vector<lli>(20, INITIAL_MAP));
        currAction = actIDLE;
        pastAction = actIDLE;

        growFactor = INITIAL_GROW;
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

    int growFactor;
};

string ActionStr(Agent::ActionType);

#endif
