/*
    maze_solver.h

    Class declaration for the maze_solver class.

    Original Author: C. Painter-Wakefield
    Additional Author: Noah Terry
*/

#ifndef _MAZE_SOLVER_H
#define _MAZE_SOLVER_H

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include "point.h"

using namespace std;

class maze_solver {
public:
    // constructor
    maze_solver(string infile, bool use_stak, bool pause=false);

    // public methods
    void initialize();
    void run();
    void step();
    void write_maze(ostream& out);
    void pause();
    void set_point_data(point &next_pt);
    static bool valid_point(point next_pt);

    // instance variables
    string maze_name;
    bool use_stack;
    bool do_pause;

    int rows, columns;
    vector<string> maze;

    bool no_more_steps;
    bool goal_reached;

    point start_pt;
    point end_pt;
    point curr_pt;

    stack<point> stack_visit;
    queue<point> queue_visit;
};

#endif