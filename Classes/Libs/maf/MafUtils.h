//
//  Maf.h
//  FantasyClicker
//
//  Created by SanHeo on 2015. 4. 15..
//
//

#ifndef __FantasyClicker__MafUtils__
#define __FantasyClicker__MafUtils__

#include <stdio.h>
#include "cocos2d.h"

namespace MafUtils {
    
    template<typename T>
    std::string toString(const T& value)
    {
        std::stringstream oss;
        oss << value;
        return oss.str();
    }

    int toInt(std::string &str);
    
    bool isDigit(const char value);
    bool isNumeric(const std::string& value);
    
    
    std::string format(const char* format, ...) CC_FORMAT_PRINTF(1, 2);
    
    
    std::string addCommaToString(std::string &input,int interval = 3,char separator = '.');
    
    std::string convertNumberToShort(std::string &input,bool bGap=false);
    std::string convertNumberToTime(double time, bool isHour = false);
    
    static char charTitle[1024] = {0,};
    std::string doubleToString(const double value);
    //수의 단위
    /*
     http://kin.naver.com/qna/detail.nhn?d1id=11&dirId=1113&docId=58954047&qb=cXVhZHJpbGxpb24=&enc=utf8&section=kin&rank=5&search_sort=0&spq=0&pid=Sf3CxdpySDGsschkKM8sssssssC-185259&sid=PLXGL0l2qByasSBseLbxWA%3D%3D
     */
    
    
    int CharToInt(const char c);
    std::string bigAddNum(std::string &aaa,std::string bbb,bool bIgnore=true);
    std::string bigSubNum(std::string &aaa,std::string bbb,bool bIgnore=true);
    std::string bigMulNum(std::string &aaa,std::string bbb,bool bIgnore=true);
    //    std::string bigDivNum(std::string aaa,std::string bbb);
    bool compareStr(std::string &aaa,std::string bbb);
    
    
    std::string TrimRight(const std::string &s, const std::string& whitespaces=" \t\r\n");
    
    static std::chrono::system_clock::time_point time;
    void startCheckTime();
    void endCheckTime();
    
    enum E_TIME{
        YEAR,MONTH,WEEK,YDAY,WDAY,DAY,HOUR,MIN,SECOND,
    };
    int GetTimeData(E_TIME etime);
    int Get1970Day();
    int Get1970Week(int dayOfweek);
    
    bool WriteTextFile(std::string path, std::string buf);
    bool WriteDataFile(std::string path, const std::vector<char>* buf);
    void CheckFile(char* name);
    
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
    std::string splitCreate(const std::vector<std::string> &s, char delim);
    
//    bool strCheck(const char *str);
//    int strlen_ko(const char *str);
    
    std::string eraseCharcter(std::string& str, char c);
    
    void ChangeLabel(cocos2d::Label* label, std::string text, cocos2d::Color3B color, bool all=false);
    std::string ChangeLPAD(int num, int Digits);
}

#endif /* defined(__FantasyClicker__MafUtils__) */
