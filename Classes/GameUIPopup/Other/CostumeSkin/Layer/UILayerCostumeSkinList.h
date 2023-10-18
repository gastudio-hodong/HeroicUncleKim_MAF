//
//  UILayerCostumeSkinList.h
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#ifndef UILayerCostumeSkinList_h
#define UILayerCostumeSkinList_h

#include "GameUIPopup/Other/CostumeSkin/Layer/UILayerCostumeSkinParent.h"


class InfoAccordionUI;
class InfoCostumeSkin;
class UIAccordionTableCostumeSkin;
class UILayerCostumeSkinList : public UILayerCostumeSkinParent
{
public:
    static UILayerCostumeSkinList* create();
    
    UILayerCostumeSkinList(void);
    virtual ~UILayerCostumeSkinList(void);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void refreshList() override;
    
public:
    // set, get
    
protected:
    // init
    void initVar();
    void initUi();
    
    // ui
    void uiContainer();
    void uiList();
    
    // ui draw
    void drawList();
    
    // set, get
    void setListLoad();
    
    // click
    
    // callback

private:
    std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> _listType;
    
    // widget
    cocos2d::ui::Layout* _uiContainerList;
    
    UIAccordionTableCostumeSkin* _uiList;
};

#endif /* UILayerCostumeSkinList_h */
