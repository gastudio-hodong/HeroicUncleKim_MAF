//
//  PopupBeelzebub.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef PopupBeelzebub_h
#define PopupBeelzebub_h

#include "GameUIPopup/Base/PopupBase.h"

class PopupBeelzebub : public PopupBase
{
public:
    static PopupBeelzebub* create();
    
    PopupBeelzebub(void);
    virtual ~PopupBeelzebub(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiAuthority();
    
    // ui draw
    void drawAuthority();
    
    // set, get
    
    // callback
    void callbackInfo(bool bResult);
    
    // schedule
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickPointSend(cocos2d::Ref* sender);
    void onClickPointArrow(cocos2d::Ref* sender);
    
    // network
    void requestBlessing();
    void responseBlessing(cocos2d::network::HttpResponse* response,std::string &data);
    
    
private:
    bool _bInitUI;
    
    //
    TInteger _nBlessing;
    TInteger _hellPointNow;
    TInteger _hellPointMax;
    
    // widget
    cocos2d::ui::ImageView* _spriteContainerBottom;
    
    cocos2d::ui::Layout* _uiContainerList;
    cocos2d::ui::Layout* _uiContainerAuthority;
};

#endif /* PopupBeelzebub_h */
