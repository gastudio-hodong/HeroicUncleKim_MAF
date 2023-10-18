//
//  InfoRaidBongStat.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/11/01.
//

#ifndef InfoRaidBongStat_hpp
#define InfoRaidBongStat_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoRaidBongStat : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidBongStat);
    
    InfoRaidBongStat();
    virtual ~InfoRaidBongStat();
    virtual bool init();
    
    
public:
    int getIdx();
    void setIdx(int value);
    int getCurrentLv();
    void setCurrentLv(int value);
    int getMaxLv();
    void setMaxLv(int value);
    double getEffect();
    
    std::string getIconPath();
    void setIconPath(std::string value);
    
    std::string getText();
    void setTextKey(std::string value);
    
private:
    int _nIdx;
    TInteger _nCurrentLv;
    int _nMaxLv;
    
    
    std::string _strPath;
    std::string _strTextKey;
    
};

#endif /* InfoRaidBongStat_hpp */
