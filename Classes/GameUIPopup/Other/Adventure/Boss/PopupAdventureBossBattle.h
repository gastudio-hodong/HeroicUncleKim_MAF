//
//  PopupAdventureBossBattle.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#ifndef PopupAdventureBossBattle_h
#define PopupAdventureBossBattle_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoAdventureBoss;
class PopupAdventureBossBattle : public PopupBase
{
public:
    CREATE_FUNC(PopupAdventureBossBattle);
    
    PopupAdventureBossBattle(void);
    virtual ~PopupAdventureBossBattle(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiReward();
    void uiButtons();
    
    // draw
    void drawDefault();
    void drawReward();
    void drawButtons();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickArrow(cocos2d::Ref* sender);
    void onClickBulletChange(cocos2d::Ref* sender);
    void onClickAttack(cocos2d::Ref* sender);
    
    // network
    
    // callback
    void callbackAttackFinish();
    void callbackNetworkAttack(bool bResult, int nResult);
    void callbackNetworkInfo(bool bResult, int nResult);
    
    
private:
    bool _bInitUI;
    
    //
    InfoAdventureBoss* _objInfoBoss;
    
    std::vector<int> _listBullet;
    int _bulletIdx;
    int _bulletItemCount;
    
    // widget
    cocos2d::ui::Layout* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsDefault;
    cocos2d::ui::Layout* _uiContentsReward;
    cocos2d::ui::Layout* _uiContentsButtons;
};

#endif /* PopupAdventureBossBattle_hpp */
