//
//  InfoProductGrowth.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoProductGrowth_h
#define InfoProductGrowth_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoProductGrowth : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoProductGrowth);
    
    InfoProductGrowth();
    virtual ~InfoProductGrowth();
    virtual bool init();
    
public:
    std::string getType();
    void setType(std::string value);
    
    int getLevel();
    void setLevel(int value);
    
    int getIdx();
    void setIdx(int value);
    
    int getIdxCash();
    void setIdxCash(int value);
    
    int getCondition();
    void setCondition(int value);
    
    bool isRewardFree();
    void setRewardFree(bool bReward);
    
    bool isRewardCash();
    void setRewardCash(bool bReward);
    
    cocos2d::Vector<InfoItems*> getItemsFree();
    void setItemsFree(std::string strItems);
    
    cocos2d::Vector<InfoItems*> getItemsCash();
    void setItemsCash(std::string strItems);
    
private:
    std::string _type;
    
    int _level;
    int _idx;
    int _idxCash;
    
    int _condition;
    
    bool _bRewardNormal;
    bool _bRewardPass;

    // 보상 리스트
    cocos2d::Vector<InfoItems*> _listItemsFree;
    cocos2d::Vector<InfoItems*> _listItemsCash;
    
};

#endif /* InfoProductGrowth_h */
