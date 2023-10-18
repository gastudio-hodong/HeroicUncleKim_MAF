//
//  UtilsDate.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 10/10/2019.
//

#include "UtilsDate.h"

USING_NS_CC;

static UtilsDate *s_SharedUtilsDate = nullptr;

UtilsDate* UtilsDate::getInstance()
{
    if (!s_SharedUtilsDate)
    {
        s_SharedUtilsDate = new (std::nothrow) UtilsDate();
        s_SharedUtilsDate->init();
    }
    
    return s_SharedUtilsDate;
}

UtilsDate::UtilsDate() :
_nTimeGap(0),
_bInitTimeServer(false)
{
    
}

UtilsDate::~UtilsDate()
{
    
}

bool UtilsDate::init()
{
    return true;
}

void UtilsDate::setTimeServer(time_t nTime)
{
    if ( nTime <= 0 )
    {
        _nTimeGap = 0;
        _bInitTimeServer = false;
    }
    else
    {
        time_t nTimeNow = time(nullptr);;
        
        _nTimeGap = nTime - nTimeNow;
        _bInitTimeServer = true;
    }
}

bool UtilsDate::isTimeServer()
{
    return _bInitTimeServer;
}

time_t UtilsDate::getTime()
{
    time_t nTimeNow = time(nullptr);
    nTimeNow = nTimeNow + _nTimeGap;
    
    return nTimeNow;
}

std::string UtilsDate::getTimeString(int64_t time, UtilsDate::eType type, std::string strFormat)
{
    std::string ret = "";
    int32_t nLength = 1024 * 100;
    
    if( type == UtilsDate::date_real_format && strFormat.length() != 0 )
    {
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, strFormat.c_str(), info);
        ret = buf;
        free(buf);
    }
    else if ( type == UtilsDate::date_real_YmdHMS )
    {// 2014.06.05 20:05:25
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%Y.%m.%d %H:%M:%S", info);
        ret = buf;
        free(buf);
    }
    else if ( type == UtilsDate::date_real_Ymd )
    {// 2014.06.05
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%Y.%m.%d", info);
        ret = buf;
        free(buf);
    }
    else if ( type == UtilsDate::date_real_md )
    {// 06.05
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%m.%d", info);
        ret = buf;
        free(buf);
    }
    else if( type == UtilsDate::date_real_mdHMS )
    {// 06.05 20:05:25
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%m.%d %H:%M:%S", info);
        ret = buf;
        free(buf);
    }
    else if( type == UtilsDate::date_real_HMS )
    {// 20:05:25
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%H:%M:%S", info);
        ret = buf;
        free(buf);
    }
    else if( type == UtilsDate::date_real_HM )
    {// 20:05
        time_t t = time;
        struct tm *info = localtime(&t);
        char* buf = (char*)malloc(nLength);
        
        strftime (buf, nLength, "%H:%M", info);
        ret = buf;
        free(buf);
    }
    else if(type == UtilsDate::date_user_type_1)
    {//00:00:00
        int32_t h = (int32_t)(time / 3600);
        int32_t m = time % 3600 / 60;
        int32_t s = time % 3600 % 60;
        
        ret = StringUtils::format("%02d:%02d:%02d", h, m, s);
    }
    else if(type == UtilsDate::date_user_type_2)
    {//00:00
        int32_t m = (int32_t)(time / 60);
        int32_t s = time % 60;
        
        ret = StringUtils::format("%02d:%02d", m, s);
    }
    
    
    
    return ret;
}
