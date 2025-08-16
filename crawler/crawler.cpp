#include <iostream>
#include <fstream>
#include <ctime>
#include <direct.h>
#include <cstdlib>
#include "crawler.h"

crawler::crawler(int md, char *directory, char *f) : h(10)
{
    website = f;
    maxdepth = md;
    targetDirectory = directory;
    setwebsite();
}

void crawler::setwebsite()
{
    ofstream outFile("website");
    if (outFile.is_open())
    {
        outFile << website << "\n";
        outFile.close();
    }
    else
    {
        cout << "Failed to create website file\n";
    }
}
void crawler::createdirectory()
{
    if (_mkdir(targetDirectory) == 0)
    {
        cout << "Directory created\n";
    }
    else
    {
        cout << "Directory already exist or failed to create\n";
    }
}

int crawler::isvalid(char *seedUrl)
{
    char urlpre[] = "https://";
    char urlpre1[] = "http://";

    if (strstre(seedUrl, urlpre) != -1 || strstre(seedUrl, urlpre1) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char *crawler::uniquename()
{
    time_t timenow = time(0);
    char *Name = inttochar((long long)timenow);
    return Name;
}

char *crawler::inttochar(long long n)
{
    if (n == 0)
    {
        char *result = new char[2];
        result[0] = '0';
        result[1] = '\0';
        return result;
    }

    bool isNegative = false;
    if (n < 0)
    {
        isNegative = true;
        n = -n;
    }

    char buffer[30];
    int i = 0;

    while (n != 0)
    {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }

    if (isNegative)
    {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    char *result = new char[i + 1];
    strcopy(result, buffer);
    return result;
}

bool crawler::is_html(char *seedUrl)
{
    const char *nonHtmlExt[] = {
        ".pdf", ".jpg", ".jpeg", ".png", ".gif", ".bmp",
        ".svg", ".webp", ".css", ".js", ".ico", ".woff", ".woff2", ".ttf"};

    char *lowerUrl = to_lowercase(seedUrl);

    for (const char *ext : nonHtmlExt)
    {
        if (strstre(lowerUrl, ext) != -1)
        {
            delete[] lowerUrl;
            return false;
        }
    }

    delete[] lowerUrl;
    return true;
}

void crawler::saveKeywordURL(const char *keyword, const char *seedUrl)
{
    char lines[1000][500];
    int lineCount = 0;
    bool found = false;

    ifstream inFile("index");
    if (inFile.is_open())
    {
        while (inFile.getline(lines[lineCount], 500) && lineCount < 1000)
        {
            int arrowPos = strstre(lines[lineCount], " → ");
            if (arrowPos != -1)
            {
                char existingKeyword[200];
                substring(existingKeyword, lines[lineCount], 0, arrowPos);

                if (strcampare(existingKeyword, keyword) == 0)
                {
                    found = true;
                    if (strstre(lines[lineCount], seedUrl) == -1)
                    {
                        strconcat(lines[lineCount], ", ");
                        strconcat(lines[lineCount], seedUrl);
                    }
                }
            }
            lineCount++;
        }
        inFile.close();
    }

    if (!found)
    {
        char newLine[500] = "";
        strcopy(newLine, keyword);
        strconcat(newLine, " → ");
        strconcat(newLine, seedUrl);
        strcopy(lines[lineCount], newLine);
        lineCount++;
    }

    ofstream outFile("index");
    if (!outFile.is_open())
    {
        cout << "Failed to open index for writing\n";
        return;
    }
    for (int i = 0; i < lineCount; i++)
    {
        outFile << lines[i] << "\n";
    }
    outFile.close();
}

char *give_keyword(const char *text)
{
    char **tokens = tokenize(text);
    if (!tokens)
    {
        cout << "Tokenization failed" << endl;
        return nullptr;
    }

    Hash<char *, int> freqMap(100);

    for (int i = 0; tokens[i] != nullptr; ++i)
    {
        if (!is_valid_word(tokens[i]))
        {
            delete[] tokens[i];
            continue;
        }

        char *lower = to_lowercase(tokens[i]);
        delete[] tokens[i];

        if (is_stop_word(lower))
        {
            delete[] lower;
            continue;
        }

        Node<char *, int> *node = freqMap.get(lower);
        if (node)
        {
            node->data += 1;
            delete[] lower;
        }
        else
        {
            freqMap.insert(lower, 1);
        }
    }

    delete[] tokens;

    char *keyword = nullptr;
    int maxFreq = 0;

    for (int i = 0; i < freqMap.getSize(); ++i)
    {
        Node<char *, int> *temp = freqMap.getNode(i);
        while (temp != nullptr)
        {
            if (temp->data > maxFreq)
            {
                maxFreq = temp->data;
                keyword = temp->key;
            }
            temp = temp->next;
        }
    }

    if (keyword)
    {
        char *result = new char[strlength(keyword) + 1];
        strcopy(result, keyword);
        return result;
    }

    return nullptr;
}

bool crawler::readFile(const char *filepath, char *buffer, size_t size)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cout << "Failed to open file " << filepath << endl;
        return false;
    }

    size_t i = 0;
    char ch;
    while (file.get(ch) && i < size - 1)
    {
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    file.close();
    return true;
}

void crawler::index_and_Url(const char *filepath, const char *seedUrl)
{
    char buffer[10000];
    if (!readFile(filepath, buffer, sizeof(buffer)))
        return;

    char cleaned[10000];
    removeScriptAndStyle(buffer, cleaned);

    char text[10000];
    stripHTMLTags(cleaned, text);

    whitespace(text);
    char *keyword = give_keyword(text);
    if (keyword != nullptr)
    {
        cout << "Keyword " << keyword << endl;
        saveKeywordURL(keyword, seedUrl);
        delete[] keyword;
    }
}

bool crawler::is_absolute_url(const char *url)
{
    return (strstre(url, "http://") != -1 || strstre(url, "https://") != -1);
}

char *crawler::resolve_relative_url(const char *baseUrl, const char *relativeUrl)
{
    int lengthBase = strlength(baseUrl);
    int lengthRel = strlength(relativeUrl);

    bool baseEndsWithSlash = (lengthBase > 0 && baseUrl[lengthBase - 1] == '/');
    bool relStartsWithSlash = (lengthRel > 0 && relativeUrl[0] == '/');

    int extraSlash = 0;
    if (baseEndsWithSlash && relStartsWithSlash) {
        relativeUrl++;
        lengthRel--;
    } 
    else if (!baseEndsWithSlash && !relStartsWithSlash) {
        extraSlash = 1;
    }

    char *resolved = new char[lengthBase + lengthRel + extraSlash + 1];

    strcopy(resolved, baseUrl);
    if (extraSlash) {
        strconcat(resolved, "/");
    }
    strconcat(resolved, relativeUrl);

    return resolved;
}


void saveUrl(char *seedUrl)
{
    ofstream outFile("visitedUrl", std::ios::app);
    if (outFile.is_open())
    {
        outFile << seedUrl << "\n";
        outFile.close();
    }
    else
    {
        cout << "Failed to open visitedUrl file\n";
    }
}

int crawler::extracturl(const char filename[], char *newUrls[], int maxUrls, const char *baseUrl)
{
    char buffer[100000];
    int len = 0;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open file " << filename << endl;
        return 0;
    }
    char ch;
    while (file.get(ch) && len < sizeof(buffer) - 1)
    {
        buffer[len++] = ch;
    }
    buffer[len] = '\0';
    file.close();

    int count = 0;
    int pos = 0;

    while (pos < len && count < maxUrls)
    {
        int a_pos = strcasestre(&buffer[pos], "<a");
        if (a_pos == -1)
            break;
        pos += a_pos;

        int href_pos = strcasestre(&buffer[pos], "href=");
        if (href_pos == -1)
        {
            pos += 3;
            continue;
        }
        href_pos += pos;

        char quote = buffer[href_pos + 5];
        if (quote != '"' && quote != '\'')
        {
            pos = href_pos + 6;
            continue;
        }

        int start = href_pos + 6;
        int end = start;
        while (buffer[end] && buffer[end] != quote)
            end++;

        if (end == start)
        {
            pos = end + 1;
            continue;
        }

        int url_len = end - start;
        char *url = new char[url_len + 1];
        for (int i = 0; i < url_len; i++)
        {
            url[i] = buffer[start + i];
        }
        url[url_len] = '\0';

        char *lower = to_lowercase(url);
        if (strstre(lower, "javascript:") == 0 || url[0] == '#' || strstre(lower, "mailto:") == 0)
        {
            delete[] url;
            delete[] lower;
            pos = end + 1;
            continue;
        }
        delete[] lower;

        char *finalUrl = nullptr;
        if (is_absolute_url(url))
        {
            finalUrl = url;
        }
        else
        {
            finalUrl = resolve_relative_url(baseUrl, url);
            delete[] url;
        }


        if (is_html(finalUrl) && h.get(finalUrl) == nullptr)
        {
            bool duplicate = false;
            for (int k = 0; k < count; k++)
            {
                if (strcampare(newUrls[k], finalUrl) == 0)
                {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
            {
                newUrls[count] = new char[strlength(finalUrl) + 1];
                strcopy(newUrls[count], finalUrl);
                count++;
            }
        }
    // saveUrl(finalUrl);

        if (!is_absolute_url(url))
        {
            delete[] finalUrl;
        }

        pos = end + 1;
    }

    return count;
}



void crawler::wgetrequest(char *seedUrl, int depth)
{
    if (depth == -1)
    {
        return;
    }

    if (!isvalid(seedUrl))
    {
        return;
    }
    if (h.get(seedUrl) != nullptr)
    {
        return;
    }

    createdirectory();

    char *filename = uniquename();
    // char *safeUrl = escape_url(seedUrl);

    char command[2000] = "";
    strcopy(command, "wget --no-check-certificate \"");
    strconcat(command, seedUrl);
    strconcat(command, "\" -O \"");
    strconcat(command, targetDirectory);
    strconcat(command, "/");
    strconcat(command, filename);
    strconcat(command, ".html\"");

    // delete[] safeUrl;

    if (system(command) != 0)
    {
        cout << "Failed to download" << seedUrl << endl;
        delete[] filename;
        return;
    }
    saveUrl(seedUrl);
    char filepath[1000];
    strcopy(filepath, targetDirectory);
    strconcat(filepath, "/");
    strconcat(filepath, filename);
    strconcat(filepath, ".html");

    char *keyCopy = new char[strlength(seedUrl) + 1];
    strcopy(keyCopy, seedUrl);

    char *fileCopy = new char[strlength(filepath) + 1];
    strcopy(fileCopy, filepath);

    h.insert(keyCopy, fileCopy);

    cout << "URL = " << keyCopy << endl;
    cout << "FILE = " << fileCopy << endl;

    index_and_Url(filepath, seedUrl);

    delete[] filename;

    char *newUrls[1000];
    int foundCount = extracturl(filepath, newUrls, 1000, seedUrl);
    for (int i = 0; i < foundCount; i++)
    {
        wgetrequest(newUrls[i], depth - 1);
        delete[] newUrls[i];
    }
}
