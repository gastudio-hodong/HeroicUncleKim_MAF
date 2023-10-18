//
//  PopupDungeonPassBadgeInfo.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/22.
//

#ifndef PopupDungeonPassBadgeInfo_h
#define PopupDungeonPassBadgeInfo_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupDungeonPassBadgeInfo : public PopupBase
{
public:
    static PopupDungeonPassBadgeInfo* create();
    
    PopupDungeonPassBadgeInfo(void);
    virtual ~PopupDungeonPassBadgeInfo(void);
    virtual bool init() override;
     
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContents();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickDungeonPass(cocos2d::Ref* sender);
    
private:
    
    Layer* _layerContainerContents;
};

#endif /* PopupDungeonPassBadgeInfo_hpp */
