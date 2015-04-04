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
    if (currAction == actTURN_L)
        switch (dir) {
        case up:    dir = left;  break;
        case left:  dir = down;  break;
        case down:  dir = right; break;
        case right: dir = up;    break;
        }
    if (currAction == actTURN_R)
        switch (dir) {
        case up:    dir = right; break;
        case right: dir = down;  break;
        case down:  dir = left;  break;
        case left:  dir = up;    break;
        }
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

Agent::ActionType Agent::Think_walls() {
    ActionType accion;

    if (mapa[nextposx][nextposy] == true) {
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
    update();

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
