/*
    maze_solver.cpp

    Code for the maze_solver class.  This class will use stack-based depth
    first search or queue-based breadth first search to find a solution (if
    possible) to a simple maze.

    Original Author: C. Painter-Wakefield
    Additional Author: Noah Terry
*/

#include "maze_solver.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// initialize()
// Find the start and goal points.  Push or enqueue the start point.  Set
// the boolean no_more_steps and goal_reached flags to false.
void maze_solver::initialize() {

    for (int i = 0; i < rows; i++){
        for(int j = 0; j < maze[i].size(); j++){
            if (maze[i][j] == 'o'){
                start_pt.set_row(i);
                start_pt.set_col(j);
                start_pt.set_data(maze[i][j]);
            }
            else if (maze[i][j] == '*'){
                end_pt.set_row(i);
                end_pt.set_col(j);
                end_pt.set_data(maze[i][j]);
            }
        }
    }

    if(use_stack) stack_visit.push(start_pt);
    else queue_visit.push(start_pt);

    no_more_steps = false;
    goal_reached = false;
}

// write_maze()
// Output the (partially or totally solved) maze on the provided output stream.
void maze_solver::write_maze(ostream& out) {
    for (int i = 0; i < rows; i++){
        out << maze[i] << endl;
    }
}

// step()
// Take one step towards solving the maze, setting no_more_steps and
// goal_reached as appropriate.  This implements the essential maze search
// algorithm; take the next location from your stack or queue, mark the
// location with '@', add all reachable next points to your stack or queue,
// etc.
void maze_solver::step() {

    vector<point> vect_points;
    if (use_stack){
        // Remove any visited points from the stack and then set the current point to the top of the stack.
        // If the stack is empty there are no more steps.
        do{
            if (stack_visit.empty()) {
                no_more_steps = true;
                return;
            }
            curr_pt = stack_visit.top();
            stack_visit.pop();
            curr_pt.set_data(maze[curr_pt.get_row()][curr_pt.get_col()]);
        } while (curr_pt.get_data() == '@');

    }
    else{
        // Remove any visited points from the queue and then set the current point to the front of the queue.
        // If the queue is empty there are no more steps.
        do{
            if (queue_visit.empty()) {
                no_more_steps = true;
                return;
            }
            curr_pt = queue_visit.front();
            queue_visit.pop();
            curr_pt.set_data(maze[curr_pt.get_row()][curr_pt.get_col()]);
        } while (curr_pt.get_data() == '@');
    }

    // If the data stored at the current point is '*' then the solution has been found.
    if (curr_pt.get_data() == '*'){
        goal_reached = true;
        no_more_steps = true;
        return;
    }

    // Initialize the next point up from the current point and add it to the vector of points.
    point next_pt_up;
    next_pt_up.set_row(curr_pt.get_row() - 1);
    next_pt_up.set_col(curr_pt.get_col());
    set_point_data(next_pt_up);
    vect_points.push_back(next_pt_up);

    // Initialize the next point to the right of the current point and add it to the vector of points.
    point next_pt_right;
    next_pt_right.set_row(curr_pt.get_row());
    next_pt_right.set_col(curr_pt.get_col() + 1);
    set_point_data(next_pt_right);
    vect_points.push_back(next_pt_right);

    // Initialize the next point down from the current point and add it to the vector of points.
    point next_pt_down;
    next_pt_down.set_row(curr_pt.get_row() + 1);
    next_pt_down.set_col(curr_pt.get_col());
    set_point_data(next_pt_down);
    vect_points.push_back(next_pt_down);

    // Initialize the next point to the left of the current point and add it to the vector of points.
    point next_pt_left;
    next_pt_left.set_row(curr_pt.get_row());
    next_pt_left.set_col(curr_pt.get_col() - 1);
    set_point_data(next_pt_left);
    vect_points.push_back(next_pt_left);

    // Set the data of visited points to '@' so they will be avoided in the future.
    if (curr_pt.get_data() == '.'){
        curr_pt.set_data('@');
        maze[curr_pt.get_row()][curr_pt.get_col()] = '@';
    }

    // push all valid points either into the stack or queue.
    for (auto possible_point : vect_points){
        if ( valid_point(possible_point) && use_stack ){
            stack_visit.push(possible_point);
        }
        else if ( valid_point(possible_point) && !use_stack ){
            queue_visit.push(possible_point);
        }
    }
}

// Sets the data of the point.
void maze_solver::set_point_data(point &next_pt){
    // If the point is within the bounds of the maze set the data of the point to the data at the corresponding point on the maze.
    if ( ( (next_pt.get_row() < rows) && (next_pt.get_row() >= 0) ) && ( (next_pt.get_col() < columns) && (next_pt.get_col() >= 0) ) ){
        next_pt.set_data(maze[next_pt.get_row()][next_pt.get_col()]);
    }
    // If the point is out of bounds then set the data to '#' so it will be treated as a wall.
    else {
        next_pt.set_data('#');
    }
}

// Tests to see if the point should be visited.
bool maze_solver::valid_point(point next_pt) {
    // If the point is either a '.' or '*' it can be used as a next possible point.
    return (next_pt.get_data() == '.') || (next_pt.get_data() == '*');
}

/***************************************************************************
    You should not need to modify code below this point.  Touch at your own
    risk!
****************************************************************************/

// Here's the constructor code.  You should not need to modify this, but you
// can if you want.  Right now it takes care of reading in the maze for you.
// The maze is stored as a Vector<string>, but you can change that if you wish.
maze_solver::maze_solver(string infile, bool use_stak, bool pause) {
    use_stack = use_stak;
    do_pause = pause;

    // parse out maze name for later use in creating output file name
    int pos = infile.find(".");
    if (pos == string::npos) {
        maze_name = infile;
    } else {
        maze_name = infile.substr(0, pos);
    }

    // open input file and read in maze
    ifstream fin(infile);
    if (!fin) {
        cerr << "Error opening input file \"" << infile << "\"; exiting." << endl;
        exit(1);
    }

    fin >> rows >> columns;

    string row;
    getline(fin, row);
    for (int i = 0; i < rows; i++) {
        getline(fin, row);
        maze.push_back(row); //old stanford library call .add(row);
    }

    fin.close();
}

// run()
// Drives the solution forward.
// While more steps are possible (while no_more_steps == false), run repeatedly
// calls step(), then write_maze() on cout, then pause().  Once there are no
// more steps, it prints a success/failure message to the user (based on the
// goal_reached flag) and writes the final maze to a solution file.
void maze_solver::run() {
    cout << "Solving maze '" << maze_name << "'." << endl;
    cout << "Initial maze: " << endl << endl;
    write_maze(cout);
    cout << endl;
    pause();

    // main loop
    while (!no_more_steps) {
        step();
        cout << endl;
        write_maze(cout);
        cout << endl;
        pause();
    }

    // final output to user
    cout << "Finished: ";
    if (goal_reached) {
        cout << "goal reached!" << endl;
    } else {
        cout << "no solution possible!" << endl;
    }

    // save solution file
    string outfile;
    if (use_stack) outfile = maze_name + "-stack-solution.txt";
    else outfile = maze_name + "-queue-solution.txt";

    ofstream fout(outfile);
    if (!fout) {
        cerr << "Could not open file \"" << outfile << "\" for writing." << endl;
        cerr << "Solution file could not be saved!" << endl;
    }
    else {
        write_maze(fout);
        fout.close();
    }
}

void maze_solver::pause() {
    if (!do_pause) return;
    string foo;
    cout << "Hit Enter to continue...";
    getline(cin, foo);
}