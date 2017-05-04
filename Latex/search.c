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
			value -= 1;
			return value;
		case 2:
			value *= 2;
			return value;
		case 3:
			value *= 3;
			return value;
		case 4:
			value /= 2;
			return value;
		case 5:
			value /=3;
			return value;
	}
}
char *operationID(int cases){
	char *stringOperator;
	switch (cases){
		case 0:
			stringOperator = "+1";
			return stringOperator;
		case 1:
			stringOperator = "-1";
			return stringOperator;
		case 2:
			stringOperator = "*2";
			return stringOperator;
		case 3:
			stringOperator = "*3";
			return stringOperator;
		case 4:
			stringOperator = "/2";
			return stringOperator;
		case 5:
			stringOperator = "/3";
			return stringOperator;
	}
}

void updateState(State *s, State old, int cases, int value){
  s->cost = old.cost + (cases / 2) + 1;
  s->pathlen = old.pathlen + 1;
  s->depth = old.depth + 1;
  s->value = value;
  s->path = malloc((old.pathlen + 1) * sizeof(Operation));
  for (int i = 0; i<old.pathlen; i++){
	s->path[i] = old.path[i]; 
	}
  s->path[old.pathlen] = newOp(cases, value);
}

void printPath(int start, State s){
	printf("%d ", start);
	for (int i = 0; i< s.pathlen ; i++){
		printf(" (%s)->%d ", operationID(s.path[i].op), s.path[i].value);
	}
	printf("\nlength = %d, cost = %d\n", s.pathlen, s.cost);
}

Fringe insertValidSucc(Fringe fringe, int value, State old, int cases) {
  State s;

  if ((value <= 0) || (value > RANGE)) {
    /* ignore states that are out of bounds */
    return fringe;
  }
  updateState(&s, old, cases, value);
  return insertFringe(fringe, s);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value;
  int limit = 9999999;					//set limit to infinity for non-IDS modes
  if (mode == IDS){limit = 1;}
  fringe = makeFringe(mode);

  
  do{									//Extra loop for the iterative deepening, in other mode only 1 iteration
	  //printf("limit is %d\n", limit);
	  state.value = start;
	  state.depth = 0;
	  state.pathlen = 0;
	  state.cost = 0;
	    state.path = malloc(sizeof(Operation));
		fringe = insertFringe(fringe, state);
	  while (!isEmptyFringe(fringe)) {
		/* get a state from the fringe */
		fringe = removeFringe(fringe, &state);
		visited++;
		/* is state the goal? */
		value = state.value;
		int valNew;
		/* insert neighbouring states */
		for (int cases = 0; cases < 6; cases++){
			valNew = newValue(value, cases);
			if (state.depth <= limit){	//in non-IDS this is always true	
				if (valNew == goal){
					goalReached = 1;
					State printState;
					updateState(&printState , state, cases, valNew);
					printPath(start, printState);
					free(state.path);
					free(printState.path);
					break;
				}
				fringe = insertValidSucc(fringe, valNew, state, cases);
			}
		}
		if (goalReached){break;}
		free(state.path);
	  }
	limit++;
	}while (mode == IDS&&!goalReached);
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
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP|IDS> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  } else if (strcmp(argv[1], "IDS") == 0)
	fringetype = IDS;
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP|IDS> [start] [goal]\n", argv[0]);
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
