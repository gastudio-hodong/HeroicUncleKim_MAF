//
//  UtilsDate.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 10/10/2019.
//

#ifndef UtilsDate_h
#define UtilsDate_h

#include "cocos2d.h"

class UtilsDate
{
public:
    enum eType {
        date_real_format,
        date_real_YmdHMS = 0,
        date_real_Ymd,
        date_real_md,
        date_real_mdHMS,
        date_real_HMS,
        date_real_HM,
        
        
        date_user_type_1, //00:00:00
        date_user_type_2, //00:00
        
        
    };

public:
    static UtilsDate* getInstance();
    
    UtilsDate(void);
    virtual ~UtilsDate(void);
    virtual bool init();
    
public:
    void setTimeServer(time_t nTime);
    bool isTimeServer();
    
    time_t getTime();
    std::string getTimeString(int64_t time, UtilsDate::eType type, std::string strFormat = "");
    
private:
    time_t _nTimeGap;
    bool _bInitTimeServer;
    
    
};

#endif /* UtilsDate_h */
