//
//  LayerNormalPet.h
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#ifndef LayerNormalPet_h
#define LayerNormalPet_h

#include "Common/ConfigDefault.h"
#include "Common/ConfigEnum.h"
#include "Common/Observer/IRefresh.h"

class LayerNormalPet : public cocos2d::ui::Layout, public IRefresh
{
public:
    static LayerNormalPet* create();

    LayerNormalPet(void);
    virtual ~LayerNormalPet(void);
    virtual bool init() override;
    virtual void onEnter() override;
    
    // refresh
    virtual void refresh(E_REFRESH eType, cocos2d::ValueMap listData) override;
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui : defalut
    void uiContainer();
    void uiInfo();
    void uiList();
    
    // ui : draw
    void drawInfo();
    
    // ui : set, get
    
    // ui : click
    void onClickPetEquip(cocos2d::Ref* sender);
    
    // callback
    
    // schedule

private:
    bool _bInitUI;
    
    //
    
    // widget
    cocos2d::ui::Layout* _uiContainerInfo;
    cocos2d::ui::Layout* _uiContainerList;


};
#endif /* LayerNormalPet_h */
