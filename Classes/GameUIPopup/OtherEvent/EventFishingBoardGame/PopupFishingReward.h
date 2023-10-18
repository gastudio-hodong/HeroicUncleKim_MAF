//
//  PopupFishingReward.hpp
//  FantasyClicker-mobile
//
//  Created by TG on 2020/07/13.
//

#ifndef PopupFishingReward_h
#define PopupFishingReward_h

#include "GameUIPopup/Base/PopupBase.h"

class CellFishingQuest;
class PopupFishingReward : public PopupBase
{

public:
    static PopupFishingReward* create(std::string strReward);
        
    PopupFishingReward(void);
    virtual ~PopupFishingReward(void);
    virtual bool init(std::string strReward);
    
public:
    
    
private:
    // init
    void initVar();
    void initUI();
    
    // draw
    
        
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
    std::string _strReward;
    
    
    // widget
    cocos2d::Label* _lbPoint;
    cocos2d::Label* _lbIconPoint;
    cocos2d::Sprite* _sprFish;
};
#endif /* PopupFishingReward_h */
