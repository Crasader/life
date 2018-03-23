//
//  GameUtils.cpp
//  simple-son
//
//  Created by ff on 15/10/21.
//
//

#include "GameUtils.h"

cocos2d::Size GameUtils::winSize;

std::vector<std::string> GameUtils::splitStr(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    std::string::size_type size = str.size();
    
    for (std::string::size_type i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void GameUtils::DealWithRectString(std::string strTemp, cocos2d::Rect &rect)
{
    /************************************************************************/
    // 处理范围字符串
    // 1. 判断字符串的格式是否正确
    int nTemp = static_cast<int>(count(strTemp.begin(), strTemp.end(), ','));
    if (nTemp != 3)
        return;
    
    // 2. 替换字符串中的"(" , ")" , ","为空的字符
    replace(strTemp.begin(), strTemp.end(), '(', ' ');
    replace(strTemp.begin(), strTemp.end(), ')', ' ');
    replace(strTemp.begin(), strTemp.end(), ',', ' ');
    
    // 3. 解析该字符串并输出到Rect中去
    float fTemp[4] = { 0 };
    std::istringstream stream(strTemp);
    for (int nIndex = 0; nIndex < 4; ++nIndex)
    {
        stream >> fTemp[nIndex];
    }
    
    rect = cocos2d::Rect(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
}

/**
 * 获取指定长度的随机数向量
 *
 * @param resVec std::vector<int> & 随机数向量
 * @param arrayLen int 向量长度
 */
void GameUtils::getRandArray(std::vector<int> &resVec, int arrayLen) {
    resVec.clear();
    for (int i = 0; i < arrayLen; i++) {
        resVec.push_back(i + 1);
    }
    int temp1, temp2;
    for (int i = 0; i < arrayLen / 2; i++) {
        temp1 = resVec[i];
        temp2 = abs(rand())%arrayLen;
        resVec[i] = resVec[temp2];
        resVec[temp2] = temp1;
    }
}

std::string GameUtils::format(const char* format, ...)
{
    char buff[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buff, 1024, format, ap);
    va_end(ap);
    std::string str = buff;
    return str;
}

cocos2d::Vec3 GameUtils::RGB2HSV(cocos2d::Color3B val)
{
    cocos2d::Vec3 result;
    int max = MAX(val.r, val.g);
    max = MAX(max, val.b);
    int min = MIN(val.r, val.g);
    min = MIN(min, val.b);
    if (val.r == max) {
        result.x = (val.g - val.b)/(max - min);
    }else if (val.g == max) {
        result.x = 2 + (val.b - val.r)/(max - min);
    }else if (val.b == max) {
        result.x = 2 + (val.r - val.g)/(max - min);
    }
    
    result.x *= 60;
    if (result.x < 0) {
        result.x += 360;
    }
    
    result.z = max*100/255;
    result.y = (max - min)/max*100;
    return result;
    
//    max=max(R,G,B)
//    min=min(R,G,B)
//    if R = max, H = (G-B)/(max-min)
//        if G = max, H = 2 + (B-R)/(max-min)
//            if B = max, H = 4 + (R-G)/(max-min)
//
//                H = H * 60
//                if H < 0, H = H + 360
// 
//                    V=max(R,G,B)
//                    S=(max-min)/max
}

cocos2d::Color3B GameUtils::HSV2RGB(cocos2d::Vec3 val)
{
    val.z = val.z*255/100;
    val.y = val.y/100;
    cocos2d::Color3B color(cocos2d::Color3B::BLACK);
    if (val.y == 0) {
        color.r = color.g = color.b = val.z;
    }else{
        val.x /= 60;
    }
    int tempH = (int)val.x;
    
    float f = val.x - tempH;
    float a = val.z * (1 - val.y);
    float b = val.z * (1 - val.y * f);
    float c = val.z * (1 - val.y * (1 - f));
//    if s = 0
//        2: R=G=B=V
//        3: else
//            4: H /= 60;
//    5: i = INTEGER(H)
//    6:
//    7: f = H - i
//    8: a = V * ( 1 - s )
//    9: b = V * ( 1 - s * f )
//    10: c = V * ( 1 - s * (1 - f ) )
    
    switch (tempH) {
        case 0:
        {
            color.r = val.z;
            color.g = c;
            color.b = a;
        }
            break;
        case 1:
        {
            color.r = b;
            color.g = val.z;
            color.b = a;
        }
            break;
        case 2:
        {
            color.r = a;
            color.g = val.z;
            color.b = c;
        }
            break;
        case 3:
        {
            color.r = a;
            color.g = b;
            color.b = val.z;
        }
            break;
        case 4:
        {
            color.r = c;
            color.g = a;
            color.b = val.z;
        }
            break;
        case 5:
        {
            color.r = val.z;
            color.g = a;
            color.b = b;
        }
            break;
            
        default:
            break;
    }
    
    return color;
//    11:
//    12: switch(i)
//    13: case 0: R = V; G = c; B = a;
//    14: case 1: R = b; G = v; B = a;
//    15: case 2: R = a; G = v; B = c;
//    16: case 3: R = a; G = b; B = v;
//    17: case 4: R = c; G = a; B = v; 
//    18: case 5: R = v; G = a; B = b;
}
