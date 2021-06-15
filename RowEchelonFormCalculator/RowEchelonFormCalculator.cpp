

#include <iostream>         //printing
#include <vector>           //container for values to be stored
#include <algorithm>        //just for std::swap as it has the proper move semantics that we need/right implementation without doing anything myself

//does the required operation for the rest of the values in the column. Notice how i is initialized to b + 1, where b is the value to skip.
//p is some scalar constructed from what is needed to set that row appropiately.
//the only thing differing in the two functions below is the operation of adding or subtracting.
//this can be simplified/improve by sending a function object of the operation to be done, but I consider it fine as it is as it doesn't lead to code bloat in these instances.
void changeRows(std::vector<std::vector<float>>& container, float p, int n, int y, int b, std::vector<std::pair<int, bool>>& map) {
    bool check = false;
    for (int i = b + 1; i < container[y].size(); i++) {
        container[n][i] = (container[n][i] + (container[y][i] * p));
        if (container[n][i] != 0 && !check) {
            map[n] = { i, true };
            check = true;
        }
    }
}

void changerRows(std::vector<std::vector<float>>& container, float p, int n, int y, int b, std::vector<std::pair<int, bool>>& map) {
    bool check = false;
    for (int i = b + 1; i < container[y].size(); i++) {
        container[n][i] = (container[n][i] - (container[y][i] * p));
        if (container[n][i] != 0 && !check) {
            map[n] = { i,true };
            check = true;
        }
    }
}

//this is to check on whether any given row is guaranteed to be in the right order and thus allowing for the next set of rows downward.
//if so, returns true, otherwise false.
bool completeOrder(std::vector<std::pair<int, bool>>& map, int index) {
    for (int i = 0; i < map.size(); i++) {
        if (i <= index) continue;
        if (map[i].second) {
            if (map[i].first < map[index].first) return false;
        }
    }
    return true;
}

int main()
{
    int check = 0;
    int a, b;
    std::cout << "Rows of your matrix? ";
    std::cin >> a;
    std::cout << "\nColumms of your matrix? ";
    std::cin >> b;
    std::vector<std::vector<float>> container(a, std::vector<float>(b));        //while the values can be int or floats, I cover all the bases by setting the container
    std::vector<std::pair<int, bool>> map(a);                                    //to float and allowing for narrowing conversions on the case that it is ints.
    std::cout << "\nType in your matrix below\n";
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            std::cin >> container[i][j];
            if (container[i][j] != 0) {             //Matrix operations require pivots from which to work on
                if (!map[i].second) {               //we can label them here in the container map as we initialize them in container
                    map[i].first = j;
                    map[i].second = true;
                }
            }
        }
    }
    for (int i = 0; i < map.size();) {
        for (int j = i, p = j; j < map.size(); j++, p++) {
            if (map[i].first > map[j].first && map[j].second) {
                while (p > i) {                                         //switches rows so that we can have pivots in right locations
                    std::swap(container[p], container[p - 1]);          //as required for reducing them. Keep in mind I switch first and
                    std::swap(map[p], map[p - 1]);                      //then reduce, where in other instances you can have them coincide.
                    p--;                                                //I haven't found this to lead to an error, so I keep the order as is.
                }
                break;
            }
        }
        if (completeOrder(map, i)) i++;
    }
    for (int j = 0; j < a; j++) {                                   //from every pivot, we clear every number below the pivot as required via established matrix operations.
        for (int i = j + 1; i < a; i++) {                       //here we simply check for the operation to be done and send it to the appropiate functions to do those operations for the particular row in question.
            if (container[i][map[j].first] != 0) {              //we know the value below pivots needs to be equal to zero, so instead of sending that value to the appropiate function, I simply set it to zero to avoid floating precision errors.
                float p = container[i][map[j].first] / container[j][map[j].first];      //the comparison used is an infinitesimal number so that we can also avoid here floating precision errors.
                if (std::abs((container[i][map[j].first] + (p * container[j][map[j].first]))) < 1e-4) {
                    container[i][map[j].first] = 0;
                    changeRows(container, p, i, j, map[j].first, map);
                }
                else if (std::abs((container[i][map[j].first] - (p * container[j][map[j].first]))) < 1e-4) {
                    container[i][map[j].first] = 0;
                    changerRows(container, p, i, j, map[j].first, map);
                }
            }
        }
    }
    std::cout << "\n\n";
    for (const auto& row : container) {
        for (float each : row) {            //print the resulting matrix
            std::cout << each << " ";
        }
        std::cout << '\n';
    }
}

