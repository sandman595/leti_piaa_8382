#include <iostream>
#include <vector>
#include <string>

#define OUTPUT

std::vector<int> prefixFunction (std::string s)
{
    std::vector<int> pi;
    pi.push_back(0);

#ifdef OUTPUT
    std::cout << "PrefixFunction calculation:" << std::endl;
    std::cout << "Pattern: ";
    for(auto ch: s)
        std::cout << ch << " ";
    std::cout << std::endl;
    std::cout << "pi = [0]" << std::endl;
#endif


    for(int i = 1, j = 0; i < s.size(); i++) {

        if(s[i] == s[j]) {
#ifdef OUTPUT
            std::cout << "s[" << i << "]" << " == " << "s[" << j << "] " << std::endl;
#endif
            pi.push_back(j + 1);
            j++;
        } else {
#ifdef OUTPUT
            std::cout << "s[" << i << "]" << " != " << "s[" << j << "] " << "; ";
#endif
            if(j == 0) {
                pi.push_back(0);
            } else {
#ifdef OUTPUT
                std::cout << "j => " << pi[j-1] << std::endl;
#endif
                j = pi[j-1];
                i--;
            }
        }

#ifdef OUTPUT
        std::cout << std::endl;
        std::cout << "Pattern: ";
        for(auto ch: s)
            std::cout << ch << " ";
        std::cout << std::endl;
        std::cout << "pi = [";
        for(int i = 0 ; i < pi.size() - 1; i++)
            std::cout << pi[i] << ", ";
        std::cout << pi[pi.size() - 1];
        std::cout << "]" << std::endl;
#endif
    }

    return pi;
}

void createPart(const std::string& text, std::string& part, int countSymbols, int index)
{
    for(int i = index, j = 0 ; i < index + countSymbols; i++, j++)
        part.push_back(text[i]);
}

std::vector<std::string> split(const std::string& text, const std::string& pattern, int lengthSmallPart, int k)
{
    std::vector<std::string> parts;
    if(k == 1) {
        parts.push_back(text);
        return parts;
    }
    int lengthText = text.size();
    int lengthPattern = pattern.size();

    int lengthBigPart = lengthSmallPart + 1;

    int countBigParts = lengthText % k;
    int countSmallParts = k - countBigParts;

    int lengthExtendedBigPart = lengthBigPart + (lengthPattern - 1);
    int lengthExtendedSmallPart = lengthSmallPart + (lengthPattern - 1);

    int i, indexText = 0;
    for(i = 0; i < countBigParts; i++, indexText += lengthBigPart) {
        std::string part;
        createPart(text, part, lengthExtendedBigPart, indexText);
        parts.push_back(part);
    }

    for(i = 0; i < countSmallParts - 1; i++, indexText += lengthSmallPart) {
        std::string part;
        createPart(text, part, lengthExtendedSmallPart, indexText);
        parts.push_back(part);
    }

    std::string finalPart;
    int lengthFinalPart = lengthText - indexText;
    createPart(text, finalPart, lengthFinalPart, indexText);
    parts.push_back(finalPart);

#ifdef OUTPUT
    std::cout << "Text is divided into " << countBigParts << " big parts with length = " << lengthExtendedBigPart << ","<< std::endl;
    std::cout << "into " << countSmallParts - 1 << " small parts with length = " << lengthExtendedSmallPart << std::endl;
    std::cout << "and into final part with length = " << lengthFinalPart << ":"<< std::endl;
    for(auto part: parts)
        std::cout << part << " ";
    std::cout << std::endl;
#endif

    return parts;
}

