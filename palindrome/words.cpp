#include<string>
#include<cctype>

using namespace std;

string format(string str)
{
    for(int i = 0; i < str.size(); i++) {
        str[i] = toupper(str[i]);
        if(!isalpha(str[i])) {
            str.erase(i,1);
            i--;
        }
    }

    return str;
}

void reverse(string str1, char str2[])
{
    int len = size(str1);

    for(int i = 0; i < len; i++) {
        str2[i] = str1[len - 1 - i];
    }

    str2[len] = '\0';
}

int compare(string one, string two)
{
    one = format(one);
    two = format(two);

    if(one.size() == 0 && two.size() == 0) {
        return 1;
    }

    if(one[0] == two[0]) {
        one = one.erase(0,1);
        two = two.erase(0,1);
        if(compare(one, two)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    /* iterative version
    if(format(one) == format(two)) {
        return 1;
    } else {
        return 0;
    }
    */
}

int palindrome(string sentence)
{
    char reversed[sentence.size()];
    reverse(sentence, reversed);

    if(compare(sentence, reversed)) {
        return 1;
    } else {
        return 0;
    }
}

int anagram(string str1, string str2)
{
    str1 = format(str1);
    str2 = format(str2);
    
    for(int i = 'A'; i <= 'Z'; i++) {
        int count1 = 0, count2 = 0;
        for(int j = 0; j < str1.size(); j++) {
            if(str1[j] == i) {
                count1++;
            }
            if(str2[j] == i) {
                count2++;
            }
        }
        if(count1 != count2) {
            return 0;
        }
    }

    return 1;
}

/*
Questions
1. What happens when you str.erase an empty string?
2. How to debug if(type:string)?
3. Is using string library in this exercise allowed?
4. How does C++ treat string variable vs char array?
5. Return 0 or 1 -> int or bool?
*/

