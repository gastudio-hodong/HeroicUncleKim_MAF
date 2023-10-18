//
//  Popup7thInformation.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#ifndef Popup7thInformation_h
#define Popup7thInformation_h

#include "GameUIPopup/Base/PopupBase.h"

class Popup7thInformation : public PopupBase
{
public:
    static Popup7thInformation* create();
    
    Popup7thInformation(void);
    virtual ~Popup7thInformation(void);
    virtual bool init() override;
    
public:
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiContent();
    
    // ui draw
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    
private:
     
    
    // widget
    cocos2d::ui::Layout* _uiContainerContents;
 
};
#endif /* Popup7thInformation_h */
