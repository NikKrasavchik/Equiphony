#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>
#include <queue>
#include <functional>
#include <string>
#include <codecvt>
#include <locale>
#include "/opt/homebrew/opt/libomp/include/omp.h"

const std::vector<std::string> text = 
{"Сижу за решеткой в темнице сырой.",
"Вскормленный в неволе орел молодой,"
"Мой грустный товарищ, махая крылом,"
"Кровавую пищу клюет под окном,"};

#define RU_LETTER_A     u'а'
#define RU_LETTER_B     u'б'
#define RU_LETTER_V     u'в'
#define RU_LETTER_G     u'г'
#define RU_LETTER_D     u'д'
#define RU_LETTER_YE    u'е'
#define RU_LETTER_YEO   u'ё'
#define RU_LETTER_ZH    u'ж'
#define RU_LETTER_Z     u'з'
#define RU_LETTER_I     u'т'
#define RU_LETTER_IY    u'й'
#define RU_LETTER_K     u'к'
#define RU_LETTER_L     u'л'
#define RU_LETTER_M     u'м'
#define RU_LETTER_N     u'н'
#define RU_LETTER_O     u'о'
#define RU_LETTER_P     u'п'
#define RU_LETTER_R     u'р'
#define RU_LETTER_S     u'с'
#define RU_LETTER_T     u'т'
#define RU_LETTER_U     u'у'
#define RU_LETTER_F     u'ф'
#define RU_LETTER_KH    u'х'
#define RU_LETTER_TS    u'ц'
#define RU_LETTER_CH    u'ч'
#define RU_LETTER_SH    u'ш'
#define RU_LETTER_SHCH  u'щ'
#define RU_LETTER_HARD  u'ъ'
#define RU_LETTER_Y     u'ы'
#define RU_LETTER_SOFT  u'ь'
#define RU_LETTER_E     u'э'
#define RU_LETTER_YU    u'ю'
#define RU_LETTER_YA    u'я'

bool definePhonemotype(char16_t a, char16_t b)
{
    if ((a == RU_LETTER_G || a == RU_LETTER_K || a == RU_LETTER_KH) && (b == RU_LETTER_G || b == RU_LETTER_K || b == RU_LETTER_KH))
        return true;
    if ((a == RU_LETTER_V || a == RU_LETTER_F) && (b == RU_LETTER_V || b == RU_LETTER_F))
        return true;
    if ((a == RU_LETTER_ZH || a == RU_LETTER_SH || a == RU_LETTER_SHCH || a == RU_LETTER_CH) && (b == RU_LETTER_ZH || b == RU_LETTER_SH || b == RU_LETTER_SHCH || b == RU_LETTER_CH))
        return true;
    if ((a == RU_LETTER_D || a == RU_LETTER_T) && (b == RU_LETTER_D || b == RU_LETTER_T))
        return true;
    if ((a == RU_LETTER_Z || a == RU_LETTER_S) && (b == RU_LETTER_Z || b == RU_LETTER_S))
        return true;
    if ((a == RU_LETTER_P || a == RU_LETTER_B) && (b == RU_LETTER_P || b == RU_LETTER_B))
        return true;
    return false;
}

bool defineSonorous(char16_t a, char16_t b)
{
    if ((a == RU_LETTER_M || a == RU_LETTER_N) && (b == RU_LETTER_M || b == RU_LETTER_N))
        return true;
    if ((a == RU_LETTER_R || a == RU_LETTER_L) && (b == RU_LETTER_R || b == RU_LETTER_L))
        return true;
    if ((a == RU_LETTER_B || a == RU_LETTER_V) && (b == RU_LETTER_B || b == RU_LETTER_V))
        return true;
    if ((a == RU_LETTER_P || a == RU_LETTER_F) && (b == RU_LETTER_P || b == RU_LETTER_F))
        return true;
    return false;
}

bool defineHardness(char16_t a, char16_t b)
{
    if ((a == RU_LETTER_SH || a == RU_LETTER_SHCH) && (b == RU_LETTER_SH || b == RU_LETTER_SHCH))
        return true;
    return false;
}

bool defineOcclusive(char16_t a, char16_t b)
{
    if (((a == RU_LETTER_B || a == RU_LETTER_P) && (b == RU_LETTER_D || b == RU_LETTER_T)) && ((b == RU_LETTER_B || b == RU_LETTER_P) && (a == RU_LETTER_D || a == RU_LETTER_T)))
        return true;
    if ((a == RU_LETTER_TS || a == RU_LETTER_CH) && (b == RU_LETTER_TS || b == RU_LETTER_CH))
        return true;
    return false;
}

