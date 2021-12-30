#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "ERROR: Correct format: ./wordGen NUM_WORDS OUTPUT_FILE" << endl;
        return 1;
    }

    long long numWords = stoi(argv[1]);
    string outFile = argv[2];

    srand(time(NULL));

    ofstream out(outFile);
    string str;
    out << numWords << endl;
    for (long long i = 0; i < numWords; ++i) {
        str.clear();
        for (int i = 0; i < 30; ++i)
            str += (char)(rand() % (122 - 97 + 1) + 97);
        out << str;
        out << rand() % 366;
        if (!(i == numWords - 1))
            out << endl;
    }
    out.close();

    return 0;
}