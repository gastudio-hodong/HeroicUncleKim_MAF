//
//  PopupTimeTravelReward.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupTimeTravelReward_h
#define PopupTimeTravelReward_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupTimeTravelReward : public PopupBase
{
public:
    static PopupTimeTravelReward* create();
    
    PopupTimeTravelReward(void);
    virtual ~PopupTimeTravelReward(void);
    virtual bool init();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiFloor();
    void uiRewardCurrency();
    void uiButton();
    
 

    // set, get
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
    
private:
    int _nRewardFloor;
    int _nRewardGem;
    int _nRewardPoint;
    std::string _strRewardKey;
    std::string _strRewardGold;
    
    std::function<void(void)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerFloor;
    cocos2d::Layer* _layerContainerRewardCurrency;
    cocos2d::Layer* _layerContainerButton;
};

#endif /* PopupTimeTravelReward_h */
