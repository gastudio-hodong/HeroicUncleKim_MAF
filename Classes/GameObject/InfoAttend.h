//
//  InfoAttend.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 14/02/2020.
//

#ifndef InfoAttend_h
#define InfoAttend_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoAttend : public cocos2d::Ref
{
    
public:
    CREATE_FUNC(InfoAttend);
    
    InfoAttend();
    virtual ~InfoAttend();
    virtual bool init();
    
public:
    int getType();
    void setType(int value);
    
    int getDay();
    void setDay(int value);
    
    InfoItems* getReward();
    void setReward(std::string value);
     
    bool isReceive();
    void setReceive(bool value);
    
private:
    int _type;
    int _day;
    std::string _reward;
    
    bool _receive;
    
};
#endif /* InfoAttend_h */
