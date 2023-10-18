//
//  PopupMissionDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/26.
//

#ifndef PopupMissionDefault_hpp
#define PopupMissionDefault_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupMissionDefault : public PopupBase
{
public:
    static PopupMissionDefault* create(E_MISSION eType);
    
    PopupMissionDefault(void);
    virtual ~PopupMissionDefault();
    bool init(E_MISSION eType);
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
protected:
    
    void initVar();
    void initUI();
    
    void uiReload();
    void uiLoadLayer();
    
    void uiTopBanner();
    void uiTopBannerSpecialMission();
    void uiTopBannerDailyMission();
    void uiTab();
    void uiTop();
    void uiTopPass();
    void uiServerMission();
    void uiSpecialMission();
    // click
    void onClickTab(Ref* sender);
    void onClickClose(Ref* sender);
    
    void onClickPass(Ref* sender);
    void onClickAchievement(Ref* sender);
    void onClickPassBadge(Ref* sender);
    
    void onClickSpecialMissionShortCut(Ref* sender);
    void onClickSpecialMissionComplete(Ref* sender);
    void showRedDotAction(E_MISSION type);
    
    void callbackSetTab(E_MISSION type);
    void callbackHide();
    void callbackSpecialMissionComplete(bool bResult, int nResult);
private:
    E_MISSION _eType;
    Layer* _layerContainerTopBanner;
    Layer* _layerContainerTab;
    Layer* _layerContainerTop;
    
    Layer* _layerContentsArea;
    Layer* _layerContainerDaily;
    Layer* _layerContainerWeekly;
    Layer* _layerContainerSpecial;
    
    Layer* _lyNowContents;
    
};

#endif /* PopupMissionDefault_hpp */
