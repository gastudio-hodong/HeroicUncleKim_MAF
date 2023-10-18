//
//  PopupFlashman.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef PopupFlashman_h
#define PopupFlashman_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupFlashman : public PopupBase, public IRefresh
{
public:
    static PopupFlashman* create();
    
    PopupFlashman(void);
    virtual ~PopupFlashman(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
public:
    void onBattle();
    
protected:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContainer();
    void uiDefault();
    void uiResult();
    void uiScoreAccReward();
    void uiButtons();
    
    // ui : draw
    void drawDefault();
    void drawResult();
    void drawScoreAccReward();
    void drawButtons();
    
    // set, get
    
    // callback
    void callbackInfo(bool bResult, int nResult);
    void callbackResult(bool bResult, int nResult, int score, bool bBest, std::string reward);
    void callbackRankReward(int rank, std::string reward);
    void callbackAccReward(bool bResult, int nResult);
    
    // schedule
    void scheduleBattle(float dt);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickHelp(cocos2d::Ref* sender);
    void onClickMission(cocos2d::Ref* sender);
    void onClickRank(cocos2d::Ref* sender);
    void onClickScoreAcc(cocos2d::Ref* sender);
    void onClickScoreAccReward(cocos2d::Ref* sender);
    void onClickDexLoots(cocos2d::Ref* sender);
    void onClickEnter(cocos2d::Ref* sender);
    void onClickEnterSkip(cocos2d::Ref* sender);
    void onTouchAttack(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onClickAttack(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    bool _bAttackBtnPressed;
    double _attackBtnAnimDelay;
    std::vector<int64_t> _listAttackBtnPressedTime;
    std::vector<cocos2d::Vec2> _listAttackBtnPressedPos;
    
    // result
    int _resultScoreBest;
    int _resultScoreNow;
    bool _resultScoreNew;
    std::string _resultReward;
    
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsBattle;
    cocos2d::ui::Layout* _uiContentsUIDefault;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
    cocos2d::ui::Layout* _uiContentsUIResult;
    cocos2d::ui::Layout* _uiContentsUIScoreAccReward;
    
    cocos2d::ui::ImageView* _uiFeverBG;
 
};
#endif /* PopupFlashman_h */
