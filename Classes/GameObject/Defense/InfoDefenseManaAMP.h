//
//  InfoDefenseManaAMP.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/21.
//

#ifndef InfoDefenseManaAMP_hpp
#define InfoDefenseManaAMP_hpp

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

class InfoDefenseManaAmpStone;
class InfoDefenseManaAMP : public cocos2d::Ref
{
public:
    static InfoDefenseManaAMP* create();
    
    InfoDefenseManaAMP();
    virtual ~InfoDefenseManaAMP();
    virtual bool init();
    
public:
    int getIndex() const;
    void setIndex(const int value);
    
    int getCurrentSuccessRate() const;
    void setCurrentSuccessRate(const int value);
    
    int getStoneIndexByOrder(const int pos);
    std::vector<int> getStoneList();
    void setStoneList(std::vector<int> list);
    int getStoneCount();
    bool isComplete();
    
    int getRedStoneCount() const;
    
    int getEquipCost();
    void setEquipCOst(const int value);
    int getOpenFloor();
    void setOpenFloor(const int value);
    
    double getManaAMPPower();
    
    bool isReddotCondition();
    void completeReddotCondition();
private:
    int _index;
    int _currentSuccessRate;
    TInteger _cost;
    TInteger _openFloor;
    std::vector<int> _listStone;
};


#endif /* InfoDefenseManaAMP_hpp */
