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
            if (map[i][j])
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


//////////////////////////////
// Reacting
//////////////////////////////

void Agent::update() {
    // Those functions do not commute!
    updateDir();
    updateMap();
    updatePos();
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
        mapa[nextposx][nextposy] = true;
    }
}

// -----------------------------------------------------------
// Thinking
// -----------------------------------------------------------

Agent::ActionType Agent::Think_random() {
    ActionType accion;

    switch(rand()%5){
    case 0: accion=actFORWARD;
        break;
    case 1: accion=actTURN_L;
        break;
    case 2: accion=actTURN_R;
        break;
    case 3: accion=actSNIFF;
        break;
    case 4: accion=actEXTRACT;
        break;
    }
    
    return accion;
}

Agent::ActionType Agent::Think_randomly() {
    ActionType accion;

    switch(rand()%4){
    case 0: accion=actFORWARD;
        break;
    case 1: accion=actTURN_L;
        break;
    case 2: accion=actTURN_R;
        break;
    case 3: accion=actEXTRACT;
        break;
    }
    
    return accion;
}

Agent::ActionType Agent::Think_walls() {
    ActionType accion;

    // Evita caminar contra una pared
    if (mapa[nextposx][nextposy] == true) {
        // Gira hacia donde no haya paredes.
        // Prueba hacia un sentido, y, si hay una pared, usa el contrario.
        switch(rand()%2) {
        case 0: accion=actTURN_L; break;
        case 1: accion=actTURN_R; break;
        }
    }
    else {
        accion = Think_randomly();
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
    if (iteration == 1000) draw_map(mapa);


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
