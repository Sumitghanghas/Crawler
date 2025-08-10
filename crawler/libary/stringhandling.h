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
    if (!data)
        return nullptr;

    int maxTokens = 1000;
    char **tokens = new char *[maxTokens];
    int count = 0;
    int i = 0;

    while (data[i] != '\0' && count < maxTokens - 1)
    {
        while (data[i] == ' ' || data[i] == ',' || data[i] == '\n')
        {
            i++;
        }

        if (data[i] != '\0')
        {
            char word[100];
            int wordcount = 0;

            while (data[i] != '\0' && data[i] != ' ' && data[i] != ',' && data[i] != '\n')
            {
                if (wordcount < 99)
                {
                    word[wordcount++] = data[i++];
                }
                else
                {
                    i++;
                }
            }

            word[wordcount] = '\0';

            tokens[count] = new char[wordcount + 1];
            for (int j = 0; j <= wordcount; j++)
            {
                tokens[count][j] = word[j];
            }

            count++;
        }
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
        "the", "is", "in", "and", "or", "of", "to", "a", "an", "for", "on", "at", "by", "this",
        "that", "with", "from", "as", "it", "are", "was", "were", "be", "has", "have", "had",
        "but", "not", "so", "if", "then", "else", "which", "what", "when", "where", "who", "whom"
    };
    int stopCount = sizeof(stopWords) / sizeof(stopWords[0]);
    for (int i = 0; i < stopCount; ++i) {
        if (strcampare(word, stopWords[i]) == 0) {
            return true;
        }
    }
    return false;
}



// char* give_keyword(const char *text)
// {
//     char **tokens = tokenize(text);
//     if (!tokens)
//     {
//         cout << "Tokenization failed." << endl;
//         return nullptr;
//     }

//     char* words[1000];
//     int freq[1000];
//     int count = 0;

//     for (int i = 0; tokens[i] != nullptr; ++i)
//     {
//         if (!is_valid_word(tokens[i]))
//         {
//             delete[] tokens[i];
//             continue;
//         }

//         char *lower = to_lowercase(tokens[i]);
//         delete[] tokens[i];

//         if (is_stop_word(lower))
//         {
//             delete[] lower;
//             continue;
//         }

//         bool found = false;
//         for (int j = 0; j < count; ++j)
//         {
//             if (strcampare(words[j], lower) == 0)
//             {
//                 freq[j]++;
//                 found = true;
//                 break;
//             }
//         }

//         if (!found)
//         {
//             if (count >= 1000)
//             {
//                 delete[] lower;
//                 break; 
//             }

//             words[count] = lower; 
//             freq[count] = 1;
//             count++;
//         }
//         else
//         {
//             delete[] lower;
//         }
//     }

//     delete[] tokens;

//     int maxFreq = 0;
//     int maxIndex = -1;
//     for (int i = 0; i < count; ++i)
//     {
//         if (freq[i] > maxFreq)
//         {
//             maxFreq = freq[i];
//             maxIndex = i;
//         }
//     }

//     if (maxIndex != -1)
//     {
//         char *result = new char[strlength(words[maxIndex]) + 1];
//         strcopy(result, words[maxIndex]);

//         for (int i = 0; i < count; ++i)
//         {
//             if (i != maxIndex)
//                 delete[] words[i];
//         } 
//         return result;
//     }

//     for (int i = 0; i < count; ++i)
//         delete[] words[i];

//     return nullptr;
// }
