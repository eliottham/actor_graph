/* 
 * File: ActorNode.h
 * Date: December 4, 2017  
 * Purpose: This file defines all the methods used to define the actor nodes  
 *      that implements actors and assigns their neighbors, size,
 *      and sorts them by distance in a priority queue */

#ifndef ACTORNODE_H
#define ACTORNODE_H
#include <string>
#include <vector>
#include <limits.h>
#include <unordered_map>
using namespace std;

class ActorNode
{
	private:

	public:
		ActorNode(string name);
		vector<ActorNode*> neighbors;
		int dist;
		ActorNode* prev;
		bool done;
		void addNeighbor(ActorNode* actor, string fmt_title, int weight);
		int neighborSize();
		unordered_map<ActorNode*, pair<string, int>> edge_map;
		string name;
        int size;
        ActorNode* parent;

        bool operator<(const ActorNode& other);
};
#endif