std::vector<int> KMP(const std::string& text, const std::string& pattern, const std::vector<int>& pi)
{
    std::vector<int> answer;
#ifdef OUTPUT
    std::cout << std::endl << "KMP for part: ";
    for(auto ch: text)
        std::cout << ch << " ";
    std::cout << std::endl;
#endif
    int textIndex = 0;
    int patternIndex = 0;
    int lengthText = text.size();
    int lengthPattern = pattern.size();
    while(textIndex < lengthText) {
#ifdef OUTPUT
        std::cout << std::endl << "Part: ";
        for(auto ch: text)
            std::cout << ch << " ";
        std::cout << std::endl;
        std::cout << "Pattern: ";
        for(auto ch: pattern)
            std::cout << ch << " ";
        std::cout << std::endl;
        std::cout << "PrefixFunction of pattern: [";
        for(int i = 0; i < pi.size() - 1; i++)
            std::cout << pi[i] << ", ";
        std::cout << pi[pi.size() - 1] << "]" << std::endl << std::endl;
#endif
        if(text[textIndex] == pattern[patternIndex]) {
#ifdef OUTPUT
            std::cout << "text[" << textIndex << "] == pattern[" << patternIndex << "]" << std::endl;
#endif
            textIndex++;
            patternIndex++;
            if(patternIndex == lengthPattern) {
                answer.push_back(textIndex - patternIndex);
#ifdef OUTPUT
                std::cout << "Substring found!" << std::endl;
#endif
            }
        } else {
#ifdef OUTPUT
            std::cout << "text[" << textIndex << "] != pattern[" << patternIndex << "]" << std::endl;
#endif
            if(patternIndex == 0) {
                textIndex++;
            } else {
                patternIndex = pi[patternIndex-1];
            }
        }
    }
    return answer;
}

void KMP()
{
    std::vector<int> answer;
    int k;
    std::vector<std::string> parts;
    std::string pattern, text;

#ifdef OUTPUT
    std::cout << "Enter a pattern:" << std::endl;
#endif
    std::cin >> pattern;
#ifdef OUTPUT
    std::cout << "Enter text: " << std::endl;
#endif
    std::cin >> text;

    int lengthText = text.size();
    int lengthPattern = pattern.size();

    if(lengthText < lengthPattern) {
        std::cout << "Text less than pattern!" << std::endl;
        return;
    }
#ifdef OUTPUT
    std::cout << "Enter the number of parts to search: " << std::endl;
#endif
    std::cin >> k;

    int lengthSmallPart = lengthText / k;

    if(lengthSmallPart < pattern.size()) {
        std::cout << "Length one part can not less pattern size!" << std::endl;
        return;
    }
    if(k <= 0) {
        std::cout << "Count of parts can not equal or less 0!" << std::endl;
        return;
    }

#ifdef OUTPUT
    std::cout << "Text will be divided into " << k << " parts" << std::endl;
#endif
    parts = split(text, pattern, lengthSmallPart, k);

    std::vector<int> pi = prefixFunction(pattern);

    std::vector<int> occurrences;
    for(int i = 0, indexText = 0; i < parts.size(); i++) {
        occurrences = KMP(parts[i], pattern, pi);
        for(int j = 0; j < occurrences.size(); j++) {
            answer.push_back(occurrences[j] + indexText);
        }
        indexText += parts[i].size() - lengthPattern + 1;
    }

    if(answer.empty())
        std::cout << -1;
    else {
#ifdef OUTPUT
        std::cout << "All occurrences of the pattern in text:" << std::endl;
#endif
        for (int i = 0; i < answer.size() - 1; i++)
            std::cout << answer[i] << ",";
        std::cout << answer[answer.size() - 1] << std::endl;
    }

}


void cycleShift(std::string a, std::string b)
{
    if(a.size() != b.size()) {
#ifdef OUTPUT
        std::cout << "Lengths of strings a and b are different!" << std::endl;
#endif
        std::cout << -1;
        return;
    }

    a += a;
#ifdef OUTPUT
    std::cout << "String a += a: " << a << std::endl;
#endif

    std::vector<int> pi = prefixFunction(b);

    std::vector<int> occurrences = KMP(a, b, pi);

    if(occurrences.empty()) {
#ifdef OUTPUT
        std::cout << "A is not cyclic shift B!" << std::endl;
#endif
        std::cout << "-1";
    } else {
#ifdef OUTPUT
        std::cout << "B string start index in A: ";
#endif
        std::cout << occurrences[0];
    }
}

void cycleShift()
{
    std::string a, b;
#ifdef OUTPUT
    std::cout << "Enter string a:" << std::endl;
#endif
    std::cin >> a;
#ifdef OUTPUT
    std::cout << "Enter string b:" << std::endl;
#endif
    std::cin >> b;
    cycleShift(a, b);
}

int main()
{
    //KMP();
    cycleShift();
    return 0;
}
