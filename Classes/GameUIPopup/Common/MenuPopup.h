//
//  MenuPopup.h
//  FantasyClicker
//
//  Created by SanHeo on 2017. 8. 8..
//
//

#ifndef MenuPopup_h
#define MenuPopup_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

#include "GameUI/UI/UIReddot.h"

class MenuPopup : public PopupBase, public IRefresh
{
public:
    enum eTag {
        tag_menu_save = 0,
        tag_menu_facebook,
        tag_menu_naver,
        tag_menu_discord,
        tag_menu_twitter,
        tag_menu_mail,
        tag_menu_gift,
        tag_menu_attend,
        tag_menu_stat,
        tag_menu_rank,
        tag_menu_achive,
        tag_menu_story,
        tag_menu_rate,
        tag_menu_rule_terms,
        tag_menu_rule_privacy,
        tag_menu_tuto,
        tag_menu_setting,
        tag_menu_bonsikdex,
        tag_menu_speech,
        tag_menu_battery,
        tag_menu_push,
        tag_menu_load,
        tag_menu_language,
        tag_menu_login,
        tag_menu_supporters,
        tag_menu_transfer,
        tag_menu_jpwiki,
        tag_menu_faq,
        tag_menu_friend,
        tag_menu_lobi,
        tag_menu_share,
        tag_menu_empty,         // 공백
        tag_menu_mini,          // 미니메뉴
        tag_menu_withdrawal,          // 탈퇴
    };
    
public:
    CREATE_FUNC(MenuPopup);
    
    MenuPopup(void);
    virtual  ~MenuPopup(void);
    virtual bool init() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
protected:
    // init
    void initVar();
    void initVarNormal();
    void initVarDevil();
    void initUi();
    
    // ui
    void uiMenuListTop();
    void uiMenuList();
    void uiGuest();
    void uiGames();
    
    //
    void showMailPopup(void);
    void showStatPopup(void);
    void showAttendPopup(void);
    void showSupportersPopup(void);
    
    // click
    void onClickClose(Ref* sender);
    void onClickOption(Ref* sender);
    void onClickSaveOk(Ref* sender);
    void onClickSaveCancel(Ref* sender);
    void onClickLoadOk(Ref* sender);
    void onClickLoadCancel(Ref* sender);
    
    // callback
    
    
    // network
    
private:
    struct element
    {
        int     tag;
        std::string strImage;
        std::string strNameKey;
    };
    
    std::vector<element> _listElementTop;
    std::vector<element> _listElementMini;
    std::vector<element> _listElement;
    
    // 임시 기능
    int _downloadSize;
    int _downloadSuccess;
    int _downloadError;
    std::string _downloadErrorMsg;
    
    //more Game
    std::vector<std::string> _vecMoreGameLink;
    
    cocos2d::Layer* _layerContainerMenuTop;
    cocos2d::Layer* _layerContainerMenu;
    cocos2d::Layer* _layerContainerGuest;
    cocos2d::Layer* _layerContainerGames;
    
public:
    // widget
    UIReddot* _uiReddotAttent;
    UIReddot* _uiReddotGift;
    UIReddot* _uiReddotBonsikdex;
    
};






#endif /* MenuPopup_h */
