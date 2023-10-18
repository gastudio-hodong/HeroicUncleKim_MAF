//
//  PopupCash.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupCash_h
#define PopupCash_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupCash : public PopupBase
{
public:
    enum class E_TAB {
        NONE = 0,
        
        NEWS,       // 뉴스
        BONUS,      // 보너스
        SPECIAL,    // 스페셜
        MONTHLY,    // 정액제
        NORMAL,     // 일반
        HOTDEAL,    // 핫딜
        PET_EQUIP,  // 펫장비
        SUMMON,     // 정령
        COSTUME,    // 코스튬
        RELIC,      // 유물
    };
    
public:
    static PopupCash* create();
    
    PopupCash(void);
    virtual ~PopupCash(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
public:
    void setTab(E_TAB tab);
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiContents();
    void uiButtons();
    void uiProgress();
    
    //
    void drawContents();
    void drawButtons();
    
    // get, set
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    
    // callback
    void callbackTable(bool bResult);
    void callbackTableProgress(int nTotal, int nSuccess, int nProgress);
    
    void callbackInfo(bool bResult);
    void callbackDailyInfo(bool bResult);
    void callbackRemainingInfo(bool bResult);
    
private:
    bool _bInitUI;
    
    //
    E_TAB _eTab;
    
    // table download
    int _nTableProgress;
    int _nTableDownloadTotal;
    int _nTableDownloadSuccess;

    // widget
    cocos2d::ui::Layout* _uiContainerContents;
    cocos2d::ui::Layout* _uiContainerButtons;
    
    cocos2d::ui::Layout* _uiContainerProgress;
    
    // widget content
    cocos2d::ui::Layout* _uiContentNew;
    cocos2d::ui::Layout* _uiContentBonus;
    cocos2d::ui::Layout* _uiContentSpecial;
    cocos2d::ui::Layout* _uiContentMonthly;
    cocos2d::ui::Layout* _uiContentNormal;
    cocos2d::ui::Layout* _uiContentHotDeal;
};

#endif /* PopupCash_h */
