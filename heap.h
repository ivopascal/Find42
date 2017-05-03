// heap.h is based on the version we used in course Algorithms and Data Structures

#ifndef HEAP_H
#define HEAP_H
int isEmptyHeap (Fringe h) ;
void heapEmptyError() ;
void doubleHeapSize (Fringe *fringe) ;
void swap(State *pa, State *pb) ;
void enqueue (State n, Fringe *fringe) ;
void upheap(Fringe *fringe, int n);
void downheap (Fringe *fringe, int n) ;
State dequeue(Fringe *fringe) ;
#endif
