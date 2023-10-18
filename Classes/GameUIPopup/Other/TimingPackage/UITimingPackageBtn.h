//
//  UITimingPackageBtn.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/06/29.
//

#ifndef UITimingPackageBtn_hpp
#define UITimingPackageBtn_hpp

#include "Common/ConfigDefault.h"

#include "Common/Observer/ITimingPackage.h"

#include "GameObject/InfoTimingPackage.h"

#include "GameUIPopup/Other/TimingPackage/PopupTimingPackage.h"
#include "GameUI/UI/UIReddot.h"

class UITimingPackageBtn : public cocos2d::Node, public ITimingPackage
{
public:
    static UITimingPackageBtn* create();
    
    UITimingPackageBtn();
    virtual ~UITimingPackageBtn();
    
    virtual bool init() override;
    virtual void update(float dt) override;
    
    virtual void onNotifyTimingPackge(cocos2d::ValueMap listData) override;
    
    void setInfoTimingPackage(InfoTimingPackage* infoTimingPackage);
    void deActiveUITimingPackageBtn();
    
    void onClick(cocos2d::Ref* pSender);
    void showPopup();
    
    void setConditionText();
    void callbackActiveTimingPackage(bool bResult, int nResult);
    void callbackReward(bool bResult, int nResult);
    void setCallbackReload(const std::function<void()>& pCallback);
    void setCallbackShowPopup(const std::function<void(InfoTimingPackage*)>& pCallback);
    
    InfoTimingPackage* getTimingPackageInfo();
    const bool isPurchase() const;
    void setVisibleTimingPackageBtn(bool isVisible);
private:
    bool _isVisible;
    bool _isActive;
    
    InfoTimingPackage* _infoTimingPackage;
    
    std::function<void()> _callbackReload;
    std::function<void(InfoTimingPackage*)> _callbackShowPopup;
    
    //
    MafNode::MafMenuItemSprite* _itemIcon;
    cocos2d::Label* _uiCondition;
    cocos2d::ui::ImageView* _uiConditionType;
    UIReddot* _reddot;
    
};

#endif /* UITimingPackageBtn_hpp */
