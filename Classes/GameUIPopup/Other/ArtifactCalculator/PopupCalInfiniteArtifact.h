//
//  PopupCalInfiniteArtifact.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/11/03.
//

#ifndef PopupCalInfiniteArtifact_h
#define PopupCalInfiniteArtifact_h

#include "GameUIPopup/Base/PopupBase.h"

class InfiniteArtifactInfo;
class PopupCalInfiniteArtifact : public PopupBase
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
        
        tag_btn2_key = 101,
        tag_btn2_point = 102,
    };
    
public:
    static PopupCalInfiniteArtifact* create(InfiniteArtifactInfo *info);
    
    PopupCalInfiniteArtifact(void);
    virtual ~PopupCalInfiniteArtifact(void);
    virtual bool init(InfiniteArtifactInfo *info);
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
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickReset(cocos2d::Ref* sender);
    void onClickLevel(cocos2d::Ref* sender);
    void onClickConfirm(cocos2d::Ref* sender);
    void onClickCurrency(cocos2d::Ref* sender);
    
    
private:
    InfiniteArtifactInfo* _info;
    
    bool _valueLongShow;
    int _levelNow;
    int _levelAfter;
    
    std::function<void(void)> _onCallback;
    // widget
    cocos2d::Layer* _layerContainerCurrency;
    
    cocos2d::Label* _labelLevel;
    cocos2d::Label* _labelEffect;
    cocos2d::Label* _labelLevelPrev;
    cocos2d::Label* _labelLevelAfter;
    cocos2d::Label* _labelLevelDiff;
    
    cocos2d::Label* _labelNeedKey;
    cocos2d::Label* _labelNeedPoint;
    
    MafNode::MafMenuItemSprite* _itemKey;
    MafNode::MafMenuItemSprite* _itemPoint;
    MafNode::MafMenuItemSprite* _itemMagnifier;
};

#endif /* PopupCalInfiniteArtifact_h */
