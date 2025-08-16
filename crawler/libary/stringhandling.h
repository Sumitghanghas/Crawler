#include <iostream>
using namespace std;

int strlength(const char dest[])
{
    int i = 0;
    while (dest[i] != '\0')
    {
        i++;
    }
    return i;
}

int strcampare(const char dest[], const char src[])
{
    int i = 0;
    while (dest[i] != '\0' && src[i] != '\0')
    {
        if (dest[i] != src[i])
        {
            return dest[i] - src[i];
        }
        i++;
    }
    return dest[i] - src[i];
}

void strcopy(char dest[], const char src[])
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return;
}

void strconcat(char dest[], const char src[])
{
    int length = strlength(dest);
    int j = 0;
    int i = length;
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return;
}

int strchar(const char src[], char a)
{
    for (int i = 0; src[i] != '\0'; i++)
    {
        if (src[i] - a == 0)
        {
            return i;
        }
    }
    return -1;
}

int strstre(const char hashstack[], const char needle[])
{
    if (!needle || needle[0] == '\0')
    {
        return 0;
    }
    for (int i = 0; hashstack[i] != '\0'; i++)
    {
        int j = 0;
        while (needle[j] != '\0' && hashstack[i + j] == needle[j])
        {
            j++;
        }
        if (needle[j] == '\0')
        {
            return i;
        }
    }
    return -1;
}

void reverse_inplace(char src[])
{
    int length = strlength(src);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = src[length - 1 - i];
        src[length - 1 - i] = src[i];
        src[i] = temp;
    }
}

int is_palindrome(const char src[])
{
    int length = strlength(src);
    int i = 0;
    int j = length - 1;
    while (i < length / 2)
    {
        if (src[i] != src[j])
        {
            return 0;
        }
        i++;
        j--;
    }
    return 1;
}

int count_words(const char src[])
{
    int count = 0;
    int word = 0;
    int i = 0;
    while (src[i] != '\0')
    {
        if (src[i] != ' ' && src[i] != '\n' && src[i] != '\t')
        {
            if (!word)
            {
                word = 1;
                count++;
            }
        }
        else
        {
            word = 0;
        }
        i++;
    }

    return count;
}

char **tokenize(const char data[])
{
    if (!data) return nullptr;

    const int maxTokens = 1000;
    char **tokens = new char*[maxTokens];
    int count = 0;

    int len = strlength(data);
    int i = 0;

    while (i < len && count < maxTokens - 1) {
        while (i < len && !( (data[i] >= 'A' && data[i] <= 'Z') ||
                             (data[i] >= 'a' && data[i] <= 'z') ||
                             (data[i] >= '0' && data[i] <= '9') )) {
            ++i;
        }
        if (i >= len) break;

        int start = i;
        while (i < len && ( (data[i] >= 'A' && data[i] <= 'Z') ||
                            (data[i] >= 'a' && data[i] <= 'z') ||
                            (data[i] >= '0' && data[i] <= '9') )) {
            ++i;
        }
        int wlen = i - start;
        tokens[count] = new char[wlen + 1];
        for (int k = 0; k < wlen; ++k) tokens[count][k] = data[start + k];
        tokens[count][wlen] = '\0';
        count++;
    }
    tokens[count] = nullptr;
    return tokens;
}

int lowercase(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return ch + 32;
    }
    return ch;
}

int strcasestre(const char haystack[], const char needle[])
{
    if (!needle || needle[0] == '\0')
    {
        return 0;
    }

    for (int i = 0; haystack[i] != '\0'; i++)
    {
        int j = 0;
        while (needle[j] != '\0' && lowercase(haystack[i + j]) == lowercase(needle[j]))
        {
            j++;
        }

        if (needle[j] == '\0')
        {
            return i;
        }
    }

    return -1;
}

void whitespace(char src[])
{
    int i = 0, j = 0;
    int inword = 0;

    while (src[i] == ' ' || src[i] == '\t' || src[i] == '\n')
    {
        i++;
    }

    while (src[i] != '\0')
    {
        if (src[i] != ' ' && src[i] != '\t' && src[i] != '\n')
        {
            src[j++] = src[i++];
            inword = 1;
        }
        else
        {
            while (src[i] == ' ' || src[i] == '\t' || src[i] == '\n')
            {
                i++;
            }
            if (inword)
            {
                src[j++] = ' ';
                inword = 0;
            }
        }
    }

    if (j > 0 && src[j - 1] == ' ')
    {
        j--;
    }

    src[j] = '\0';
}
void substring(char dest[], const char src[], int start, int end)
{
    int j = 0;
    for (int i = start; i < end && src[i] != '\0'; i++)
    {
        dest[j++] = src[i];
    }
    dest[j] = '\0';
}

