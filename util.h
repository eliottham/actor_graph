#ifndef UTIL_H
#define UTIL_H

#include <chrono>

#include <vector>
#include <string>
#include <iostream>

using std::istream;

class Timer{
private:
    /* start the timer */
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    
public:
    
    /*
     * Function called when starting the timer.
     */
    void begin_timer();
    
    /*
     * Function called when ending the timer. Returns duration in nanoseconds
     * PRECONDITION: begin_timer() must be called before this function
     */
    long long end_timer();
    
    
};

class Utils{
private:
    
    /* Remove the first set of characters before the first space
	 * from the string line.  This function assumes these will 
	 * be a numeric frequency and the rest of the line is a word
	 * or phrase.
	 */

    
    
    
public:

    
    
    
};


#endif //UTIL_H
