#include <iostream>
#include <fstream>
#include <ctime>
#include <direct.h>
#include <cstdlib>
#include "crawler.h"

crawler::crawler(int md, char *directory , char* f) : h(10)
{
    website = f;
    maxdepth = md;
    targetDirectory = directory;
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
    const char *extensions[] = {
        ".pdf", ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".",
        ".svg", ".webp", ".css", ".js", ".ico", ".woff", ".woff2", ".ttf"};

    char checkUrl[100];
    strcopy(checkUrl, seedUrl);
    checkUrl[strlength(checkUrl)] = '\0';

    for (int i = 0; checkUrl[i] != '\0'; i++)
    {
        checkUrl[i] = lowercase(checkUrl[i]);
    }

    int index = strchar(checkUrl, '.');
    if (index == -1)
    {
        return true;
    }

    const char *ext = &checkUrl[index];

    for (const char *exte : extensions)
    {
        if (strcampare(ext, exte) == 0)
        {
            return false;
        }
    }
    return true;
}

void crawler::saveKeywordURL(const char* keyword, const char* seedUrl) {
    char lines[1000][500];
    int lineCount = 0;
    bool found = false;

    ifstream inFile("index");
    if (inFile.is_open()) {
        while (inFile.getline(lines[lineCount], 500)) {
            if (lineCount > 0) { 
                int arrowPos = strstre(lines[lineCount], " → ");
                if (arrowPos != -1) {
                    char existingKeyword[200];
                    substring(existingKeyword, lines[lineCount], 0, arrowPos);
                    if (strcampare(existingKeyword, keyword) == 0) {
                        found = true;
                        strconcat(lines[lineCount], ", ");
                        strconcat(lines[lineCount], seedUrl);
                    }
                }
            }
            lineCount++;
        }
        inFile.close();
    }

    if (lineCount == 0) {
        strcopy(lines[0], website);
        lineCount = 1;
    }

    if (!found) {
        char newLine[500] = "";
        strcopy(newLine, keyword);
        strconcat(newLine, " → ");
        strconcat(newLine, seedUrl);
        strcopy(lines[lineCount], newLine);
        lineCount++;
    }

    ofstream outFile("index");
    for (int i = 0; i < lineCount; i++) {
        outFile << lines[i] << "\n";
    }
    outFile.close();
}


char* give_keyword(const char *text)
{
    char **tokens = tokenize(text);
    if (!tokens)
    {
         cout << "Tokenization failed" <<  endl;
        return nullptr;
    }

    Hash<char*, int> freqMap(100); 

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

        Node<char*, int>* node = freqMap.get(lower);
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

    char* keyword = nullptr;
    int maxFreq = 0;

    for (int i = 0; i < freqMap.getSize(); ++i)
    {
        Node<char*, int>* temp = freqMap.getNode(i);
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
        char* result = new char[strlength(keyword) + 1];
        strcopy(result, keyword);
        return result; 
    }

    return nullptr;
}


void crawler::index_and_Url(const char *filepath, const char *seedUrl)
{   
    ifstream file(filepath);
    if (!file.is_open())
    {
        cout << "Failed to open" << endl;
        return;
    }

    char buffer[10000];
    int i = 0;
    char ch;

    while (file.get(ch) && i < 9999)
    {
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    file.close();

    char cleaned[10000];
    int j = 0;
    bool inside_script = false;
    bool inside_style = false;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (!inside_script && !inside_style && strcasestre(&buffer[i], "<script") == 0)
        {
            inside_script = true;
            while (buffer[i] != '\0' && !(buffer[i] == '<' && strcasestre(&buffer[i], "</script>") == 0))
            {
                i++;
            }
            while (buffer[i] != '\0' && buffer[i] != '>')
            {
                i++;
            }
            continue;
        }

        if (!inside_style && !inside_script && strcasestre(&buffer[i], "<style") == 0)
        {
            inside_style = true;
            while (buffer[i] != '\0' && !(buffer[i] == '<' && strcasestre(&buffer[i], "</style>") == 0))
            {
                i++;
            }
            while (buffer[i] != '\0' && buffer[i] != '>')
            {
                i++;
            }
            continue;
        }

        if (!inside_script && !inside_style)
        {
            cleaned[j++] = buffer[i];
        }

        if (inside_script && buffer[i] == '>' && strcasestre(&buffer[i - 8], "</script>") == 0)
        {
            inside_script = false;
        }

        if (inside_style && buffer[i] == '>' && strcasestre(&buffer[i - 7], "</style>") == 0)
        {
            inside_style = false;
        }
    }
    cleaned[j] = '\0';

    char text[10000];
    j = 0;
    bool inside_tag = false;

    for (int k = 0; cleaned[k] != '\0'; k++)
    {
        if (cleaned[k] == '<')
        {
            inside_tag = true;
        }
        else if (cleaned[k] == '>')
        {
            inside_tag = false;
            continue;
        }

        if (!inside_tag && cleaned[k] != '>')
        {
            text[j++] = cleaned[k];
        }
    }
    text[j] = '\0';

    whitespace(text);
    char *keyword = give_keyword(text);
    if (keyword != nullptr)
    {
        cout << "Keyword: " << keyword << endl;
        saveKeywordURL(keyword,seedUrl);
       
        delete[] keyword;
    }
}

int crawler::extracturl(const char filename[], char *newUrls[], int maxUrls)
{
    char buffer[10000];
    int i = 0;

     ifstream file(filename);
    if (!file.is_open())
    {
         cout << "Failed to open the file: " << filename <<  endl;
        return 0;
    }

    char ch;
    while (file.get(ch) && i < 9999)
    {
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    file.close();

    int count = 0;
    int pos = 0;

    while (pos < strlength(buffer) && count < maxUrls)
    {
        int a_pos = strstre(&buffer[pos], "<a");
        if (a_pos == -1)
            break;

        pos += a_pos;

        int href_pos = strstre(&buffer[pos], "href=");
        if (href_pos == -1)
        {
            pos += 2;
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

        while (buffer[end] != '\0' && buffer[end] != quote && end - start < 999)
        {
            end++;
        }

        if (buffer[end] == '\0')
        {
            pos = end;
            continue;
        }

        int url_len = end - start;
        char *url = new char[url_len + 1];
        for (int k = 0; k < url_len; k++)
        {
            url[k] = buffer[start + k];
        }
        url[url_len] = '\0';

        if (is_html(url) && h.get(url) == nullptr)
        {
            char *safeUrl = new char[url_len + 1];
            strcopy(safeUrl, url);
            newUrls[count++] = safeUrl;
        }
        delete[] url; 

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

     cout << "[Depth " << depth << "] Crawling " << seedUrl <<  endl;

    createdirectory();

    char *filename = uniquename();

    char command[1000] = "";
    strcopy(command, "wget \"");
    strconcat(command, seedUrl);
    strconcat(command, "\" -O \"");
    strconcat(command, targetDirectory);
    strconcat(command, "/");
    strconcat(command, filename);
    strconcat(command, ".html\"");

    if (system(command) != 0)
    {
         cout << "Failed to download: " << seedUrl <<  endl;
        delete[] filename;
        return;
    }

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

    cout << "URL = " << keyCopy <<  endl;
    cout << "FILE = " << fileCopy <<  endl;

    index_and_Url(filepath,seedUrl);

    delete[] filename;

    char *newUrls[1000];
    int foundCount = extracturl(filepath, newUrls, 1000);

    for (int i = 0; i < foundCount; i++)
    {
        wgetrequest(newUrls[i], depth - 1);
        delete[] newUrls[i];
    }
}
