#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "fringe.h"
///////////////////////////////////////////operation functions/////////////////////////
Operation newOp(int op, int value){
	Operation o;
	o.value = value;
	o.op = op;
	return o;
}
///////////////////////////////////////////heap functions/////////////////////////

int isEmptyHeap (Fringe h) {
	return (h.front == 1);
}

void heapEmptyError() {
	printf("heap empty\n");
	abort();
}

void doubleHeapSize (Fringe *fringe) {
	//printf("doubleing heap size\n");
	int newSize = 2 * fringe->size;
	fringe->states = realloc(fringe->states, newSize * sizeof(State));
	assert(fringe->states != NULL);
	fringe->size = newSize;
}

void swap(State *pa, State *pb) {
	State h = *pa;
	*pa = *pb;
	*pb = h;
}

void enqueue (State n, Fringe *fringe) {
	//printf ("enqueueing position\n");
	int fr = fringe->front;
	if ( fr >= 1 +  fringe->size ) { 		//size is only increased after enqueing
		doubleHeapSize(fringe);
	}
	fringe->states[fr] = n;
	upheap(fringe,fr);
	fringe->front++;
}

void upheap(Fringe *fringe, int n){
	//printf("upheap started\n");
	if (n<=1){return;}
	if ( fringe->states[n/2].cost>fringe->states[n].cost ) {
		swap(&(fringe->states[n]),&(fringe->states[n/2]));
		upheap(fringe,n/2);
	}
}

void downheap (Fringe *fringe, int n) {
	//printf("downheap started\n");
	int fr = fringe->front;
	int indexMax = n;
	if ( fr < 2*n+1 ) { /* node n is a leaf, so nothing to do */
		return;
	}
	if ( fringe->states[n].cost > fringe->states[2*n].cost ) {
		indexMax = 2*n;
	}
	if ( fr > 2*n+1 && fringe->states[indexMax].cost > fringe->states[2*n+1].cost ) {
		indexMax = 2*n+1;
	}
	if ( indexMax != n ) {
		swap(&(fringe->states[n]),&(fringe->states[indexMax]));
		downheap(fringe,indexMax);
	}
}

State dequeue(Fringe *fringe) {
	//printf("dequeue started\n");
	State n;
	if ( isEmptyHeap(*(fringe) ) ){
		//heapEmptyError();
		/*This value is used to message that this state should not be freed*/
		n.value = -1;
		return n;
	}
	n = fringe->states[1];
	fringe->front--;
	fringe->states[1] = fringe->states[fringe->front];
	downheap(fringe,1);
	return n;
}

void printHeap(Fringe fringe){
	printf("heap is :");
	for (int i = 0; i< fringe.size; i++){
		printf("%d ", fringe.states[i]);
	}
	printf("\n");
}

/////////////////////////////////Fringe functions////////////////////////////////////////////////

Fringe makeFringe(int mode) {
  /* Returns an empty fringe. 
   * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP) 
   */
  Fringe f;
  if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
      (mode != PRIO) && (mode != HEAP) && (mode != IDS)) {
    fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
    fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
    exit(EXIT_FAILURE);
  }
  f.mode = mode;
  f.size = f.front = f.rear = 0; /* front+rear only used in FIFO mode */
  f.states = malloc(MAXF*sizeof(State));
  if (mode == HEAP||mode == PRIO){
	  f.size = f.front = f.rear = 1;
	  f.states[0].path = malloc(sizeof(Operation));
	  } 						// heap index starts from 1, easier to calculate, allocated memory for easy free
  if (f.states == NULL) {
	fprintf(stderr, "makeFringe(): memory allocation failed.\n");
    exit(EXIT_FAILURE);      
  }
  f.maxSize = f.insertCnt = f.deleteCnt = 0;
  return f;
}



void deallocFringe(Fringe fringe) {
  State state;
  /* Frees the memory allocated for the fringe */
  while(!isEmptyFringe(fringe)){
	fringe = removeFringe(fringe, &state);
	if(state.value != -1){
		free(state.path);
	}
  }
  if(fringe.mode == HEAP | fringe.mode == PRIO){free(fringe.states[0].path);}
  free(fringe.states);
}
void resetFringe(Fringe fringe){
	deallocFringe(fringe);
	fringe.states = malloc(MAXF*sizeof(State));
	fringe.front = fringe.rear = fringe.size = 0;
}

int getFringeSize(Fringe fringe) {
  /* Returns the number of elements in the fringe 
   */
  return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
  /* Returns 1 if the fringe is empty, otherwise 0 */
  return (fringe.size == 0 ? 1 : 0);
}

Fringe insertFringe(Fringe fringe, State s, ...) {
  /* Inserts s in the fringe, and returns the new fringe.
   * This function needs a third parameter in PRIO(HEAP) mode.
   */
  // printf("%d\n", s.value);
  //int priority;
  //va_list argument;

  if (fringe.size == MAXF) {
    fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.insertCnt++;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
  case IDS:
    fringe.states[fringe.size] = s;
    break;
  case FIFO:
    fringe.states[fringe.rear++] = s;
    fringe.rear %= MAXF;
    break;
  case HEAP:
  case PRIO: //using a heap for priority queue
	enqueue(s, &fringe);
	break;
  }

  fringe.size++;
  if (fringe.size > fringe.maxSize) {
    fringe.maxSize = fringe.size;
  }
  return fringe;
}

Fringe removeFringe(Fringe fringe, State *s) {
  /* Removes an element from the fringe, and returns it in s. 
   * Moreover, the new fringe is returned.
   */
   //printf ("removing\n");
  if (fringe.size < 1) {
    fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.deleteCnt++;
  fringe.size--;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
  case IDS:
    *s = fringe.states[fringe.size];
    break;
  case FIFO:
    *s = fringe.states[fringe.front++];
    fringe.front %= MAXF;
    break;
  case HEAP:
  case PRIO: //for priotity queue implementation we use a heap
	*s = dequeue(&fringe);
	break;
  }
  return fringe;
}

void showStats(Fringe fringe) {
  /* Shows fringe statistics */
  printf("#### fringe statistics:\n");
  printf(" #size        : %7d\n", fringe.size);
  printf(" #maximum size: %7d\n", fringe.maxSize);
  printf(" #insertions  : %7d\n", fringe.insertCnt);
  printf(" #deletions   : %7d\n", fringe.deleteCnt);
  printf("####\n"); 
}
