#include "Hashtable.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int testCollision(ifstream& in) {
    Hashtable<int> calendar(false, 365);

    string temp;
    in >> temp;

    int count = 0;
    int probes = 0;
    string k;
    int val;
    while (probes < 1) {
        in >> k;
        temp = k.substr(30);
        val = stoi(temp);
        k = k.substr(0, 30);
        if (!in.good())
            return -1;
        //cout << "K: " << k << " VAL: " << val << endl;
        probes = calendar.add(k, val);
        ++count;
    }
    return count;
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "ERROR: Correct format: ./birthdays NUM_TESTS INPUT_FILE" << endl;
        return 1;
    }

    int numTests = stoi(argv[1]);
    string inFile = argv[2];
    ifstream in(inFile);

    cout << "Generating birthdays..." << endl;

    int* stats = new int[numTests];
    int lowVals = 0;
    for (int i = 0; i < numTests; ++i) {
        stats[i] = testCollision(in);
        if (stats[i] == -1) {
            // error
            cout << "Error: program ran out of words." << endl;
            cout << "To run " << numTests << " tests, one would need about ";
            cout << numTests * 23 << " words." << endl;
            return 1;
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        if (stats[i] <= 23)
            ++lowVals;
    }

    in.close();

    long long avg = 0;
    for (int i = 0; i < numTests; ++i) {
        avg += stats[i];
    }
    avg /= numTests;

    cout << "Out of " << numTests << " birthday tests, " << lowVals << " collisions occurred in 23 or less";
    cout << " (" << (double)lowVals / (double)numTests << "%)." << endl;
    cout << "Average collision: " << avg << endl;

    // dealloc
    delete[] stats;

    return 0;
}