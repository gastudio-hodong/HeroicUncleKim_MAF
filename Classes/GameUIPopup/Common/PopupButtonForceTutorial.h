//
//  PopupButtonForceTutorial.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/25.
//

#ifndef PopupButtonForceTutorial_hpp
#define PopupButtonForceTutorial_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupButtonForceTutorial : public PopupBase
{
public:
    
    static PopupButtonForceTutorial* create();
    
    PopupButtonForceTutorial();
    virtual ~PopupButtonForceTutorial(void);
    virtual bool init() override;
    
    //
    void setCallbackListener(const std::function<void(cocos2d::Ref*)>& callback);
    void setButton(cocos2d::ui::Button* const btnButton);
    void setButton(MafNode::MafMenuItemSprite*  const itemButton);
    
    virtual void show() override;
protected:
    // init
    void initVar();
    
    void uiButtonEffect(cocos2d::Node* const node);
    
    //click
    void onClick(cocos2d::Ref* const sender);
private:
    cocos2d::ui::Button* _btnButton;
    MafNode::MafMenuItemSprite* _itemButton;
    
    cocos2d::Node* _pOriginParent;
    cocos2d::Vec2 _posOrigin;
    
    std::function<void(cocos2d::Ref*)> _callbackListener;
};

#endif /* PopupButtonForceTutorial_hpp */
