//
//  UILayerCostumeSkinList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "UILayerCostumeSkinList.h"

#include "GameObject/InfoAccordionUI.h"

#include "GameUIPopup/Other/CostumeSkin/UIAccordion/UIAccordionTableCostumeSkin.h"

#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

UILayerCostumeSkinList* UILayerCostumeSkinList::create()
{
    UILayerCostumeSkinList* pRet = new(std::nothrow) UILayerCostumeSkinList();
    if ( pRet && pRet->init() )
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

UILayerCostumeSkinList::UILayerCostumeSkinList(void)
{
    
}

UILayerCostumeSkinList::~UILayerCostumeSkinList(void)
{
    for(auto obj : _listType)
        obj.second.clear();
    
    _listType.clear();
}

bool UILayerCostumeSkinList::init()
{
    if ( !UILayerCostumeSkinParent::init() )
    {
        return false;
    }
    
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayerCostumeSkinList::onEnter()
{
    UILayerCostumeSkinParent::onEnter();
    
    // init
    initVar();
    initUi();
}

void UILayerCostumeSkinList::refreshList()
{
    //
    setListLoad();
    
    // draw
    drawList();
}

#pragma mark - init
void UILayerCostumeSkinList::initVar()
{
    //
    setListLoad();
}

void UILayerCostumeSkinList::initUi()
{
    uiContainer();
    uiList();
}


#pragma mark - ui
void UILayerCostumeSkinList::uiContainer()
{
    Size size = getContentSize();
    
    //
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize( Size(size.width, size.height) );
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerList->setPosition( Vec2(size.width / 2, size.height) );
    addChild(_uiContainerList);
}

void UILayerCostumeSkinList::uiList()
{
    _uiContainerList->removeAllChildren();
    
    auto layout = _uiContainerList;
    auto size = layout->getContentSize();
    
    //
    _uiList = UIAccordionTableCostumeSkin::create(size, _listType);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(size.width / 2, 0);
    layout->addChild(_uiList);
}

#pragma mark - ui draw
void UILayerCostumeSkinList::drawList()
{
    _uiList->setList(_listType);
    _uiList->reloadData();
}

#pragma mark - set, get, add
void UILayerCostumeSkinList::setListLoad()
{
    //
    _listType.clear();
    
    
    auto listCostumeSkin = CostumeSkinManager::getInstance()->getListCostumeSkin();
    cocos2d::Vector<InfoCostumeSkin*> listSkinOwned;
    cocos2d::Vector<InfoCostumeSkin*> listSkinNonOwned;
    
    for ( auto& objSkin : listCostumeSkin )
    {
        bool bBuy = CostumeSkinManager::getInstance()->isCostumeSkinBuy(objSkin->getIdx());
        if ( bBuy == true )
        {
            listSkinOwned.pushBack(objSkin);
        }
        else
        {
            listSkinNonOwned.pushBack(objSkin);
        }
    }
    
    
    // owned
    {
        cocos2d::Vector<InfoAccordionUI*> list;
        for ( auto& objSkin : listSkinOwned )
        {
            auto infoAccordionUI = InfoAccordionUI::create();
            infoAccordionUI->setIdx(objSkin->getIdx());
            
            list.pushBack(infoAccordionUI);
        }
        
        _listType.push_back(std::make_pair("own", list));
    }
    
    // non owned
    {
        cocos2d::Vector<InfoAccordionUI*> list;
        for ( auto& objSkin : listSkinNonOwned )
        {
            auto infoAccordionUI = InfoAccordionUI::create();
            infoAccordionUI->setIdx(objSkin->getIdx());
            
            list.pushBack(infoAccordionUI);
        }
        _listType.push_back(std::make_pair("non_own", list));
    }
}

#pragma mark - click

#pragma mark - callback
