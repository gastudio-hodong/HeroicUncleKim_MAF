//
//  InfoEvent7th.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//

#ifndef InfoEvent7th_hpp
#define InfoEvent7th_hpp

#include "Common/ConfigDefault.h"

class InfoEvent7th : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEvent7th);
    
    InfoEvent7th();
    virtual ~InfoEvent7th();
    virtual bool init();
    
public:
    int getType();
    void setType(int type);
    
    int getLevel();
    void setLevel(int level);
    
    int getPointCountMin();
    void setPointCountMin(int count);
    
    int getPointCountMax();
    void setPointCountMax(int count);
    
    int getPointCountPersonalLimit();
    void setPointCountPersonalLimit(int count);
    
    int getConditionPersonLevel();
    void setConditionPersonLevel(int level);
    
    std::string getName();
    void setNameKey(std::string key);
    
    std::string getDesc();
    void setDescKey(std::string key);
   
    std::string getReward();
    void setReward(std::string reward);
    
private:
    TInteger _nType;
    TInteger _nLevel;
    TInteger _nPointCountMin;
    TInteger _nPointCountMax;
    TInteger _nPointCountPersonalLimit;
    
    TInteger _nConditionPersonLevel;
    
    std::string _strNameKey;
    std::string _strDiscKey;
    std::string _strReward;
};


#endif /* InfoEvent7th_hpp */
