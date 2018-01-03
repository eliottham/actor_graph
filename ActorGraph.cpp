/* 
 * File: ActorGraph.cpp
 * Date: December 4, 2017  
 * Purpose: This file is meant to exist as a container for starter code that 
 *    you can use to read the input file format defined in movie_casts.tsv. 
 *    Feel free to modify any/all aspects as you wish.  Includes implementation
 *    of helper methods for functionality of ActorGraph
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ActorNode.h"
#include "ActorNode.cpp"
#include "Movie.h"
#include "Movie.cpp"
#include "ActorGraph.h"
using namespace std;

ActorGraph::ActorGraph(void) {} // Constructor

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    ifstream infile(in_filename);  // initialize the file stream

    bool have_header = false;

    while (infile) {  // keep reading lines until the end of file is reached
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // key for movie_map title#@year
        string fmt_title = movie_title + "#@" +  record[2];
        // find the movie object for our key if it exists
        Movie* movie;

        if(movie_map.find(fmt_title) == movie_map.end()) { 
            movie = new Movie(movie_title, movie_year); // create movie object if one does not exist
            movie_map.insert(make_pair(fmt_title, movie)); // add it to the map
        }
        else {
            movie = movie_map[fmt_title];
        }

        // find the actor object if it exists
        ActorNode* actor;

        if(actor_map.find(actor_name) == actor_map.end()) { 
            actor = new ActorNode(actor_name); // create actor object if it does not exist
            actor_map.insert(make_pair(actor_name, actor)); // add it to the map
        }
        else {
            actor = actor_map[actor_name];
        }

        movie->addActor(actor); // add the actor to the cast of the movie
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    if(use_weighted_edges) {
        useWeights();
    }

    return true;
}

/**builds graph by going through the movies and calls makeEdges() to make
 *   connections between the neighbors **/
void ActorGraph::build() {
    for(auto& m: movie_map) {
        m.second->makeEdges();
    }
}

/**to use when using Dijkstra's algorithm.  Method goes through movies and
 *   calls calcWeight to determine the weights of the neighbors **/
void ActorGraph::useWeights() {
    for(auto& m: movie_map) {
        m.second->calcWeight();
    }
}

/**helper method to sort the movies by order of year by using a priority queue
 *   goes through all the movies and adds them to the priority queue **/ 
void ActorGraph::sortMovies() {
    sorted_movies = priority_queue<Movie*, vector<Movie*>, MoviePtrComp> ();

    for(auto& m: movie_map) {
        sorted_movies.push(m.second);
    }
}

/**method to be used by when ufind is called as the algorithm.  Goes through
 *    each movie years by the priority queue, and calls method ufind() */
bool ActorGraph::ufindByYear()
{
    if(sorted_movies.size() == 0) { //the priority queue is empty
        return false;
    }

    curr_year = sorted_movies.top()->year;
    Movie* curr_movie;

    while(1) {
        if(sorted_movies.top()->year != curr_year) break;
        if(sorted_movies.size() == 0) break;

        curr_movie = sorted_movies.top(); // get the next movie
        curr_movie->ufind(); // builds ufind graph
        sorted_movies.pop();
    }
    return true;
}

/** method to go through the priority queue of sorted movies and build the 
 *      graph by each year **/
bool ActorGraph::buildByYear() {
    if(sorted_movies.size() == 0) {  //the priority queue is empty
        return false;
    }

    curr_year = sorted_movies.top()->year;
    Movie* curr_movie;

    while(1) {
        if(sorted_movies.top()->year != curr_year) break;
        if(sorted_movies.size() == 0) break;   
        
        curr_movie = sorted_movies.top(); //get the next movie 
        curr_movie->makeEdges();  //make connections between actors 
        sorted_movies.pop();
    }
    return true;
}

/**method to reinitialize all the neighbors each time you go through a new 
 *      pair of actors to test **/
void ActorGraph::resetNeighbors() {
    for(auto& a: actor_map) {
        a.second->neighbors = vector<ActorNode*> ();
        a.second->edge_map = unordered_map<ActorNode*, pair<string, int> > ();
    }
}

/** helper method to get the size of each neighbor **/
int ActorGraph::neighborSize(string actor_name) {
    ActorNode* actor = actor_map[actor_name];
    return actor->neighborSize();
}

/** helper method to get the actor from the file **/
ActorNode* ActorGraph::getActor(string actor_name) {
    return actor_map[actor_name];
}

/**helper method to reset the nodes in the graph for each time you go through
 *      a new pair of actors to test **/
void ActorGraph::resetNodes() {
    for(auto& a: actor_map) {
      a.second->dist = numeric_limits<int>::max(); //set to infinity
      a.second->prev = nullptr;
      a.second->done = false;
      a.second->parent = nullptr;
      a.second->size = 1;
    }
}

/**method to delete all the nodes at the end **/
void ActorGraph::deleteAll() {
    for(auto& m: movie_map) {
        delete m.second;
    }

    for(auto& a: actor_map) {
        delete a.second;
    }
}

ActorGraph::~ActorGraph() {
    deleteAll();
}