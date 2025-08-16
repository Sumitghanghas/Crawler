
#include <iostream>
using namespace std;

//-------Node class-----//
template <typename v, typename k>
class Node {
public:
    v key;
    k data;
    Node* next;
Node(v ke, k value);

};


//-------HashTable class-----//
template <typename v, typename k>
class Hash
{
private:
    int size;
    int thrashold = 75;
    Node<v, k> **table;

    void resize();

    int hashFunction(int key)
    {
        return key % size;
    }

    int hashFunction(string key)
    {
        int hash = 0;
        for (char ch : key)
        {
            hash = (hash * 31 + ch) % size;
        }
        return hash;
    }
    // int hashFunction(char* key);


public:
    int capacity;
    Hash(int s);
    Hash(const Hash<v, k> &other);
    Hash<v, k> &operator=(const Hash<v, k> &other);
    void insert(v key, k value);
    void remove(v key, k value);
    void display();
    int search(v key, k value);
    Node<v, k>* get(v key);
int getSize();
Node<v, k>* getNode(int index);
    ~Hash();
};

#include "source.cpp"
