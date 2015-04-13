#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
using namespace std;
typedef long long int lli;
typedef vector< vector<lli> > Map;
typedef vector< vector<bool> > MapB;


#define INITIAL_MAP 1
#define INITIAL_GROW 15 

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
        leftposx = 9;
        leftposy = 10;
        rghtposx = 11;
        rghtposy = 10;
        dir  = up;
        
        mapa = Map(20, vector<lli>(20, INITIAL_MAP));
        expl = MapB(20, vector<bool>(20, false));
        paso = Map(20, vector<lli>(20, 0));
        currAction = actIDLE;
        pastAction = actIDLE;

        growFactor = INITIAL_GROW;
        d_suficiente_trufa = 1;
        iteration = 0;

        prob_forw = 0;
        prob_left = 0;
        prob_rght = 0;
        prob_snif = 0;
        prob_extr = 0;
        prob_idle = 0;
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
    ActionType Think_map();

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
    int leftposx;
    int leftposy;
    int rghtposx;
    int rghtposy;
    Direction dir;

    int prob_forw;
    int prob_left;
    int prob_rght;
    int prob_snif;
    int prob_extr;
    int prob_idle;

    ActionType currAction;
    ActionType pastAction;

    Map mapa;
    MapB expl;
    Map paso;

    float d_suficiente_trufa;
    int iteration;

    Direction gira(Direction,ActionType);
    ActionType aleatoriza();
    int valor(int,int);

    int growFactor;
};

string ActionStr(Agent::ActionType);

#endif
