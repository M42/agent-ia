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


Agent::Direction Agent::updateDir(Agent::Direction dir, Agent::ActionType action) {
    if (action == actTURN_L)
        switch (dir) {
        case up:    return left;  break;
        case left:  return down;  break;
        case down:  return right; break;
        case right: return up;    break;
        }
    if (action == actTURN_R)
        switch (dir) {
        case up:    return right; break;
        case right: return down;  break;
        case down:  return left;  break;
        case left:  return up;    break;
        }

    return dir;
}

void Agent::updatePos(Agent::Direction dir) {
    switch (dir) {
    case up: posy--; break;
    case down: posy++; break;
    case left: posx--; break;
    case right: posx++; break;
    }
}

void Agent::updateMap() {
    if (bump_) {
        int posbumpx = posx;
        int posbumpy = posy;

        switch (dir) {
        case up: posbumpy--; break;
        case down: posbumpy++; break;
        case left: posbumpx--; break;
        case right: posbumpx++; break;
        }

        mapa[posbumpx][posbumpy] = true;
    }
}

// -----------------------------------------------------------
// Edited Code
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

Agent::ActionType Agent::Think_walls() {
    ActionType accion;
    int posbumpx = posx;
    int posbumpy = posy;

    switch (dir) {
    case up: posbumpy--; break;
    case down: posbumpy++; break;
    case left: posbumpx--; break;
    case right: posbumpx++; break;
    }

    if (mapa[posbumpx][posbumpy] == true) {
        switch(rand()%2){
        case 0: accion=actTURN_L;
            break;
        case 1: accion=actTURN_R;
            break;
        }
    }
    else {
        accion = Think_random();
    }

    return accion;
}

Agent::ActionType Agent::Think() {
    // Number of iterations
    static int iteration = 0;

    // PRECONDITIONS
    updateMap();
    if ((currAction == actFORWARD) and (not bump_))
        updatePos(dir);
    dir = updateDir(dir,currAction);

    // POSCONDITIONS
    iteration++;
    if (iteration == 1000) draw_map(mapa);

    currAction = Think_walls();

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
