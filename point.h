/*
    point.h

    Class declaration for the point class.

    Author: Noah Terry
*/
#ifndef NTERRY__MAZEGRAPH_POINT_H
#define NTERRY__MAZEGRAPH_POINT_H

#include <string>
using namespace std;

class point {
public:
    point(){ _row = _col = 0; }
    int get_row(){ return _row; }
    int get_col(){ return _col; }
    char get_data() { return _data; }
    void set_row(int row) { _row = row; }
    void set_col(int col) { _col = col; }
    void set_data(char data) { _data = data; }
private:
    int _row = 0;
    int _col = 0;
    char _data;
};


#endif
