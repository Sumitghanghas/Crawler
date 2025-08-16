#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include "libary/stringhandling.h"
#include "Hastable/hash.h"

using namespace std;

class crawler
{
private:
    int maxdepth;
    char *targetDirectory;
    char *website;
    Hash<char *, char *> h;
    int extracturl(const char filename[], char *newUrls[], int maxUrls);
    bool is_absolute_url(const char *url);
    char *resolve_relative_url(const char *baseUrl, const char *relativeUrl);
    int extracturl(const char filename[], char *newUrls[], int maxUrls, const char *baseUrl);
    void setwebsite();
public:
    crawler(int md, char *directory, char *f);
    void createdirectory();
    void wgetrequest(char *url, int depth);
    char *uniquename();
    char *inttochar(long long n);
    bool is_html(char *seedUrl);
    int isvalid(char *seedUrl);
    void parse_all_files();
    void index_and_Url(const char *filepath, const char *seedUrl);
    void saveKeywordURL(const char *keyword, const char *seedUrl);
    bool readFile(const char *filepath, char *buffer, size_t size);
};

#include "./crawler.cpp"
#endif