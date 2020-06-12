#include <stdlib.h>
#include <PriorityQueue.h> //https://github.com/CollinDietz/PriorityQueue

#include "Pathfinding.h"
#include "Data.h"

typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char dist;
} Node;

inline unsigned char isqrt(int32_t n) {
    int32_t r = n;
    while(r*r > n) {
        r += n/r;
        r /= 2;
    }
    return r;
}

inline int32_t sqr(int32_t x) {
    return x*x;
}

unsigned char distance(Node n, int32_t goalx, int32_t goaly) {
    int32_t nx = n.x;
    int32_t ny = n.y;
    return isqrt(sqr(nx-goalx) + sqr(ny-goaly));
}

bool compare(Node a, Node b) {
    return a.dist < b.dist;
}

// Warning: This function returns nodes but doesn't set the dist value
Node* getNeighbours(Node node, int* nbFound) {

    // Allocates space for 8 neighbours even if less are found to save time
    Node* res = (Node*) malloc(sizeof(Node)*8);

    int arrayindex = 0;
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {

            if(x == 0 && y == 0) continue;

            unsigned char nx = node.x + x;
            unsigned char ny = node.y + y;

            if(! inMatrixBounds(lowResMap, nx, ny)) continue;
            if(getMatrixValue(lowResMap, nx, ny) == 255) continue;

            (res+arrayindex)->x = nx;
            (res+arrayindex)->y = ny;

            arrayindex++;
        }
    }

    *nbFound = arrayindex;
    return res;
}

void findPath() {

    int posx = (position->x)/3;
    int posy = (position->y)/3;
    int tarx = (target->x)/3;
    int tary = (target->y)/3;
    needsPathUpdate = 0;

    // Resets the pixels that are not walls in map (sets them to 254)
    for(int y = 0; y < LOWRES_SIZE; y++)
        for(int x = 0; x < LOWRES_SIZE; x++)
            if(getMatrixValue(lowResMap, x, y) != 255)
                setMatrixValue(lowResMap, x, y, 254);

    // This queue contains the nodes that will be processed by priority order
    // (distance to the start, this version of the algorithm starts at the end)
    PriorityQueue<Node> queue = PriorityQueue<Node>(compare);
    Node end = {(unsigned char)tarx, (unsigned char)tary, 0};
    setMatrixValue(lowResMap, tarx, tary, 0);
    queue.push(end);

    // Main loop
    while (! queue.isEmpty()) {

        Node current = queue.pop();
        unsigned char currentScore = getMatrixValue(lowResMap, current.x, current.y);

        // A path has been found
        if(current.x == posx && current.y == posy)
            return;

        // Processes neighbours of the current node
        int nbFound;
        Node* neighbours = getNeighbours(current, &nbFound);
        for(int i = 0; i < nbFound; i++) {

            unsigned char tentative = currentScore+1;
            unsigned char nx = (neighbours+i)->x;
            unsigned char ny = (neighbours+i)->y;

            // New best path to this node has been found
            if(tentative < getMatrixValue(lowResMap, nx, ny)) {

                setMatrixValue(lowResMap, nx, ny, tentative);
                (neighbours+i)->dist = tentative + distance(neighbours[i], posx, posy);

                // If the node is not in the queue yet, adds it
                if(true) // TODO: This would greatly optimise
                    queue.push(neighbours[i]);
            }
        }
        free(neighbours);
    }
    
    Serial.println("NO PATH FOUND");
}
