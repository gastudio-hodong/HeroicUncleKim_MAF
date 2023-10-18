//
//  PopupPet.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#ifndef PopupPet_h
#define PopupPet_h

#include "GameUIPopup/Base/PopupBase.h"
#include "Common/Observer/IRefresh.h"

class PopupPet : public PopupBase, public IRefresh
{
private :
    enum class E_TAB {
        NONE = 0,
        
        CAPABILITY = 1, //능력
        EVOLUTION = 2, // 각성
        REINFORCE = 3, // 강화
        EQUIP = 4, // 장비
        SKIN = 5, // 코스튬
    };
    
public:
    static PopupPet* create(E_PET ePet);
    
    PopupPet(void);
    virtual ~PopupPet(void);
    virtual bool init(E_PET ePet);
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
protected:
    
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiInfo();
    void uiTab();
    void uiEquip();
    
    //
    void drawInfo();
    void drawTab();
    void drawEquip();
    
    // click
    void onClickClose(cocos2d::Ref* sender);
    void onClickStory(cocos2d::Ref* sender);
    void onClickPet(cocos2d::Ref* sender);
    void onClickPetEquip(cocos2d::Ref* sender);
    void onClickPetPrincessSwitch(cocos2d::Ref* sender);
    void onClickTab(cocos2d::Ref* sender);
    
private:
    bool _bInitUI;
    
    //
    std::vector<E_PET> _listPet;
    
    E_PET _ePet;
    PopupPet::E_TAB _eTab;
    
    bool _bEvent;

    // widget
    cocos2d::ui::Layout* _layerContainerInfo;
    cocos2d::ui::Layout* _layerContainerTab;
    cocos2d::ui::Layout* _layerContainerEquip;
    
    
};

#endif /* PopupPet_h */
