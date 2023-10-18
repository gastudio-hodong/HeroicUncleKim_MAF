//
//  InfoEventReturnAttendReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/06/20.
//

#ifndef InfoEventReturnAttendReward_hpp
#define InfoEventReturnAttendReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventReturnAttendReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventReturnAttendReward);
    InfoEventReturnAttendReward();
    virtual ~InfoEventReturnAttendReward();
    virtual bool init();
    
public:
    const int getDay() const;
    void setDay(const int value);
    
    const bool isReward() const;
    void setIsReward(const bool value);
    
    const cocos2d::Vector<InfoItems*> getListReward() const;
    void setListReward(cocos2d::Vector<InfoItems*> list);
protected:
    int _nDay;
    bool _isReward;
    cocos2d::Vector<InfoItems*> _listReward;
};

#endif /* InfoEventReturnAttendReward_hpp */
