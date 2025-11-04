#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "heap.h"
using namespace std;

// Maximum number of nodes allowed in the Huffman tree
const int MAX_NODES = 64;

// Arrays to store Huffman tree node data
int weightArr[MAX_NODES];   // Frequency or combined weight of each node
int leftArr[MAX_NODES];     // Index of left child (-1 if none)
int rightArr[MAX_NODES];    // Index of right child (-1 if none)
char charArr[MAX_NODES];    // Character stored in leaf nodes

// Function declarations
void buildFrequencyTable(int freq[], const string& filename);
int createLeafNodes(int freq[]);
int buildEncodingTree(int nextFree);
void generateCodes(int root, string codes[]);
void encodeMessage(const string& filename, string codes[]);

int main() {
    int freq[26] = {0};  // Frequency table for lowercase letters a–z

    // Step 1: Count character frequencies from input file
    buildFrequencyTable(freq, "input.txt");

    // Step 2: Create leaf nodes for characters with non-zero frequency
    int nextFree = createLeafNodes(freq);

    // Step 3: Build Huffman encoding tree using a min heap
    int root = buildEncodingTree(nextFree);

    // Step 4: Traverse tree to generate binary codes for each character
    string codes[26];  // Stores Huffman codes for a–z
    generateCodes(root, codes);

    // Step 5: Encode the input message and display results
    encodeMessage("input.txt", codes);

    return 0;
}

// Step 1: Read file and count frequencies of lowercase letters
void buildFrequencyTable(int freq[], const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    char ch;
    while (file.get(ch)) {
        // Convert uppercase to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Count only lowercase letters
        if (ch >= 'a' && ch <= 'z')
            freq[ch - 'a']++;
    }
    file.close();

    cout << "Frequency table built successfully.\n";
}

// Step 2: Create leaf nodes for each character with non-zero frequency
int createLeafNodes(int freq[]) {
    int nextFree = 0;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            charArr[nextFree] = 'a' + i;       // Store character
            weightArr[nextFree] = freq[i];     // Store frequency
            leftArr[nextFree] = -1;            // No children yet
            rightArr[nextFree] = -1;
            nextFree++;
        }
    }
    cout << "Created " << nextFree << " leaf nodes.\n";
    return nextFree;
}

// Step 3: Build Huffman encoding tree using heap operations
int buildEncodingTree(int nextFree) {
    MinHeap heap;

    // Push all leaf node indices into the heap
    for (int i = 0; i < nextFree; ++i) {
        heap.push(i, weightArr);
    }

    // Combine nodes until only one remains (the root)
    while (heap.size > 1) {
        int i1 = heap.pop(weightArr);  // Smallest node
        int i2 = heap.pop(weightArr);  // Second smallest

        // Create new parent node
        weightArr[nextFree] = weightArr[i1] + weightArr[i2];
        leftArr[nextFree] = i1;
        rightArr[nextFree] = i2;

        heap.push(nextFree, weightArr);  // Push new node into heap
        nextFree++;
    }

    // Return index of root node
    return heap.pop(weightArr);
}

// Step 4: Traverse tree using DFS to generate Huffman codes
void generateCodes(int root, string codes[]) {
    stack<pair<int, string>> s;
    s.push({root, ""});  // Start from root with empty code

    while (!s.empty()) {
        auto [nodeIndex, code] = s.top();
        s.pop();

        // If leaf node, store code for character
        if (leftArr[nodeIndex] == -1 && rightArr[nodeIndex] == -1) {
            char c = charArr[nodeIndex];
            if (c >= 'a' && c <= 'z') {
                codes[c - 'a'] = code;
            }
        } else {
            // Traverse right child (adds '1')
            if (rightArr[nodeIndex] != -1) {
                s.push({rightArr[nodeIndex], code + "1"});
            }
            // Traverse left child (adds '0')
            if (leftArr[nodeIndex] != -1) {
                s.push({leftArr[nodeIndex], code + "0"});
            }
        }
    }
}

// Step 5: Print Huffman codes and encode the input message
void encodeMessage(const string& filename, string codes[]) {
    // Print character-to-code mapping
    cout << "\nCharacter : Code\n";
    for (int i = 0; i < 26; ++i) {
        if (!codes[i].empty())
            cout << char('a' + i) << " : " << codes[i] << "\n";
    }

    // Print encoded message
    cout << "\nEncoded message:\n";

    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        // Normalize to lowercase
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';

        // Output Huffman code for each character
        if (ch >= 'a' && ch <= 'z')
            cout << codes[ch - 'a'];
    }
    cout << "\n";
    file.close();
}