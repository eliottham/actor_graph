/* 
 * File: Movie.h
 * Date: December 4, 2017  
 * Purpose:  Initializes functions needed for the Movie class 
 *      Implements the movie class that creates a cast of actors that
 *      belong to each movie, creating edges between each pairs of actors,
 *      given weights of movies, and sorts these movies in order by year.
 *      Implements functions for ufind algorithm needed as well*/
#ifndef MOVIE_H
#define MOVIE_H
#include <vector>
#include <string>
#include "ActorNode.h"
using namespace std;

class Movie
{
	private:
		vector<ActorNode*> cast;
		string title;
		int weight;

	public:
        int year;
		Movie(string title, int year);
		void addActor(ActorNode* actor);
		void makeEdges();
		void calcWeight();
        void merge(ActorNode* n1, ActorNode* n2);
        ActorNode* find(ActorNode* node);
        void ufind();

		bool operator<(const Movie& other);
};
#endif
