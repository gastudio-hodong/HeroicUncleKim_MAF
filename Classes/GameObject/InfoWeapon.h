//
//  InfoWeapon.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 8. 3..
//

#ifndef InfoWeapon_h
#define InfoWeapon_h

#include "Common/ConfigDefault.h"

class InfoItems;
class InfoWeapon : public cocos2d::Ref
{
public:
    CREATE_FUNC(InfoWeapon);
    
    InfoWeapon();
    virtual ~InfoWeapon();
    virtual bool init();
    
public:
    int getIdx();
    void setIdx(int idx);
    
    std::string getName();
    void setNameKey(std::string key);
    
    std::string getComment();
    void setCommentKey(std::string key);
    
    int getLegend();
    void setLegend(int legend);
    
    cocos2d::Vector<InfoItems*> getCost();
    void setCost(std::string value);
    
    int getEnhanceMax();
    void setEnhanceMax(int enhanceMax);
    
    cocos2d::Vector<InfoItems*> getEnhanceCost();
    void setEnhanceCost(std::string value);
    
private:
    int _idx;
    std::string _nameKey;
    std::string _descKey;
    
    int _legend;
    
    // buy cost
    cocos2d::Vector<InfoItems*> _listCost;
    
    //
    int _enhanceMax;
    cocos2d::Vector<InfoItems*> _listEnhanceCost;

    
};

#endif /* InfoWeapon_h */
