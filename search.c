#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fringe.h"
#define RANGE 1000000

int newValue (int value, int cases){
	switch (cases){
		case 0:
			value += 1;
			return value;
		case 1:
			value *= 2;
			return value;
		case 2:
			value *= 3;
			return value;
		case 3:
			value -= 1;
			return value;
		case 4:
			value /= 2;
			return value;
		case 5:
			value /=3;
			return value;
	}
}

Fringe insertValidSucc(Fringe fringe, int value) {
  State s;
  if ((value <= 0) || (value > RANGE)) {
    /* ignore states that are out of bounds */
    return fringe;
  }
  s.value = value;
  return insertFringe(fringe, s);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value;

  fringe = makeFringe(mode);
  state.value = start;
  fringe = insertFringe(fringe, state);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    
    int valNew;
    
    /* insert neighbouring states */
    int cases;
    for (cases = 0; cases < 6; cases++){
		valNew = newValue(value, cases);
		if (valNew == goal){
			goalReached = 1;
			break;
		}
		fringe = insertValidSucc(fringe, valNew);
	}
	if (goalReached){break;} 
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);  
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);
  search(fringetype, start, goal); 
  return EXIT_SUCCESS;
}
