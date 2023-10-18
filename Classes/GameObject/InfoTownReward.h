//
//  InfoTownReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/07.
//

#ifndef InfoTownReward_h
#define InfoTownReward_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoTownReward : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoTownReward);
    
    InfoTownReward();
    virtual ~InfoTownReward();
    virtual bool init();
        
public:
    int getType();
    void setType(int value);
    
    int getCondition();
    void setCondition(int value);
     
    InfoItems* getItems();
    void setItems(std::string value);
    
    bool isReward();
    void setReward(bool value);
    
private:

    int _type;
    int _condition;
    
    std::string _items;
    bool _bReward;
};

#endif /* InfoTownReward_hpp */
