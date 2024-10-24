#include "Algo.h"
#include <iostream>

using std::cout;

int main()
{
    vector<int> a {5,4,8,9,6,2,3,7,48,45,41,12,0,-1,52,100,102,103,996,965,145,412,152,142};
    const vector<int>& b = MergeSort(a);

    for (int i = 0; i < b.size(); i++) {
        cout << b[i] << " ";
    }

    return EXIT_SUCCESS;
}