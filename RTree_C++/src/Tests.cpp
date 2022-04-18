#include <vector>
#include <iostream>
#include "RoutingEntry.h"

using namespace std;

int main (){
    RoutingEntry test;
    //   |
    //   |
    //   4|
    //   3|
    //   2|
    //   1|     x x
    //   0| _ _ x x _ _ _
    //  -1|     x x
    //  -2|
    //  -3| 0 1 2 3 4
    test.from.emplace_back(-1);
    test.from.emplace_back(-1);
    test.from.emplace_back(-1);
    test.to.emplace_back(-3);
    test.to.emplace_back(-3);
    test.to.emplace_back(-3);
    vector<int> v = {0, 0, 0};
    cout << "Distance: " << test.calculateDistance(v) << endl;
    cout << "Area: " << test.calculateArea() << endl;
}

//TODO method for each test and these put in main