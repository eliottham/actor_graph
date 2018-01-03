/*
 * File: Movie.cpp
 * Date: December 4, 2017  
 * Purpose:  Implements the movie class that creates a cast of actors that
 *      belong to each movie, creating edges between each pairs of actors,
 *      given weights of movies, and sorts these movies in order by year.
 *      Implements functions for ufind algorithm needed as well */

#include <iostream>
#include <string>
#include <vector>
#include "Movie.h"
#include "ActorNode.h"
using namespace std;

// Constructor
Movie::Movie(string title, int year) : title(title), year(year), weight(1) {}

// Add ActorNode pointers to the movie's cast.
void Movie::addActor(ActorNode* actor) {
	cast.push_back(actor);
}

/* Create edges between each pair of actors in the cast.
 * The first argument is the other end of the edge (other actor)while the second
 * and third are the formatted movie title (printed edge) and movie weight.
 */
void Movie::makeEdges() {
	for(unsigned int i = 0; i < cast.size() - 1; i++) {
		for(unsigned int j = i + 1; j < cast.size(); j++) {
			cast[i]->addNeighbor(cast[j], title + "#@" + to_string(year), weight);
			cast[j]->addNeighbor(cast[i], title + "#@" + to_string(year), weight);
		}
	}
}

// Caluclate the weight of the movie.
void Movie::calcWeight() {
	weight = 1 + (2015 - year);
}

// Overloaded operator used to sort Movie's by year in priority queue.
bool Movie::operator<(const Movie& other) {
	return this->year > other.year;
}

// Find sentinel node of a specified node.
ActorNode* Movie::find(ActorNode* node){
    ActorNode* curr;
    curr = node;
    
    while(curr->parent != nullptr) { //traverse up
        curr = curr->parent;
    }
    return curr;
}

/* Merge two disjoint sets by making the sentinel node of one set the parent of 
 * the other based on size.
 */
void Movie::merge(ActorNode* n1, ActorNode* n2) {
    ActorNode* sent1;
    ActorNode* sent2;

    sent1 = find(n1);
    sent2 = find(n2);

    if(sent1 == sent2) return;  // have been merged

    if(sent1->size > sent2->size) { //merge to the larger tree
        sent2->parent = sent1;
        sent1->size += sent2->size;
    }
    else {
        sent1->parent = sent2;
        sent2->size += sent1->size;
    }
}

// Perform union find on all ActorNodes in Movie's cast.
void Movie::ufind(){
    for(unsigned int i = 0; i < cast.size() - 1; i++) {
        for(unsigned int j = i + 1; j < cast.size(); j++) {
            merge(cast[i], cast[j]);
        }
    }
}
