//
//  PopupRevive.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/06/2019.
//

#ifndef PopupRevive_hpp
#define PopupRevive_hpp


#include "GameUIPopup/Base/PopupBase.h"

class PopupRevive : public PopupBase
{
public:
    static PopupRevive* create();
    
    PopupRevive(void);
    virtual ~PopupRevive(void);
    virtual bool init() override;
        
    virtual void show() override;

public:
    // set
    void setCallbackResult(const std::function<void(int, bool, bool, int)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContentNormal();
    void uiContentDevil();
    
    void uiButtonNormal();
    void uiButtonDevil();
    
    void uiCube();
    
    void drawContentNormal();
    void drawKeyBooster();
    
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickBoosterPopup(Ref* sender);
    void onClickPrestigePopup(Ref* sender);
    
    // callback
    void callbackBoosterResult();
    void callbackReviveDetailResult(int nType, int nTypeButton);
    void callbackReviveDetailDevilResult(int nType, int nTypeButton);
 
public:
    // game
    void onPrestigePopup(int nType);
    
private:
    int _nFloor;
    int _nFloorSub;
    
    std::function<void(int, bool, bool, int)> _onCallbackResult;
    
    // widget
    cocos2d::Layer* _layerContainerParent;
    
    // widget other
    cocos2d::Sprite* _sprBoosterIcon;
    cocos2d::Label* _labelBoosterEffect;
    cocos2d::Label* _labelMsg;
    cocos2d::Label* _labelBooster;

};
#endif /* PopupRevive_hpp */
