//
//  PopupAdventureRelicTotalSpec.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/12.
//

#ifndef PopupAdventureRelicTotalSpec_hpp
#define PopupAdventureRelicTotalSpec_hpp

#include "GameUIPopup/Base/PopupBase.h"

#include "GameObject/InfoAccordionUI.h"

class PopupAdventureRelicTotalSpec : public PopupBase
{
public:
    static PopupAdventureRelicTotalSpec* create();
    
    PopupAdventureRelicTotalSpec(void);
    virtual ~PopupAdventureRelicTotalSpec(void);
    virtual bool init() override;
public:
    void loadData();
    void initVar();
    void initUI();
    
    void uiTop();
    void uiTable();
    
    void onClickClose(Ref* sender);
    
private:
    cocos2d::Node* _node;
    cocos2d::Layer* _lyTopUI;
    cocos2d::Layer* _lyTableUI;
    cocos2d::extension::TableView *_table;
    std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> _listType;
};


#endif /* PopupAdventureRelicTotalSpec_hpp */
