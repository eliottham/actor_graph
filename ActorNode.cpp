/* 
 * File: ActorNode.cpp
 * Date: December 4, 2017  
 * Purpose: This file implements actors and assigns their neighbors, size,
 *      and sorts them by distance in a priority queue */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ActorNode.h"

using namespace std;

// Constructor
ActorNode::ActorNode(string name) : name(name), dist(numeric_limits<int>::max()),
                                    prev(nullptr), done(false), size(1), parent(nullptr) {}

/* Adds ActorNodes from direct edges into a vector of "neighbors". Uses that
 * object as a key for an unordered_map (edge_map) which gives the edge that
 * connects the two actors (formatted movie title) and the weight of the movie
 * that connects them.
 */
void ActorNode::addNeighbor(ActorNode* actor, string fmt_title, int weight){
	if(find(neighbors.begin(), neighbors.end(), actor) == neighbors.end()) {
		neighbors.push_back(actor);
		edge_map[actor] = make_pair(fmt_title, weight); // title#@year
	}
    
    // If a new movie connects the two actors with a lower weight, store that instead.
    if(weight < edge_map[actor].second) {
        edge_map[actor] = make_pair(fmt_title, weight);
    }
}

// Returns how many neighbors a given node has.
int ActorNode::neighborSize() {
	return neighbors.size();
}

// Overloaded operator to sort ActorNode's by distance in a priority queue.
bool ActorNode::operator<(const ActorNode& other){
    return this->dist > other.dist;
}
