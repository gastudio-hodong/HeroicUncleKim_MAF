//
//  InfoEvent7thPerson.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/01/11.
//
 
#ifndef InfoEvent7thPerson_hpp
#define InfoEvent7thPerson_hpp

#include "Common/ConfigDefault.h"

class InfoEvent7thPerson : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoEvent7thPerson);
    
    InfoEvent7thPerson();
    virtual ~InfoEvent7thPerson();
    virtual bool init();
    
public:
    int getLevel();
    void setLevel(int level);
    
    int getPointCountMin();
    void setPointCountMin(int count);
    
    int getPointCountMax();
    void setPointCountMax(int count);
    
    std::string getReward();
    void setReward(std::string reward);
    
private:
    int _nIdx;
    
    TInteger _nLevel;
    TInteger _nPointCountMin;
    TInteger _nPointCountMax;
    
    std::string _strReward;
};


#endif /* InfoEvent7thPerson_hpp */
