//
//  PopupEnterCount.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 11/10/2019.
//

#ifndef PopupEnterCount_h
#define PopupEnterCount_h

#include "GameUIPopup/Base/PopupBase.h"

#include "ui/CocosGUI.h"

class PopupEnterCount : public PopupBase
{
public:
    CREATE_FUNC(PopupEnterCount);
    
    PopupEnterCount();
    virtual ~PopupEnterCount(void);
    virtual bool init();
    virtual void onEnter();
    virtual void update(float dt);
    
public:
    //set
    void setCallbackEnter(const std::function<void(int)>& callback);
    void setCallbackEnterBafometz(const std::function<void(int)>& callback);
    
protected:
    
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiButtons();
    
    void drawInfo();
    void drawButtons();
    
    //click
    void onClickClose(Ref* sender);
    void onClickCountUpDown(Ref* sender);
    void onClickEnter(Ref* sender);
    void onClickEnterBafometz(Ref* sender);
    
private:
    std::function<void(int)> _onCallbackEnter;
    std::function<void(int)> _onCallbackEnterBafometz;
    
    int _nHeartCount;
    int _nCount;
    
    float _fUpgradeTime;
    float _fUpgradeTimeMax;
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIButtons;
    
    cocos2d::ui::CText* _uiTextCount;
    
    cocos2d::ui::Button* _uiContentArrowL;
    cocos2d::ui::Button* _uiContentArrowR;
    
    cocos2d::ui::Button* _uiBtnEnter;
    cocos2d::ui::Button* _uiBtnEnterBafometz;
};

#endif /* PopupEnterCount_hpp */
