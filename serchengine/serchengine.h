#include <iostream>
using namespace std;


class search
{
private:
    char *website;

public:
    search(char *seedurl);
    void searchUrls(char *keyword);
    int searchLink();
};
#include "./serchengine.cpp"
