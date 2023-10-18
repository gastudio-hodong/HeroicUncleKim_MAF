//
//  PopupBafometz.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupBafometz_h
#define PopupBafometz_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBafometz : public PopupBase
{
public:
    static PopupBafometz* create();
    
    PopupBafometz(void);
    virtual ~PopupBafometz(void);
    virtual bool init();
    virtual void onEnter();
    virtual void update(float dt);
    
public:
    // set
    void setCallbackSkip(const std::function<void(int)>& callback);
    void setCallbackAuto(const std::function<void(bool)>& callback, bool isAuto);
    void setCallbackReward(const std::function<void(void)>& callback);
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiInfo();
    void uiContent();
    
    // ui draw
    void drawInfo();
    void drawContent();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickCountUpDown(cocos2d::Ref* sender);
    void onClickSkip(cocos2d::Ref* sender);
    void onClickGetReward(cocos2d::Ref* sender);
    
    // network
    void requestReward();
    void responseReward(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    int _nBafometzCount;
    int _nSkip;
    
    bool _isAuto;
    float _fUpgradeTime;
    float _fUpgradeTimeMax;
    
    std::function<void(int)> _onCallbackSkip;
    std::function<void(bool)> _onCallbackAuto;
    std::function<void(void)> _onCallbackReward;
    
    
    // widget
    cocos2d::ui::Layout* _uiContentsUIInfo;
    cocos2d::ui::Layout* _uiContentsUIContent;
    
    cocos2d::ui::ImageView* _uiInfoBafometzIcon;
    cocos2d::ui::CText* _uiContentCount;
    cocos2d::ui::Button* _uiContentArrowL;
    cocos2d::ui::Button* _uiContentArrowR;
    cocos2d::ui::LoadingBar* _uiContentProgress;
    cocos2d::ui::CText* _uiContentProgressCount;
    cocos2d::ui::CText* _uiContentGet;
    cocos2d::ui::Button* _uiContentReward;
    
};

#endif /* PopupBafometz_h */
