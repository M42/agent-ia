#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <utility>

using namespace std;

// AUXILIARY FUNCTIONS
void draw_map(Map& map, MapB& expl) {
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            //cerr << map[i][j] << "\t";
            if (expl[i][j])
                cerr << "*";
            else
                cerr << "?";
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

Agent::ActionType Agent::aleatoriza() {
    int pf = prob_forw;
    int pl = prob_left;
    int pr = prob_rght;
    int ps = prob_snif;
    int pe = prob_extr;
    int pi = prob_idle;

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

    leftposx = posx;
    leftposy = posy;

    switch (gira(dir,actTURN_L)) {
    case up: leftposy--; break;
    case down: leftposy++; break;
    case left: leftposx--; break;
    case right: leftposx++; break;
    }

    rghtposx = posx;
    rghtposy = posy;

    switch (gira(dir,actTURN_R)) {
    case up: rghtposy--; break;
    case down: rghtposy++; break;
    case left: rghtposx--; break;
    case right: rghtposx++; break;
    }
}

void Agent::updateMap() {
    // Adds walls to the map
    if (bump_) {
        mapa[nextposx][nextposy] = 0;
        expl[nextposx][nextposy] = true;
    }

    // Probabilistic growth
    for (int i=0; i<20; i++)
        for (int j=0; j<20; j++) 
            if (mapa[i][j] != 0)
                mapa[i][j] += growFactor;

    // Extraction
    if (currAction == actEXTRACT) {
        mapa[posx][posy] = 1;
    }

    // Sniffing
    if (currAction == actSNIFF) {
        mapa[posx][posy] = 1 + trufa_size_ * 1000;
    }

    // Exploration
    if (!expl[posx][posy])
        expl[posx][posy] = true;
}

void Agent::updateAct() {
    pastAction = currAction;
}

// -----------------------------------------------------------
// Analizing
// -----------------------------------------------------------



// -----------------------------------------------------------
// Thinking
// -----------------------------------------------------------

Agent::ActionType Agent::Think_random() {
    prob_forw = 1;
    prob_left = 1;
    prob_rght = 1;
    prob_extr = 1;
    prob_snif = 1;
    prob_idle = 0;

    return aleatoriza();
}

Agent::ActionType Agent::Think_randomly() {
    prob_forw = 1;
    prob_left = 1;
    prob_rght = 1;
    prob_extr = 1;
    prob_snif = 0;
    prob_idle = 0;

    return aleatoriza();
}

Agent::ActionType Agent::Think_walls() {
    ActionType accion;
    prob_forw = 1;
    prob_left = 1;
    prob_rght = 1;
    prob_extr = 1;
    prob_snif = 0;
    prob_idle = 0;

    // Evita caminar contra una pared
    if (mapa[nextposx][nextposy] == 0) {
        prob_forw = 0;
    }

    // Evita contravenir la acción anterior
    if (pastAction == actTURN_L)
        prob_rght = 0;

    if (pastAction == actTURN_R)
        prob_left = 0;

    accion = aleatoriza();

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

Agent::ActionType Agent::Think_map() {
#ifndef SUFICIENTE_TRUFA
#define SUFICIENTE_TRUFA 4000
#endif

    const double FACTOR_GIRO = 1.5;  
    ActionType accion;

    if (mapa[posx][posy] >= SUFICIENTE_TRUFA) {
        accion = actEXTRACT;
    }
    else if (mapa[nextposx][nextposy] != 0 and 
             mapa[nextposx][nextposy]*FACTOR_GIRO >= mapa[leftposx][leftposy] and
             mapa[nextposx][nextposy]*FACTOR_GIRO >= mapa[rghtposx][rghtposy]) {
        accion = actFORWARD;
    }
    else if (mapa[leftposx][leftposy] >= mapa[rghtposx][rghtposy])
        accion = actTURN_L;
    else
        accion = actTURN_R;

    return accion;
}

Agent::ActionType Agent::Think() {
    // Number of iterations
    static int iteration = 0;

    // PRECONDITIONS
    update();

    // POSCONDITIONS
    iteration++;
    if (iteration == 2000) draw_map(mapa, expl);


    currAction = Think_random();
#ifdef RANDOMLY
    currAction = Think_randomly();
#endif
#ifdef WALLS
    currAction = Think_walls();
#endif
#ifdef MAP
    currAction = Think_map();
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
