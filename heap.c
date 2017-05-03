// heap.c is based on the version we used in course Algorithms and Data Structures

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int isEmptyHeap (Fringe h) {
	return (h.front == 1);
}

void heapEmptyError() {
	printf("heap empty\n");
	abort();
}

void doubleHeapSize (Fringe *hp) {
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
	//printf ("enqueueing position with %c\n", n.type);
	int fr = fringe->front;
	if ( fr == fringe->size ) {
		doubleHeapSize(fringe);
	}
	fringe->states[fr] = n;
	upheap(fringe,fr);
	//printf("the first position is %d %d\n", fringe->states[1].col, fringe->states[1].row);
	fringe->front++;
}

void upheap(Fringe *fringe, int n){
	//printf("upheap started\n");
	if (n<=1){return;}
	if ( fringe->states[n/2].value>fringe->states[n].value ) {
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
	if ( fringe->states[n].value > fringe->states[2*n].value ) {
		indexMax = 2*n;
	}
	if ( fr > 2*n+1 && fringe->states[indexMax].value > fringe->states[2*n+1].value ) {
		indexMax = 2*n+1;
	}
	if ( indexMax != n ) {
		swap(&(fringe->states[n]),&(fringe->states[indexMax]));
		downheap(fringe,indexMax);
	}
}

State dequeue(Fringe *fringe) {
	State n;
	if ( isEmptyHeap(*(fringe) ) {
		heapEmptyError();
	}
	n = fringe->states[1];
	fringe->front--;
	fringe->states[1] = fringe->states[fringe->front];
	downheap(hp,1);
return n;
}
