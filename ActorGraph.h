/* 
 * File: ActorGraph.h
 * Date: December 4, 2017  
 * Purpose: This file is meant to exist as a container for starter code that 
 *    you can use to read the input file format defined in movie_casts.tsv. 
 *    Feel free to modify any/all aspects as you wish. 
 *    Includes declaration of pointer comparisons and function declarations 
 *    of for ActorGraph.cpp
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include "ActorNode.h"
#include "Movie.h"
using namespace std;

/**Pointer comparison between two movies **/
class MoviePtrComp
{
    public:
        bool operator()(Movie*& lhs, Movie*& rhs) const
        {
            return *lhs < *rhs;
        }
};

/**Pointer comparison between two actors **/
class ActorPtrComp
{
    public:
        bool operator()(ActorNode*& lhs, ActorNode*& rhs) const
        {
            return *lhs < *rhs;
        }
};

class ActorGraph {
    protected:
        unordered_map<string, Movie*> movie_map;
        unordered_map<string, ActorNode*> actor_map;
        priority_queue<Movie*, vector<Movie*>, MoviePtrComp> sorted_movies;
        void deleteAll();


    public:
        ActorGraph(void);

        ~ActorGraph();

        bool loadFromFile(const char* in_filename, bool use_weighted_edges);

        int neighborSize(string actor_name);

        void build();

        ActorNode* getActor(string actor_name);

        void resetNodes();

        void useWeights();

        void sortMovies();

        bool buildByYear();

        int curr_year;

        void resetNeighbors();

        bool ufindByYear();
};


#endif // ACTORGRAPH_H
