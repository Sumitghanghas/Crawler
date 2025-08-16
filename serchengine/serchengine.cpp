#include <iostream>
#include <fstream>
using namespace std;


search::search(char *seedurl){
    website = seedurl;
}

void search::searchUrls(char *keyword)
{
    char line[500];
  
    ifstream inFile("index");
    char *lowercasekeyword = to_lowercase(keyword);
    if (!inFile.is_open())
    {
        cout << "filed to open file.\n";
        return;
    }

    bool found = false;
    while (inFile.getline(line, sizeof(line)))
    {
        int arrowPos = strstre(line, " → ");
        if (arrowPos != -1)
        {
            line[arrowPos] = '\0';
            if (strcampare(line, lowercasekeyword) == 0)
            {
                found = true;
                char *urls = &line[arrowPos + 3];
                cout << urls << endl;
                break;
            }
            line[arrowPos] = ' ';
        }
    }

    if (!found)
    {
        cout << "No URLs found for keyword " << keyword << "\n";
    }

    inFile.close();
}

int search::searchLink(){
     char line[500];
    ifstream inFile("website");
    if (!inFile.is_open()) {
        return 0; 
    }

    if (!inFile.getline(line, 500)) {
        inFile.close();
        return 0;
    }
    inFile.close();

    if (strcampare(line,website) == 0) {
        return 1; 
    }
    return 0;
}