//
//  PopupCalArtifactReinforce.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupCalArtifactReinforce_h
#define PopupCalArtifactReinforce_h

#include "GameUIPopup/Base/PopupBase.h"

class InfoArtifact;
class PopupCalArtifactReinforce : public PopupBase
{
private:
    enum eTag {
        tag_container = 10,
        
        tag_btn1_1 = 20,
        tag_btn1_10 = 21,
        tag_btn1_100 = 22,
        tag_btn1_1000 = 23,
        tag_btn1_max = 24,
        
        tag_btn1_m1 = 30,
        tag_btn1_m10 = 31,
        tag_btn1_m100 = 32,
        tag_btn1_m1000 = 33,
        
        tag_btn2_cube = 101,
    };
    
public:
    static PopupCalArtifactReinforce* create(InfoArtifact *info);
    
    PopupCalArtifactReinforce(void);
    virtual ~PopupCalArtifactReinforce(void);
    virtual bool init(InfoArtifact *info);
    virtual void onEnter();
    
public:
    // set
    void setCallback(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiCurrency();
    void uiTop();
    void uiBottom();
    
    // set, get
    void drawArtifactInfo();
    void drawArtifactLevel();
    
    
    // schedule
    void buyArtifact(float dt);
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReset(cocos2d::Ref* sender);
    void onClickLevel(cocos2d::Ref* sender);
    void onClickConfirm(cocos2d::Ref* sender);
    void onClickReinforceReset(cocos2d::Ref* sender);
    void onClickReinforceResetOk(cocos2d::Ref* sender);
    void onClickCurrency(cocos2d::Ref* sender);
    
private:
    InfoArtifact* _info;
    
    bool _valueLongShow;
    int _reinforceNow;
    int _reinforceAfter;
    
    
    std::function<void(void)> _onCallback;
    
    
    // widget
    cocos2d::Layer* _layerContainerCurrency;
    
    cocos2d::Label* _labelReinforce;
    cocos2d::Label* _labelLevel;
    cocos2d::Label* _labelEffect;
    cocos2d::Label* _labelLevelPrev;
    cocos2d::Label* _labelLevelAfter;
    cocos2d::Label* _labelLevelDiff;
    
    cocos2d::Label* _labelNeedCube;
    
    MafNode::MafMenuItemSprite* _itemCube;
    MafNode::MafMenuItemSprite* _itemMagnifier;
};

#endif /* PopupCalArtifactReinforce_h */
