//
//  UtilsString.h
//  FantasyClicker-mobile
//
//  Created by 이명열 on 2016. 4. 20..
//
//

#ifndef UtilsString_h
#define UtilsString_h

#include "cocos2d.h"
#include <string>
#include <iomanip>

class UtilsString
{
public:
    static UtilsString* getInstance();
    
    UtilsString(void);
    virtual ~UtilsString(void);
    virtual bool init();
    
public:
    std::string format(const char* format, ...);
    
    std::string comma(const int64_t num);
    
    std::string trim(const std::string& str, const char* trim = " \t\v\r\n");
    
    std::vector<std::string> split(const std::string& str, char token);
    std::string splitJoin(std::vector<std::string> &strings, std::string delim);
    
    std::string urlencode(const std::string& str);
    
    std::string replaceAll(std::string str, const std::string from, const std::string to);

private:
    
private:
    
};

#endif /* UtilsString_h */
