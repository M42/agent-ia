#include<fstream>
#include<sstream>
#include<iomanip>
#include<iostream>
#include <unistd.h>
using namespace std;
#include<stdlib.h>
#include"random_num_gen.h"
#include"environment.h"
#include"agent.h"
#include"evaluator.h"
#include"string.h"
#include<string>

const int NEWGAME_BTN_ID=102,SELECTMAP_BTN_ID=103,DISPLAY_BTN_ID=104,NEWMAP_BTN_ID=105;
const int NEWMAP_OK_BTN_ID=202,NEWMAP_CANCEL_BTN_ID=203;
const int SELECTMAP_SELECT_ID=302,SELECTMAP_CANCEL_BTN_ID=303;
const int DO_ONE_STEP_BTN_ID=402,DO_ONE_RUN_BTN_ID=403,NEXT_RUN_BTN_ID=404,DO_ALL_RUN_BTN_ID=405;
const int ERROR_OK_BTN_ID=502;

string file_name_input="agent.map";
char life_time_input[1000]="2000",total_runs_input[1000]="10";
char display_step[20]="100",display_time[20]="10";
char life_time_temp[1000],total_runs_temp[1000],file_name_temp[1000];
int tx,ty,tw,th;
float magnification_x,magnification_y;
long long life_time=2000,current_time,total_runs=10,current_run,Trufa_Collected,consumed_energy;
long long total_collect,total_consumed_energy;
char time_step_msg[100],action_msg[100],Trufa_Collected_msg[100],consumed_energy_msg[100];

Environment *env=NULL;
Agent *agent=NULL;
RandomNumberGenerator *rng=NULL;
Evaluator *evaluator=NULL;
Agent::ActionType action=Agent::actIDLE;
ifstream fin;


void doOneStep();
void doOneRun();
void doAllRun();
void nextRun();
void newGame();


void doOneStep(){
    if(current_time<life_time){
        env->Change(*rng);
        agent->Perceive(*env);

        action = agent->Think();
        env->AcceptAction(action);
        evaluator->Eval(action,*env);

        Trufa_Collected=env->TrufaCollected();
        consumed_energy=evaluator->ConsumedEnergy();

        ++current_time;
        if(current_time==life_time){//¤@½üµ²§ô
            total_collect+=Trufa_Collected;
            total_consumed_energy+=consumed_energy;
        }
    }
}

void doOneRun(){
    while(current_time<life_time){
        doOneStep();
    }
}

void doAllRun(){
    doOneRun();
    while(current_run<total_runs){
        nextRun();
        doOneRun();
    }
}

void nextRun(){
    if(current_run<total_runs){
        ++current_run;
        newGame();
    }
}

void newGame(){
    fin.close();
    fin.clear();
    delete env;
    delete agent;
    delete rng;
    delete evaluator;

    fin.open(file_name_input.c_str());
    if(fin){
        current_time=0;
        Trufa_Collected=0;
        consumed_energy=0;
        env=new Environment(fin);
        rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
        evaluator=new Evaluator();
        agent=new Agent();
    }
    else{
        fin.clear();
        char temp[1000]={"map/"};
        strcat(temp,file_name_input.c_str());
        fin.open(temp);
        if(fin){
            current_time=0;
            Trufa_Collected=0;
            consumed_energy=0;
            env=new Environment(fin);
            rng=new RandomNumberGenerator(env->RandomSeed()+current_run);
            evaluator=new Evaluator();
            agent=new Agent();
        }
        else{
        }
    }
}


int main(int argc, char* argv[]){
    file_name_input = argv[1];
    long long trufa_total;

    for (int i=0; i<10; i++) {
        newGame();
        doOneRun();
        trufa_total += Trufa_Collected;
    }

    cout << trufa_total/10.0 << endl;
}

