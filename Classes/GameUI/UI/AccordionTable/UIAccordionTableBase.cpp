//
//  UIAccordionTable.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/13.
//

#include "UIAccordionTableBase.h"

USING_NS_CC;

UIAccordionTableBase* UIAccordionTableBase::create(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    UIAccordionTableBase* pRet = new(std::nothrow) UIAccordionTableBase();
    if ( pRet && pRet->init(size, list) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UIAccordionTableBase::UIAccordionTableBase(void):
_table(nullptr)
,_dLayerInterval(5)
{
    _listInfoAccordionUI.clear();
}

UIAccordionTableBase::~UIAccordionTableBase(void)
{
    _listInfoAccordionUI.clear();
}

bool UIAccordionTableBase::init(const Size size, const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    if ( !LayerColor::initWithColor(Color4B()) )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    //
    _listInfoAccordionUI = list;
    
    // init
    initVar();
    initUi();
    
    return true;
}
 
#pragma mark - init
void UIAccordionTableBase::initVar()
{
}

void UIAccordionTableBase::initUi()
{
    _table = ui::ScrollView::create();
    _table->setBounceEnabled(false);
    _table->setDirection(ui::ScrollView::Direction::VERTICAL);
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _table->setContentSize(getContentSize());
    _table->setPosition(Vec2(getContentSize().width/2, 0));
    _table->setSwallowTouches(false);
    addChild(_table);
    
    uiContainer();
}

#pragma mark - ui
void UIAccordionTableBase::uiContainer()
{
    //interface
}

#pragma mark - utils
void UIAccordionTableBase::linkLayerCallback()
{
    for(auto obj : _listLayer)
    {
        obj->setCallbackRefresh(CC_CALLBACK_0(UIAccordionTableBase::updateLayout, this));
    }
}

void UIAccordionTableBase::updateLayout()
{
    if(_table == nullptr)
        return;
    
    Vec2 pos = _table->getInnerContainerPosition();
    Vec2 beSize = _table->getInnerContainerSize();
    Vec2 afSize = Vec2(getContentSize().width, 0);
    
    for(auto obj : _listLayer)
    {
        afSize.y += obj->getContentSize().height + _dLayerInterval;
    }
    _table->setInnerContainerSize(Size(afSize));
    
    if(afSize.y > beSize.y)
        pos.y -= (afSize.y - beSize.y);
    else
        pos.y += (beSize.y - afSize.y);
    
    if(afSize.y <= _table->getContentSize().height)
        pos = Vec2(pos.x, 0);
    
    _table->moveInnerContainerCostum(pos);
    
    float posY = _table->getContentSize().height < afSize.y ? afSize.y : _table->getContentSize().height;
    for(auto obj : _listLayer)
    {
        obj->setPosition(Vec2(_table->getContentSize().width/2, posY));
        posY -= obj->getContentSize().height + _dLayerInterval;
    }
}

void UIAccordionTableBase::reloadData()
{
    for(auto obj : _listInfoAccordionUI)
    {
        auto layout = _table->getInnerContainer()->getChildByName<UIAccordionLayerBase*>(obj.first);
        if ( layout != nullptr )
        {
            layout->setList(obj.second);
            layout->reloadData();
        }
    }
    
    updateLayout();
}

void UIAccordionTableBase::setList(const std::vector<std::pair<std::string, Vector<InfoAccordionUI*>>> list)
{
    _listInfoAccordionUI.clear();
    _listInfoAccordionUI = list;
}
