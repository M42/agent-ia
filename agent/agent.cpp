#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// AUXILIARY FUNCTIONS
typedef vector< vector<bool> > Map;

void draw_map(Map& map) {
    for (int i=0; i<20; i++) {
        for (int j=0; j<20; j++) {
            if (map[i][j])
                cerr << "%";
            else
                cerr << ".";
        }
        cerr << endl;
    }
}

// -----------------------------------------------------------
// Edited Code
// -----------------------------------------------------------

Agent::ActionType Agent::Think() {
    // DEFINITIONS


    // Iconic map, up to relative position
    static Map mapa = Map(20, vector<bool>(20, false));
    static int posx = 0;
    static int posy = 0;

    // Number of iterations
    static int iteration = 0;


    // REAL CODE
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


    // POSCONDITIONS
    iteration++;
    if (iteration == 1000) draw_map(mapa);
    return accion;
}

// -----------------------------------------------------------
//
// -----------------------------------------------------------

void Agent::Perceive(const Environment &env) {
    trufa_size_ = env.TrufaAmount();
    bump_ = env.isJustBump();
}

string ActionStr(Agent::ActionType accion)
{
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
