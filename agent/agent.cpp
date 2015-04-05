#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <utility>

using namespace std;

// AUXILIARY FUNCTIONS
void draw_map(Map& map) {
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            if (map[i][j] == 0)
                cerr << "0";
            else
                cerr << "-";
        }
        cerr << endl;
    }
}

Agent::Direction Agent::gira(Direction d, ActionType a) {
    if (a == actTURN_L)
        switch (d) {
        case up:    return left;  break;
        case left:  return down;  break;
        case down:  return right; break;
        case right: return up;    break;
        }

    if (a == actTURN_R)
        switch (d) {
        case up:    return right; break;
        case right: return down;  break;
        case down:  return left;  break;
        case left:  return up;    break;
        }

    return d;
}

Agent::ActionType Agent::aleatoriza(int pf, int pl, int pr, int ps, int pe, int pi) {
    int suma = pf + pl + pr + ps + pe + pi;
    int prob = rand()%suma;
    
    if (prob < pf) 
        return actFORWARD;
    if (prob < pf+pl) 
        return actTURN_L;
    if (prob < pf+pl+pr) 
        return actTURN_R;
    if (prob < pf+pl+pr+ps) 
        return actSNIFF;
    if (prob < pf+pl+pr+ps+pe)
        return actEXTRACT;
    if (prob < pf+ps+pr+ps+pe+pi) 
        return actIDLE;
}

//////////////////////////////
// Reacting
//////////////////////////////

void Agent::update() {
    // Those functions do not commute!
    updateDir();
    updateMap();
    updatePos();
    updateAct();
}

void Agent::updateDir() {
    dir = gira(dir,currAction);
}

void Agent::updatePos() {
    if (currAction == actFORWARD and not bump_) {
        posx = nextposx;
        posy = nextposy;
    }

    nextposx = posx;
    nextposy = posy;

    switch (dir) {
    case up: nextposy--; break;
    case down: nextposy++; break;
    case left: nextposx--; break;
    case right: nextposx++; break;
    }
}

void Agent::updateMap() {
    if (bump_) {
        mapa[nextposx][nextposy] = 0;
    }
}

void Agent::updateAct() {
    pastAction = currAction;
}

// -----------------------------------------------------------
// Thinking
// -----------------------------------------------------------

Agent::ActionType Agent::Think_random() {
    return aleatoriza(1,1,1,1,1,0);
}

Agent::ActionType Agent::Think_randomly() {
    return aleatoriza(1,1,1,0,1,0);
}

Agent::ActionType Agent::Think_walls() {
    ActionType accion;
    int prob_forw = 1;
    int prob_left = 1;
    int prob_rght = 1;

    // Evita caminar contra una pared
    if (mapa[nextposx][nextposy] == 0) {
        prob_forw = 0;
    }

    // Evita contravenir la acción anterior
    if (pastAction == actTURN_L)
        prob_rght = 0;

    if (pastAction == actTURN_R)
        prob_left = 0;

    accion = aleatoriza(prob_forw,prob_left,prob_rght,0,1,0);

    // Si al girar va a encontrar una pared, gira al otro lado
    int giraposx = posx;
    int giraposy = posy;
        
    switch (gira(dir,accion)) {
    case up: giraposy--; break;
    case down: giraposy++; break;
    case left: giraposx--; break;
    case right: giraposx++; break;
    }
    
    if (mapa[giraposx][giraposy] == 0) {
        if (accion == actTURN_L)
            accion = actTURN_R;
        else
            accion = actTURN_L;
    }

    return accion;
}

Agent::ActionType Agent::Think() {
    // Number of iterations
    static int iteration = 0;

    // PRECONDITIONS
    update();

    // POSCONDITIONS
    iteration++;
    if (iteration == 2000) draw_map(mapa);


    currAction = Think_random();
#ifdef RANDOMLY
    currAction = Think_randomly();
#endif
#ifdef WALLS
    currAction = Think_walls();
#endif

    return currAction;
}

// -----------------------------------------------------------
//
// -----------------------------------------------------------

void Agent::Perceive(const Environment &env) {
    trufa_size_ = env.TrufaAmount();
    bump_ = env.isJustBump();
}

string ActionStr(Agent::ActionType accion) {
    switch (accion) {
    case Agent::actFORWARD: return "FORWARD";
    case Agent::actTURN_L: return "TURN LEFT";
    case Agent::actTURN_R: return "TURN RIGHT";
    case Agent::actSNIFF: return "SNIFF";
    case Agent::actEXTRACT: return "EXTRACT";
    case Agent::actIDLE: return "IDLE";
    default: return "???";
    }
}
