//
//  PopupAdventureMission.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/07.
//

#ifndef PopupAdventureMission_hpp
#define PopupAdventureMission_hpp

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupAdventureMission : public PopupBase
{
public:
    static PopupAdventureMission* create();
    
    PopupAdventureMission(void);
    virtual ~PopupAdventureMission(void);
    virtual bool init();
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiTop();
    void uiDailyMission();
    void uiWeeklyMission();
    void uiBottom();
    
    void refreshUI();
    
    // click
    void onClickClose(cocos2d::Ref* const sender);
    void onClickAllComplete(cocos2d::Ref* const sender);
    
    void callbackMissionAllComplete(bool bResult, int nResult);
private:
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyDailyMission;
    cocos2d::Layer* _lyWeeklyMission;
    cocos2d::Layer* _lyBottomUI;
};

#endif /* PopupAdventureMission_hpp */
