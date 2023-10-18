//
//  InfoEventNewAttendReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#ifndef InfoEventNewAttendReward_hpp
#define InfoEventNewAttendReward_hpp

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoEventNewAttendReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEventNewAttendReward);
    InfoEventNewAttendReward();
    virtual ~InfoEventNewAttendReward();
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

#endif /* InfoEventNewAttendReward_hpp */