bool is_valid_word(const char word[])
{
    int len = strlength(word);
    if (len == 0)
        return false;

    if (word[0] == '&' && word[len - 1] == ';')
        return false;

    for (int i = 0; i < len; i++)
    {
        if (word[i] == '=' || word[i] == ';' || word[i] == '(' || word[i] == ')' || word[i] == '{' || word[i] == '}')
            return false;
    }

    int a = 0;
    for (int i = 0; i < len; i++)
    {
        if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
        {
            a++;
        }
    }

    return a > 0;
}

char *to_lowercase(const char *word) {
    int len = strlength(word);
    char *lower = new char[len + 1];
    for (int i = 0; i < len; ++i) {
        lower[i] = lowercase(word[i]);
    }
    lower[len] = '\0';
    return lower;
}

bool is_stop_word(const char *word) {
    const char *stopWords[] = {
        "nbsp",
    "a",
    "an",
    "and",
    "are",
    "as",
    "at",
    "be",
    "by",
    "for",
    "from",
    "has",
    "he",
    "in",
    "is",
    "it",
    "its",
    "of",
    "on",
    "that",
    "the",
    "to",
    "was",
    "will",
    "with",
    "would",
    "you",
    "your",
    "have",
    "had",
    "this",
    "these",
    "they",
    "them",
    "their",
    "there",
    "then",
    "than",
    "or",
    "but",
    "not",
    "can",
    "could",
    "should",
    "would",
    "may",
    "might",
    "must",
    "shall",
    "will",
    "do",
    "does",
    "did",
    "done",
    "been",
    "being",
    "about",
    "after",
    "all",
    "also",
    "any",
    "because",
    "before",
    "both",
    "but",
    "each",
    "even",
    "first",
    "her",
    "here",
    "him",
    "his",
    "how",
    "if",
    "into",
    "just",
    "like",
    "make",
    "many",
    "me",
    "more",
    "most",
    "my",
    "new",
    "no",
    "now",
    "only",
    "other",
    "our",
    "out",
    "over",
    "said",
    "same",
    "see",
    "she",
    "so",
    "some",
    "such",
    "take",
    "time",
    "two",
    "up",
    "use",
    "very",
    "way",
    "we",
    "well",
    "were",
    "what",
    "when",
    "where",
    "which",
    "who",
    "why",
    "work",
    "page",
    "home",
    "contact",
    "about",
    "services",
    "products",
    "news",
    "blog",
    "login",
    "register",
    "search",
    "help",
    "support",
    "privacy",
    "terms",
    "policy",
    "www",
    "quot",
    "http",
    "https",
    "com",
    "org",
    "net",
    "edu",
    "gov",
    "io",
    "co",
    "uk",
    "us",
    "info",
    "site",
    "online",
    "web",
    "page",
    "html",
    "www",
    "www.",
    "http://",
    "https://",
    };
    int stopCount = sizeof(stopWords) / sizeof(stopWords[0]);
    for (int i = 0; i < stopCount; ++i) {
        if (strcampare(word, stopWords[i]) == 0) {
            return true;
        }
    }
    return false;
}

void removeScriptAndStyle(const char *input, char *output)
{
    int in_len = strlength(input);
    int j = 0;
    bool inside_tag = false;
    bool inside_script = false;
    bool inside_style = false;

    for (int i = 0; i < in_len; i++)
    {
        if (!inside_script && !inside_style && !inside_tag && strcasestre(&input[i], "<script") == 0)
        {
            inside_script = true;
            while (i < in_len && input[i] != '>')
                i++;
            continue;
        }

        if (inside_script && strcasestre(&input[i], "</script>") == 0)
        {
            while (i < in_len && input[i] != '>')
                i++;
            inside_script = false;
            continue;
        }

        if (!inside_script && !inside_style && !inside_tag && strcasestre(&input[i], "<style") == 0)
        {
            inside_style = true;
            while (i < in_len && input[i] != '>')
                i++;
            continue;
        }

        if (inside_style && strcasestre(&input[i], "</style>") == 0)
        {
            while (i < in_len && input[i] != '>')
                i++;
            inside_style = false;
            continue;
        }

        if (!inside_script && !inside_style)
        {
            if (input[i] == '<')
            {
                inside_tag = true;
            }
            else if (input[i] == '>')
            {
                inside_tag = false;
                continue;
            }
            else if (!inside_tag)
            {
                output[j++] = input[i]; 
            }
        }
    }
    output[j] = '\0';
}

void stripHTMLTags(const char *input, char *output)
{
    int j = 0;
    bool inside_tag = false;
    for (size_t i = 0; input[i] != '\0'; ++i)
    {
        if (input[i] == '<') {
            inside_tag = true;
            continue;
        }
        if (input[i] == '>') {
            inside_tag = false;
            continue;
        }
        if (!inside_tag) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

// char* escape_url(const char* url) {
//     int len = strlength(url);
//     char* escaped = new char[len * 2 + 1];
//     int j = 0;
//     for (int i = 0; i < len; i++) {
//         if (url[i] == '"' || url[i] == '\\') {
//             escaped[j++] = '\\';
//         }
//         escaped[j++] = url[i];
//     }
//     escaped[j] = '\0';
//     return escaped;
// }
