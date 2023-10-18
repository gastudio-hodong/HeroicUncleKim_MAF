//
//  InfoRaidBoxrate.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoRaidBoxrate_hpp
#define InfoRaidBoxrate_hpp

#include "Common/ConfigDefault.h"

class InfoRaidBoxrate : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidBoxrate);
    struct InfoRate
    {
        InfoRate(int lv, float rate)
        {
            nLv = lv;
            fRate = rate;
        }
        int nLv = 0;
        float fRate = 0;
    };
    
    InfoRaidBoxrate();
    virtual ~InfoRaidBoxrate();
    virtual bool init();
    
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getCount();
    void setCount(int value);
    
    std::vector<InfoRaidBoxrate::InfoRate> getListRate();
    void setListRate(std::string value);
private:
    int _nIdx;
    float _nCount;
    
    std::vector<InfoRaidBoxrate::InfoRate> listRate;
};

#endif /* InfoRaidBoxrate_hpp */
