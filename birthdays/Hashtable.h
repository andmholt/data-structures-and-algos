#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~ DECLARE ~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
struct Item {
    Item(std::string k, const T& val);
    bool operator==(Item other);
    std::string k;
    T val;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
class Hashtable {
public:
    Hashtable(bool debug = false, unsigned int size = 11);
    ~Hashtable();
    int add(std::string k, const T& val);
    const T& lookup(std::string k);
    void reportAll(std::ostream& out) const;
    void resize();
    int hash(std::string k) const;

private:
    bool debug;
    int m;
    int r[5];
    int itemsInTable;
    Item<T>** table;
    static const int m_default[17];
    static const int r_default[5];
    T garbage;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~ IMPLEMENT ~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template<class T>
Item<T>::Item(std::string k, const T& val) {
    this->k = k;
    this->val = val;
}

template<class T>
bool Item<T>::operator==(Item other) {
    return (other.k == this->k && other.val == this->val);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// init static vars
template<class T>
const int Hashtable<T>::m_default[17]
        = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117};

template<class T>
const int Hashtable<T>::r_default[5] = {983132572, 1468777056, 552714139, 984953261, 261934300};

// constructor
template<class T>
Hashtable<T>::Hashtable(bool debug, unsigned int size) {
    this->debug = debug;
    m = size;
    itemsInTable = 0;
    table = new Item<T>*[size];
    for (unsigned int i = 0; i < size; ++i)
        table[i] = nullptr;

    // set rand seed
    srand(std::time(nullptr));

    // init r
    if (debug) {
        for (int i = 0; i < 5; ++i)
            r[i] = r_default[i];
    } else {
        for (int i = 0; i < 5; ++i) {
            r[i] = rand() % m;
        }
    }
}

// destructor
template<class T>
Hashtable<T>::~Hashtable() {
    // dealloc
    for (int i = 0; i < m; ++i)
        if (table[i] != nullptr)
            delete table[i];
    delete[] table;
}

template<class T>
int Hashtable<T>::add(std::string k, const T& val) {
    int hashNum = hash(k);

    garbage = val;

    // probe
    int newHash = hashNum;
    int probeCount = 0;
    do {
        if (table[newHash] != nullptr) {
            // std::cout << "hash = " << newHash << std::endl;
            if (table[newHash]->k == k && table[newHash]->val == val) {
                return 0;
            }
        } else {
            table[newHash] = new Item<T>(k, val);
            ++itemsInTable;
            break;
        }
        newHash = (hashNum + (int)pow(++probeCount, 2)) % m;
    } while (table[newHash] != nullptr);

    if ((itemsInTable + 1) / m > 0.5)
        resize();

    return probeCount;
}

template<class T>
const T& Hashtable<T>::lookup(std::string k) {
    int hashNum = hash(k);

    // probe
    int newHash = hashNum;
    int probeCount = 0;
    while (table[newHash] != NULL) {
        if (table[newHash] != NULL) {
            if (table[newHash]->k == k)
                return table[newHash]->val;
        }
        else break;
        newHash = (hashNum + (int)pow(++probeCount, 2)) % m;
    }

    return garbage;
}

template<class T>
void Hashtable<T>::reportAll(std::ostream& out) const {
    for (int i = 0; i < m; ++i) {
        if (table[i] != NULL)
            out << table[i]->k << ' ' << table[i]->val << std::endl;
    }
}

template<class T>
void Hashtable<T>::resize() {

    // find new size
    int newSize = 0;
    int oldSize = m;
    for (int i = 0; i < 17; ++i)
        if (m < m_default[i])
            newSize = m_default[i];

    // new table, swap tables
    Item<T>** newTable = new Item<T>*[newSize];
    for (int i = 0; i < newSize; ++i)
        newTable[i] = nullptr;
    Item<T>** oldTable = table;
    table = newTable;
    m = newSize;

    // gen new r vals
    if (!debug)
        for (int i = 0; i < 5; ++i)
            r[i] = rand() % m;

    // re-hash
    itemsInTable = 0;
    int trash;
    for (int i = 0; i < oldSize; ++i)
        if (oldTable[i] != nullptr) {
            trash = add(oldTable[i]->k, oldTable[i]->val);
            delete oldTable[i];
        }

    // get rid of warnings
    if (trash < 0)
        trash = 0;

    // dealloc
    delete[] oldTable;
}

template<class T>
int Hashtable<T>::hash(std::string k) const {
    long long w[5];
    int letterCount = k.size() - 1;
    long temp;
    for (int i = 4; i >= 0; --i) {
        temp = 0;
        for (int j = 5; j >= 0; --j) {
            if (letterCount >= 0)
                temp += (long long)pow(27, 5 - j) * (long long)((long long)k[letterCount--] - 'a' + 1);
        }
        w[i] = temp;
    }

    long long hashNum = 0;
    for (int i = 0; i < 5; ++i) {
        hashNum += (long long)r[i] * (long long)w[i];
        // std::cout << "  " << w[i] << std::endl;
    }
    hashNum = hashNum % m;

    return hashNum;
}