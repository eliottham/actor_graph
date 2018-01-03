/* 
 * Date: December 4, 2017 
 * File: pathfinder.cpp
 *     Purpose: To find the shortest path between a pair of actors.
 *     -> 4 command arguments :
 *      (1) Name of text file containing the movie casts
 *      (2) u or w (unweighted or weighted path)
 *      (3) Name of text file containing actors to find the paths.
 *      (4) Name of output file
 */
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include "ActorNode.h"
#include "Movie.h"
#include "ActorGraph.h"
#include "ActorGraph.cpp"
using namespace std;

int main(int argc, char* argv[]) {
    ifstream in1(argv[1]);
    ifstream in3(argv[3]);
    ifstream in4(argv[4]);
    string typeOfWeight = argv[2];

    /** Null checks **/
    //Check if not 4 arguments after calling ./pathfinder
    if(argc != 5) {
        cerr << "Invalid amount of arguments" << endl;
        cerr << "Example: ./pathfinder movie_casts.tsv u test_pairs.tsv"  
             << "out_paths_unweighted.tsv" << endl;
        return -1;
    }

    //Check if argv[2] is not u or w
    if(typeOfWeight != "w"  && typeOfWeight!= "u") {
        cerr << "argument needs to be either u or w" << endl;
        return -1;
    }

    //Check if argv[1] is invalid
    if(!in1.good()) {
        cerr << "argv[1]File does not exist" << endl;
        return -1;
    }
    if(in1.peek() == ifstream::traits_type::eof()) {
        cout << "argv[1] File is empty" << endl;
        return -1;
    }

    //Check if argv[3] is invalid
    if(!in3.good()) {
        cerr << "argv[3] File does not exist" << endl;
        return -1;
    }
    if(in3.peek() == ifstream::traits_type::eof()) {
        cerr << "argv[3] File is empty" << endl;
        return -1;
    }

    ActorGraph* actor_graph = new ActorGraph(); //initialize graph

    //open the input file and output files
    ofstream outfile;
    outfile.open(argv[4], ios::binary);
    outfile << "(actor)--[movie#@year]-->(actor)--...\n"; //header
    
    ActorNode* start;
    ActorNode* end;
    ActorNode* curr;

    if(typeOfWeight == "u") { // use default dummy weights of 1
    	actor_graph->loadFromFile(argv[1], false);
    }
    if(typeOfWeight == "w") { // use calculated weights of each edge
    	actor_graph->loadFromFile(argv[1], true);
    }

    actor_graph->build(); // create the edges between the vertices

    bool have_header = false;
    // find the shortest path between the two specified nodes
    while(in3) {
    	string line;

        // get the next line
    	if (!getline( in3, line )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

		istringstream ss(line);
		vector<string> record;

		// get the names of the starting and ending vertices
		while(ss) {
			string next;
			if(!getline(ss, next, '\t')) break;

		  	record.push_back(next);
		}

		actor_graph->resetNodes(); // reinitialize each vertices dist,prev,done
	    start = actor_graph->getActor(record[0]); // the starting ActorNode
		end = actor_graph->getActor(record[1]); // the ending ActorNode

		priority_queue<ActorNode*, vector<ActorNode*>, ActorPtrComp> pq;

		/** perform dijkstra's algorithm **/
		start->dist = 0;
		pq.push(start); // enqueue weight of 0 with starting vertex

        int weight;
        int total_dist;

        ActorNode* neighbor;
		while(!pq.empty()) {
			curr = pq.top();
			pq.pop();

            if(typeOfWeight == "w") { // if using weighted paths
                if(curr == end) break; // end djikstra's when finding the end node
            }

			if(!curr->done) { 
				curr->done = true;
				for(unsigned int i = 0; i < curr->neighborSize(); i++) {
					neighbor = curr->neighbors[i];
					weight = curr->edge_map[neighbor].second;
					total_dist = curr->dist + weight;
					
					if(total_dist < neighbor->dist) {
						neighbor->prev = curr;
						neighbor->dist = total_dist;
						pq.push(neighbor);
					}
				}
			}
		}

		// add the path from end to beginning onto a stack
		stack<ActorNode*> path;
		path.push(end);

		curr = end;
		while(curr->prev != start) { //pushes curr->prev to the stack  
			path.push(curr->prev);
			curr = curr->prev;
		}

		path.push(start);

		// write the path to the outfile
		while(path.top() != end) {
			curr = path.top();
			path.pop();
			outfile << "(" << curr->name << ")--[" << curr->edge_map[path.top()].first << "]-->";
		}
		outfile << "(" << end->name << ")" << endl;
	}

    //close the files
	in1.close();
    in3.close();
    outfile.close();

    delete actor_graph;
    return 0;
}
