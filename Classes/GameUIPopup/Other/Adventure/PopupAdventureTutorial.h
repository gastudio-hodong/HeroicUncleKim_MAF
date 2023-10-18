//
//  PopupAdventureTutorial.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/05/25.
//

#ifndef PopupAdventureTutorial_hpp
#define PopupAdventureTutorial_hpp

#include "GameUIPopup/Base/PopupBase.h"

class PopupAdventureTutorial : public PopupBase
{
public:
    static PopupAdventureTutorial* create(const int nStep);
    
    PopupAdventureTutorial();
    virtual ~PopupAdventureTutorial(void);
    bool init(const int nStep);
    
    virtual void show() override;
    void setCallbackHide(const std::function<void(void)>& callback);
    
protected:
    
    // init
    void initVar();
    void initUI();
    
    // ui
    void uiTop();
    void uiMiddle();
    void uiBottom();
    
    //click
    void onClickClose(Ref* const sender);
    
    void addTutorialStep();
private:
    int _nTutorialNum;
    
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyMiddleUI;
    cocos2d::Layer* _lyBottomUI;
    
    std::function<void(void)> _callbackHide;
};

#endif /* PopupAdventureTutorial_hpp */
