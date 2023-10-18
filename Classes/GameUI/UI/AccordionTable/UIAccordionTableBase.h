//
//  UIAccordionTable.hpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#ifndef UIAccordionTable_hpp
#define UIAccordionTable_hpp

#include "Common/ConfigDefault.h"
#include "Common/Observer/IRefresh.h"

#include "GameObject/InfoAccordionUI.h"
#include "GameUI/UI/AccordionTable/UIAccordionLayer/UIAccordionLayerBase.h"

class UIAccordionTableBase : public cocos2d::LayerColor
{
public:
    static UIAccordionTableBase* create(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list);
    
    UIAccordionTableBase(void);
    virtual ~UIAccordionTableBase(void);
    virtual bool init(const cocos2d::Size size, const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list);
    
protected:
    // init
    virtual void initUi();
    virtual void initVar();
    
    
public:
    void reloadData();
    void setList(const std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> list);
    
protected:
    ///interface
    virtual void uiContainer();
    
    //utils
    virtual void linkLayerCallback() final;
    virtual void updateLayout() final;
    
protected:
    ///레이어간 간격 변수
    double _dLayerInterval;
    // widget
    cocos2d::ui::ScrollView *_table;
    
    std::vector<std::pair<std::string, cocos2d::Vector<InfoAccordionUI*>>> _listInfoAccordionUI;
    
    cocos2d::Vector<UIAccordionLayerBase*> _listLayer;
};

#endif /* UIAccordionTable_hpp */
