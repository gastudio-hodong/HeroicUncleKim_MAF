//
//  PopupPetEquipStatus.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 31/07/2019.
//

#ifndef PopupPetEquipStatus_hpp
#define PopupPetEquipStatus_hpp

#include "GameUIPopup/Base/PopupBase.h"

class InfoPetEquip;
class PopupPetEquipStatus : public PopupBase
{
public:
    static PopupPetEquipStatus* create(int idx);
    
    PopupPetEquipStatus(void);
    virtual ~PopupPetEquipStatus(void);
    virtual bool init(int idx);
    virtual void onEnter() override;
    
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiInfoLevel();

    // draw
    void drawInfo();
    void drawInfoLevel();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickLevelUP(cocos2d::Ref* sender);
    
    // callback
    void callbackEquipLevelUP(bool bResult);
    
private:
    bool _bInitUI;

    //
    int _idx;
    
    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerInfoLevel;
    
     
};


#endif /* PopupPetEquipStatus_hpp */
