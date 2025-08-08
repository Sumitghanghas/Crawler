#include <iostream>
#include "crawler.h"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: [exe] [URL] [Directory] [Depth]\n";
        return 1;
    }

    char* seedUrl = argv[1];
    char* targetDirectory = argv[2];
    int maxdepth = atoi(argv[3]); 

    crawler c(maxdepth, targetDirectory);

    c.wgetrequest(seedUrl,maxdepth);

    return 0;
}

