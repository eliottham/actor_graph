/*
 *Overview of actorconnections.cpp: Given a movie database and a list of pair 
 *    actors, actorconnections will answer when every actor pair becomes 
 *    connected using either bfs or ufind algorithm.
 *    4 command arguments:
 *        (1) Name of a text faile containing the movie casts
 *        (2) Name of text file containing the names of actor pairs 
 *        (3) Name of ouput text file
 *        (4) bfs or ufind (determines which algorithm to be used)  If fourth
 *              arguemnt is not given, default is ufind
 */ 

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include "ActorNode.h"
#include "Movie.h"
#include "ActorGraph.h"
#include "ActorGraph.cpp"
#include "util.h"
#include "util.cpp"
using namespace std;

bool BFS(ActorNode* start, ActorNode* end);  //declaration of BFS method

int main(int argc, char* argv[]) {
    ifstream in1(argv[1]);
    ifstream in2(argv[2]);
    ifstream in3(argv[3]);

    /** Null checks **/
    // check number of args
    if(argc < 4 || argc > 5) {
        cerr << "Invalid amount of arguments" << endl;
        cerr << "Example: ./actorconnections movie_casts.tsv test_paris.tsv" 
             <<  "out_connections_bfs.tsv ufind" << endl;
        return -1;
    }

    // check if argv[1] is valid
    if(!in1.good()) {
        cerr << "argv[1]File does not exist" << endl;
        return -1;
    }
    if(in1.peek() == ifstream::traits_type::eof()) {
        cout << "argv[1] File is empty" << endl;
        return -1;
    }

    // check if argv[2] is valid
    if(!in2.good()) {
        cerr << "argv[2] File does not exist" << endl;
        return -1;
    }
    if(in2.peek() == ifstream::traits_type::eof()) {
        cerr << "argv[2] File is empty" << endl;
        return -1;
    }

    // Initialize actor graph 
    ActorGraph* actor_graph = new ActorGraph(); 
    actor_graph->loadFromFile(argv[1], false); // build empty graph of actor nodes
    actor_graph->sortMovies(); // sort the movies by year into a queue

    // Open outfile for writing
    ofstream outfile;
    outfile.open(argv[3], ios::binary);
    outfile << "Actor1\tActor2\tYear\n"; //header

    // Initialize timer 
    Timer timer;

    bool have_header = false;

    /** Implement BFS **/
    if(argv[4] == "bfs"){
        //timer.begin_timer();  <- used for comparing computational times

        ActorNode* start;
        ActorNode* end;

        while(in2) { // find the year each pair of actors become connected
            string line;

            if(!getline(in2, line)) break;

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

            actor_graph->resetNeighbors(); // revert to empty graph of actor nodes
	        actor_graph->sortMovies(); // reinitialize queue of sorted movies

            start = actor_graph->getActor(record[0]); // the starting ActorNode
		    end = actor_graph->getActor(record[1]); // the ending ActorNode

            // create the graph by year
            bool path_exists = false;
            while(actor_graph->buildByYear()) { 
                actor_graph->resetNodes();
                if(BFS(start, end)) { // if BFS returns true, path exists
                    path_exists = true;
                    outfile << start->name << "\t" << end->name <<
                            "\t" << actor_graph->curr_year << endl;
                    break;
                }
            }

            if(!path_exists) { // if no path exists after all years were added
                outfile << start->name << "\t" << end->name << "\t" << "9999" << endl;
            }
        }
        //long bfsruntime = timer.end_timer();
        //cout << "BFS runtime: " << bfsruntime << endl;
    }
        /** implements ufind (default if no algorithm is given) **/
    else {
        //timer.begin_timer();

        ActorNode* start;
        ActorNode* end;

        while(in2) { // find the year each pair of actors become connected
            string line;
            if(!getline(in2, line)) break;

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
           
            actor_graph->resetNodes(); // revert to empty graph of actor nodes
            actor_graph->sortMovies(); // reinitialize queue of sorted movies
            
            start = actor_graph->getActor(record[0]); // the starting ActorNode
            end = actor_graph->getActor(record[1]); // the ending ActorNode
            
            ActorNode* start_parent = start;
            ActorNode* end_parent = end;

            bool path_exists = false;
            // create graph by year
            while(actor_graph->ufindByYear()) {
                while(start_parent->parent != nullptr) { // traverse up tree
                    start_parent = start_parent->parent;
                 }

                 while(end_parent->parent != nullptr) { // traverse up tree
                     end_parent = end_parent->parent;
                 }

                 if(start_parent == end_parent) { // we then know a path exits
                    path_exists = true;
                    outfile << start->name << "\t" << end->name <<
                            "\t" << actor_graph->curr_year << endl;

                    break;
                 }
            }
            if(!path_exists) {  // prints 9999 if no path exists between actors
                outfile << start->name << "\t" << end->name << "\t" << "9999" << endl;
            }
        }
        //long ufindRuntime = timer.end_timer();
        //cout << "ufindRuntime: " << ufindRuntime << end;

    }

    // close files 
    in1.close();
    in2.close();
    outfile.close();

    delete actor_graph;
    return 0; 
}

/**Function to implement the BFS algorithm
 * Purpose: Uses BFS algorithm to find the connection between actors 
 * Inputs: Two actor nodes 
 * Outputs: True if path exists, false if not **/
bool BFS(ActorNode* start, ActorNode* end) {
    queue<ActorNode*> q; //initialize queue
    start->dist = 0; //set the distance to 0
    q.push(start); //add to queue

    ActorNode* curr;

    while(!q.empty()) { // conduct BFS on graph created up to current year
        curr = q.front();
        q.pop();
        if(!curr->done) {
            curr->done = true;
            for(unsigned int i = 0; i < curr->neighborSize(); i++) {
                if(curr->neighbors[i]->dist > curr->dist + 1) {
                    curr->neighbors[i]->dist = curr->dist + 1;
                    curr->neighbors[i]->prev = curr;
                    q.push(curr->neighbors[i]);
                }
            }
        }
    }

    // check if path exists between actors in this year
    curr = end;
    while(curr->prev != nullptr) { //traverse through curr->prev
        curr = curr->prev;
        if(curr == start) { // path exists
            return true;

        }
    }
    return false;
}
