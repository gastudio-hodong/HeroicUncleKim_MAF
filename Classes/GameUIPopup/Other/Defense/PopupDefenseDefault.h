//
//  PopupDefenseDefault.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/07/05.
//

#ifndef PopupDefenseDefault_hpp
#define PopupDefenseDefault_hpp
#include "GameUIPopup/Base/PopupBase.h"

class LayerDefenseMain;
class PopupDefenseDefault : public PopupBase
{
public:
    static PopupDefenseDefault* create();
    
    PopupDefenseDefault();
    virtual ~PopupDefenseDefault(void);
    virtual bool init() override;
    virtual void onEnter() override;
protected:
    void initVar();
    void initUI();
    
    void uiMain();
    void uiBottom();
    //event
    
    void onBackground();
    //Click
    void onClickClose(Ref* sender);
    void onClickButton(Ref* sender);
    //callback
    
    void onServerInfo(const bool bResult, const int nResult);
    void onServerUpdateFloor(const bool bResult, const int nResult);
private:
    cocos2d::Layer* _lyContainerParent;
    cocos2d::Layer* _lyMainGame;
    cocos2d::Layer* _lyBottom;
    
    LayerDefenseMain* _mainGame;
};


#endif /* PopupDefenseDefault_hpp */
