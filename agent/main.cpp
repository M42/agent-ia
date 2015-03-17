#include "GUI.h"

int main(int argc, char *argv[]){
	return GUI::startDraw(argc,argv);
}

#include <pthread.h>
void junk() {
    int i;
    i=pthread_getconcurrency();
};
