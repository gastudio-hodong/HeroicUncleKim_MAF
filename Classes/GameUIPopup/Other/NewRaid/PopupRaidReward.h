//
//  PopupRaidReward.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/12/13.
//

#ifndef PopupRaidReward_hpp
#define PopupRaidReward_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoBadge.h"
#include "GameObject/InfoItems.h"

class PopupRaidReward : public PopupBase
{
public:
    static PopupRaidReward* create(cocos2d::Vector<InfoItems*> vecReward);
    
    PopupRaidReward(void);
    virtual ~PopupRaidReward(void);
    virtual bool init(cocos2d::Vector<InfoItems*> vecReward);
    
    void initVar();
    void initUI();
    void initTopUI();
    void initBottomUI();
    
    void onClick(Ref* sender);
    
private:
    cocos2d::Vector<InfoItems*> _vecReward;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyBottomUI;
};

#endif /* PopupRaidReward_hpp */
