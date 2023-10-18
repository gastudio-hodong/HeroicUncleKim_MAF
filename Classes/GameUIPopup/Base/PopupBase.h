//
//  PopupBase.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 05/06/2019.
//

#ifndef PopupBase_h
#define PopupBase_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"

enum class PopupBaseType
{
    BASE,
    TRANSPARENT
};

class PopupBase : public cocos2d::LayerColor
{
public:
    PopupBase();
    virtual ~PopupBase();
    virtual bool init(PopupBaseType eType = PopupBaseType::BASE);
    virtual void onExit() override;
    
public:
    virtual void backkey();
    virtual void show();
    virtual void hide();
    
    // set, get
    void setHideBackKey(bool bHide);
    bool isHideBackKey();
    void setHideAll(bool bHide);
    bool isHideAll();
    void setNextPopup(PopupBase* popup);
    
protected:
    E_PLACE _ePlace;
    
    // 백키로 닥기 여부
    bool _bHideBackKey;     // 백키로 닫기 여부
    bool _bHideAll;         // 전체 닫기 여부
    
    // widget : 컨테이너
    cocos2d::ui::Scale9Sprite *_spriteContainer;
    
    PopupBase* _nextPopup;
};

#endif /* PopupBase_h */
