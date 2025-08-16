#include <iostream>
#include "hash.h"
using namespace std;

int main()
{
    Hash<int, string> m(4);
    m.insert(1, "apple");
    m.insert(2, "banana");
    m.insert(3, "cherry");

    m.display();

    Hash<int, string> m2 = m;
    m2.insert(5, "date");

    m2.display();

    Hash<int, string> m3(2);
    m3 = m; 
    // m3.insert(33, "cherry"); 


    m3.display();

    return 0;
}