bool defineDeafness(char16_t a, char16_t b)
{
    if ((a == RU_LETTER_B || a == RU_LETTER_T) && (b == RU_LETTER_B || b == RU_LETTER_T))
        return true;
    if ((a == RU_LETTER_P || a == RU_LETTER_D) && (b == RU_LETTER_P || b == RU_LETTER_D))
        return true;
    return false;
}

bool defineGroup(char16_t a, char16_t b)
{
    return false;
}

bool defineSoftIota(char16_t a, char16_t b)
{
    return false;
}

bool defineHardIota(char16_t a, char16_t b)
{
    return false;
}

double defineCost(char16_t a, char16_t b)
{
    double cost = 0;

    if (a == b)
        cost = 0;
    else if (definePhonemotype(a, b))
        cost += 0.5;
    else if (defineSonorous(a, b))
        cost += 1;
    else if (defineHardness(a, b))
        cost += 1.5;
    else if (defineOcclusive(a, b))
        cost += 2;
    else if (defineDeafness(a, b))
        cost += 2.5;
    else if (defineGroup(a, b))
        cost += 3;
    else if (defineGroup(a, b))
        cost += 2.5;
    else if (defineHardIota(a, b))
        cost += 3.5;

    return cost;
}

std::vector<std::vector<double>> levenshteinDist(std::u16string word1, std::u16string word2) {
    int size1 = word1.size();
    int size2 = word2.size();
    std::vector<std::vector<double>> verif(size1+1, std::vector<double>(size2+1));

    if (size1 == 0)
        return std::vector<std::vector<double>>(0, std::vector<double>(0));
    if (size2 == 0)
        return std::vector<std::vector<double>>(0, std::vector<double>(0));

    for (int i = 0; i <= size1; i++)
        verif[i][0] = i;
    for (int j = 0; j <= size2; j++)
        verif[0][j] = j;

    for (int i = 1; i <= size1; i++) {
        for (int j = 1; j <= size2; j++) {
            double cost = defineCost(word2[j - 1], word1[i - 1]);
            verif[i][j] = fmin(fmin(verif[i - 1][j] + 1, verif[i][j - 1] + 1), verif[i - 1][j - 1] + cost);
        }
    }

    return verif;
}


std::vector<std::vector<double>> findDiagonals(std::vector<std::vector<double>> vec)
{
    std::vector<std::vector<double>> res;
    for (int i = vec.size()-1; i >= 0; i--)
    {
        std::vector<double> tres;
        for (int j = 0; j < vec.size() - i; j++)
        {
            tres.push_back(vec[i+j][j]);
        }
        res.push_back(tres);
    }
    
    for (int i = 1; i < vec[0].size()-1; i++)
    {
        std::vector<double> tres;
        for (int j = i; j < vec[0].size(); j++)
        {
            tres.push_back(vec[j - i][j]);
        }
        res.push_back(tres);
    }
    return res;
}

int main()
{
    std::vector<std::vector<std::vector<double>>> res;
    for (int i = 0; i < 4; i++)
        res.push_back(std::vector<std::vector<double>>());

    #pragma omp parallel
    {
        int threadNum = omp_get_thread_num();

         std::u16string str1;
         std::u16string str2;
        switch (threadNum)
        {
        case 0:
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
            str1 = utf16conv.from_bytes(text[0]);
            str2 = utf16conv.from_bytes(text[1]);
            break;
            
        // case 1:
        //     std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
        //     str1 = utf16conv.from_bytes(text[2]);
        //     str2 = utf16conv.from_bytes(text[3]);
        //     break;

        // case 2:
        //     std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
        //     str1 = utf16conv.from_bytes(text[0]);
        //     str2 = utf16conv.from_bytes(text[2]);
        //     break;

        // case 3:
        //     std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
        //     str1 = utf16conv.from_bytes(text[1]);
        //     str2 = utf16conv.from_bytes(text[3]);
        //     break;
        }
        std::vector<std::vector<double>> levRes = levenshteinDist(str1, str2);
        // std::vector<std::vector<double>> diagRes = findDiagonals(levRes);

        for (int i = 0; i < levRes.size(); i++)
        {
            for (int j = 0; j < levRes[i].size(); j++)
            {
                std::cout << levRes[i][j] << " ";
            }
            std::cout << std::endl;
        }

    //     res[threadNum] = diagRes;
    }
    return 0;
}
