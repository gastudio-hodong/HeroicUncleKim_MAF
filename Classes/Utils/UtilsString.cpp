//
//  UtilsString.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UtilsString.h"

#include "Common/ConfigDefault.h"

USING_NS_CC;

static UtilsString *s_SharedUtilsString = nullptr;
UtilsString* UtilsString::getInstance()
{
    if (!s_SharedUtilsString)
    {
        s_SharedUtilsString = new (std::nothrow) UtilsString();
        s_SharedUtilsString->init();
    }
    
    return s_SharedUtilsString;
}

UtilsString::UtilsString()
{
    
}

UtilsString::~UtilsString()
{
    
}

bool UtilsString::init()
{
    
    return true;
}

std::string UtilsString::format(const char* format, ...)
{
    va_list args;
    std::string ret(512, '\0');

    va_start(args, format);
    int nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
    va_end(args);

    if (nret >= 0)
    {
        if ((unsigned int)nret < ret.length())
        {
            ret.resize(nret);
        }
        else if ((unsigned int)nret > ret.length()) { // VS2015/2017 or later Visual Studio Version
            ret.resize(nret);

            va_start(args, format);
            nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
            va_end(args);

            assert(nret == ret.length());
        }
        // else equals, do nothing.
    }
    else
    { // less or equal VS2013 and Unix System glibc implement.
        do
        {
            ret.resize(ret.length() * 3 / 2);

            va_start(args, format);
            nret = vsnprintf(&ret.front(), ret.length() + 1, format, args);
            va_end(args);

        } while (nret < 0);

        ret.resize(nret);
    }

    return ret;
}

std::string UtilsString::comma(int64_t arg)
{
    std::string ret;
    std::string num;
    num = std::to_string(arg);
    
    double_t fl = (double_t)(num.length() / 3);
    
    while ( fl > 0 )
    {
        std::string buf;
        buf = num.substr(num.length() - 3, num.length());
        
        if(fl != 1)
            ret = "," + buf + ret;
        else
            ret = buf + ret;
        
        num = num.substr(0, num.length() - 3);
        
        fl = (double_t)(num.length() / 3);
    }
    
    if ( num.length() > 0)
    {
        if(ret.length() > 0)
            ret = num + "," + ret;
        else
            ret = num;
    }
    
    return ret;
}

std::string UtilsString::trim(const std::string& str, const char* trim /*= " \t\v\r\n"*/)
{
    std::string ret;
    std::string::size_type left = str.find_first_not_of(trim);
    
    if ( left != std::string::npos )
    {
        std::string::size_type right = str.find_last_not_of(trim);
        ret = str.substr(left, right - left + 1);
    }
    
    return ret;
}

std::vector<std::string> UtilsString::split(const std::string& str, char token)
{
    std::vector<std::string> ret;
    std::istringstream stream(str);
    
    std::string field;
    while ( std::getline(stream, field, token) )
    {
        ret.push_back(field);
    }
    
    return ret;
}

std::string UtilsString::splitJoin(std::vector<std::string> &strings, std::string delim)
{
    return std::accumulate(strings.begin(), strings.end(), std::string(), [&delim](const std::string &x, const std::string &y){
        return x.empty() ? y : x + delim + y;
    });
}

std::string UtilsString::urlencode(const std::string& str)
{
    std::string escaped="";
    size_t max = str.length();
    
    for ( int32_t i = 0; i < max; i++ )
    {
        if((48 <= str[i] && str[i] <= 57) ||//0-9
           (65 <= str[i] && str[i] <= 90) ||//abc...xyz
           (97 <= str[i] && str[i] <= 122) || //ABC...XYZ
           (str[i]=='~' || str[i]=='!' || str[i]=='*' || str[i]=='(' || str[i]==')' || str[i]=='\''))
        {
            escaped.append(&str[i], 1);
        }
        else
        {
            escaped.append( format("%%%02x", str[i] & 0xff) );
        }
    }
    
    return escaped;
}

std::string UtilsString::replaceAll(std::string str, const std::string from, const std::string to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
