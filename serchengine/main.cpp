#include <iostream>
#include "../crawler/crawler.h"
#include "serchengine.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: [exe] [URL] [Directory] [Depth]\n";
        return 1;
    }

    char *seedUrl = argv[1];
    char *targetDirectory = argv[2];
    int maxdepth = atoi(argv[3]);

    char input[4];
    search s(seedUrl);
    int found = s.searchLink();
    crawler c(maxdepth, targetDirectory, seedUrl);

    if (found)
    {
        cout << "This website data is present. You want to Crawl again (yes/no): ";
        cin >> input;
        if (strcampare(input, "yes") == 0 || strcampare(input, "Yes") == 0 || strcampare(input, "YES") == 0)
        {
            remove("index");
            c.wgetrequest(seedUrl, maxdepth);
            char keyword[50];
            cout << "Enter the keyword ";
            // cin >> keyword;
            // s.searchUrls(keyword);
        }
        else if (strcampare(input, "no") == 0 || strcampare(input, "No") == 0 || strcampare(input, "NO") == 0)
        {
            char keyword[50];
            cout << "Enter the keyword ";
            // cin >> keyword;
            // s.searchUrls(keyword);
        }
    }
    else
    {
        remove("index");
        c.wgetrequest(seedUrl, maxdepth);
        char keyword[50];
        cout << "Enter the keyword ";
        // cin >> keyword;
        // s.searchUrls(keyword);
    }

    remove(targetDirectory);
    return 0;
}
