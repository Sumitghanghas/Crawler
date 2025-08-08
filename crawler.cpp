#include <iostream>
#include <fstream>
#include <ctime>
#include <direct.h>
#include <cstdlib>
#include "crawler.h"

crawler::crawler(int md, char *directory) : h(10)
{
    maxdepth = md;
    targetDirectory = directory;
}

void crawler::createdirectory()
{
    if (_mkdir(targetDirectory) == 0)
    {
        cout << "Directory created" << "\n";
    }
    else
    {
        cout << "Directory already exist or failed to create" << "\n";
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

void crawler::index_and_Url(const char *filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cout << "Failed to open file: " << filepath << endl;
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

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (!inside_script && strcasestre(&buffer[i], "<script") == 0)
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

        if (!inside_script)
        {
            cleaned[j++] = buffer[i];
        }

        if (inside_script && buffer[i] == '>' && strcasestre(&buffer[i - 8], "</script>") == 0)
        {
            inside_script = false;
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
   if (keyword != nullptr) {
    cout << "hello " << keyword << endl;
    delete[] keyword;
}

    return;
}
void crawler::parse_all_files()
{

    int file = h.getSize();

    for (int i = 0; i < file; i++)
    {
        Node<char *, char *> *current = h.getNode(i);
        while (current != nullptr)
        {
            cout << "\nURL =" << current->key << endl;
            cout << "FILE =" << current->data << endl;

            index_and_Url(current->data);
            current = current->next;
        }
    }
}

int crawler::extracturl(const char filename[], char *newUrls[], int maxUrls)
{
    char buffer[10000];
    int i = 0;

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open the file: " << filename << endl;
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

        char *url = new char[1000];
        int url_len = 0;
        for (int k = start; k < end && url_len < 999; k++)
        {
            url[url_len++] = buffer[k];
        }
        url[url_len] = '\0';

        if (is_html(url) && h.get(url) == nullptr)
        {
            newUrls[count++] = url;
        }
        else
        {
            delete[] url;
        }
        pos = end + 1;
    }

    return count;
}

void crawler::wgetrequest(char *seedUrl, int depth)
{
    if (depth == -1)
    {
        parse_all_files();
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

    cout << "[Depth " << depth << "] Crawling " << seedUrl << endl;

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
        cout << "Failed to download: " << seedUrl << endl;

        delete[] filename;
        return;
    }

    char filepath[1000];
    strcopy(filepath, targetDirectory);
    strconcat(filepath, "/");
    strconcat(filepath, filename);
    strconcat(filepath, ".html");
    h.insert(seedUrl, filepath);
    h.display();
    delete[] filename;

    char *newUrls[1000];
    int foundCount = extracturl(filepath, newUrls, 1000);

    for (int i = 0; i < foundCount; i++)
    {
        wgetrequest(newUrls[i], depth - 1);
        delete[] newUrls[i];
    }
}
