//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

// MinHeap structure for managing Huffman tree node indices by weight
struct MinHeap {
    int data[64];  // Stores indices of nodes in the Huffman tree
    int size;      // Current number of elements in the heap

    // Constructor initializes heap size to 0
    MinHeap() { size = 0; }

    // Insert a node index into the heap and restore heap order
    void push(int idx, int weightArr[]) {
        data[size] = idx;                 // Place new index at the end
        upheap(size, weightArr);         // Restore heap property upward
        size++;                          // Increase heap size
    }

    // Remove and return the index of the node with the smallest weight
    int pop(int weightArr[]) {
        if (size == 0) {
            return -1;                   // Heap is empty
        }
        int minIdx = data[0];            // Root holds smallest index
        size--;                          // Reduce heap size
        data[0] = data[size];            // Move last element to root
        downheap(0, weightArr);          // Restore heap property downward
        return minIdx;                   // Return smallest index
    }

    // Restore heap property by moving a node upward
    void upheap(int pos, int weightArr[]) {
        while (pos > 0) {
            int parent = (pos - 1) / 2;
            // If child is smaller than parent, swap them
            if (weightArr[data[pos]] < weightArr[data[parent]]) {
                swap(data[pos], data[parent]);
                pos = parent;            // Continue upheap from new position
            } else {
                break;                   // Heap property satisfied
            }
        }
    }

    // Restore heap property by moving a node downward
    void downheap(int pos, int weightArr[]) {
        while (true) {
            int left = 2 * pos + 1;      // Left child index
            int right = 2 * pos + 2;     // Right child index
            int smallest = pos;          // Assume current is smallest

            // Compare with left child
            if (left < size && weightArr[data[left]] < weightArr[data[smallest]]) {
                smallest = left;
            }
            // Compare with right child
            if (right < size && weightArr[data[right]] < weightArr[data[smallest]]) {
                smallest = right;
            }

            // If a child is smaller, swap and continue downheap
            if (smallest != pos) {
                swap(data[pos], data[smallest]);
                pos = smallest;
            } else {
                break;                   // Heap property satisfied
            }
        }
    }
};

#endif