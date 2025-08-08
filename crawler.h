// #pragma once
#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include "stringhandling.h"
#include "Hastable/hash.h"

using namespace std;

class crawler
{
private:
    int maxdepth;
    char *targetDirectory;
    Hash<char*, char*> h;
    int extracturl(const char filename[], char *newUrls[], int maxUrls);

public:
    crawler(int md, char *directory);
    void createdirectory();
    void wgetrequest(char *url , int depth);
    char *uniquename();
    char *inttochar(long long n);
    bool is_html(char *seedUrl);
    int isvalid(char *seedUrl);
    void parse_all_files();
    void index_and_Url(const char* filepath);
};

#include "./crawler.cpp"
#endif
