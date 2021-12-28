#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void decrypt(long, int);
void encrypt();
long calcKey(long, long);
long calcGCD(long, long);
long modExp(long, long, long);
string toBinary(long);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Incorrect format" << endl <<
        "Instead use: ./rsa [key p] [key q] where p and q are two large primes" << endl;
        return 1;
    }

    // input vars
    long p, q;
    string command;
    long d;
    int n;

    p = stol(argv[1]);
    q = stol(argv[2]);

    d = calcKey(p, q);
    n = p * q;

    while (command != "EXIT") {

        cout << "Please enter a command:" << endl <<
        "(ENCRYPT, DECRYPT, EXIT)" << endl;

        cin >> command;

        // evaluate command type
        if (command == "EXIT")
            break;
        else if (command == "DECRYPT")
            decrypt(d, n);
        else if (command == "ENCRYPT")
            encrypt();
    }

    return 0;
}

void decrypt(long d, int n) {

    // error
    if (n < 27) {
        cout << "Error: n value is too small. Terminating..." << endl;
        terminate();
    }

    long x = 1 + (log(n / 27) / log(100));

    string input;
    string output;

    cout << "Format: [input file] [output file]" << endl;
    cin >> input >> output;

    // open files
    ifstream inputFile(input);
    ofstream outputFile(output);

    long C;
    long M;
    string word;

    while (inputFile.good()) {

        word.clear();

        inputFile >> C;
        M = modExp(C, n, d);

        for (int i = 0; i < x; ++i) {
            if (M % 100 == 0)
                word = ' ' + word;
            else
                word = (char)('`' + M % 100) + word;
            M /= 100;
        }

        outputFile << word;
    }

    // close files
    inputFile.close();
    outputFile.close();
}

long calcKey(long p, long q) {

    // find LCM
    long l = ((p - 1) * (q - 1));
    l /= calcGCD(p - 1, q - 1);

    // error
    if (l <= 65537) {
        cout << "Error: LCM of p-1 and q-1 < e" << endl;
        terminate();
    }

    // extended Euclidian Algorithm
    long s = 0;
    long old_s = 1;
    long t = 1;
    long old_t = 0;
    long r = 65537;
    long old_r = l;
    long quotient;
    long temp;

    while (r != 0) {
        quotient = (long)floor((long double)old_r / (long double)r);
        temp = r;
        r = old_r - quotient * r;
        old_r = temp;
        temp = s;
        s = old_s - quotient * s;
        old_s = temp;
        temp = t;
        t = old_t - quotient * t;
        old_t = temp;
    }

    long gcd = old_r;
    long d = old_t;

    if (d < 0)
        d += (p - 1) * (q - 1);

    // error
    if (gcd != 1) {
        cout << "Decryption key not guaranteed to work correctly or securely. "
             << "Terminating program now..." << endl;
        terminate();
    }

    return d;
}

long calcGCD(long p, long q) {

    if (p < q)
        swap(p, q);

    if (p == 0)
        return q;
    else if (q == 0)
        return p;
    else {
        // long quotient;
        long remainder;

        // quotient = p/q;
        remainder = p % q;

        return calcGCD(q, remainder);
    }
}

void encrypt() {

    // read vars
    string filename;
    int n;
    string message;

    cout << "Format: [filename] [p*q] [message]" << endl;

    cin >> filename >> n;
    getline(cin, message);
    message.erase(0, 1);

    // if error
    if (n < 27) {
        cout << "Error: n value is too small.. Terminating..." << endl;
        terminate();
    }

    int e = 65537;
    long x = 1 + (log(n / 27) / log(100));

    long M;
    long C;
    long messageIndex = 0;

    ofstream output(filename);

    while (messageIndex < (int)message.size()) {

        M = 0;

        if (messageIndex != 0)
            output << ' ';

        for (int i = 0; i < x; ++i) {

            // read char from line
            M *= 100;
            if (messageIndex >= (int)message.size())
                M += 0;
            else if ((int)message[messageIndex] == 32)
                M += 0;
            else if ((int)message[messageIndex] == 0)
                M += 0;
            else
                M += (int)message[messageIndex] - 96;
            ++messageIndex;
        }

        C = modExp(M, n, e);
        output << C;
    }

    output.close();
}

long modExp(long b, long m, long n) {

    // convert M to binary
    string binaryN = toBinary(n);
    int nLen = binaryN.size();

    long x;
    unsigned long long power;

    // M = b
    // binaryE = n
    // n = m

    x = 1;
    power = b % m;
    for (int i = nLen - 1; i >= 0; --i) {
        if (binaryN[i] == '1')
            x = (x * power) % m;
        power = (power * power) % m;
    }

    return x;
}

string toBinary(long M) {

    string binaryNum;
    long toStr;
    long next = M;

    while (next != 0) {
        toStr = next % 2;
        next /= 2;
        binaryNum = to_string(toStr) + binaryNum;
    }

    return binaryNum;
}