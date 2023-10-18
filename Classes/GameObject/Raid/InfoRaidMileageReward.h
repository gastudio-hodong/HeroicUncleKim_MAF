//
//  InfoRaidMileageReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/27.
//

#ifndef InfoRaidMileageReward_hpp
#define InfoRaidMileageReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoRaidMileageReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoRaidMileageReward);
    
    InfoRaidMileageReward();
    virtual ~InfoRaidMileageReward();
    virtual bool init();
    
    
public:
    int getIdx();
    void setIdx(int value);
    
    int getValue();
    void setValue(int value);
    
    cocos2d::Vector<InfoItems*> getListReward();
    void setListReward(cocos2d::Vector<InfoItems*> list);
    
private:
    int _nIdx;
    float _nValue;
    
    cocos2d::Vector<InfoItems*> _listReward;
};


#endif /* InfoRaidMileageReward_hpp */
