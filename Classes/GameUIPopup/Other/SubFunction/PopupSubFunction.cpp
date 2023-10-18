//
//  PopupSubFunction.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupSubFunction.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Other/SubFunction/Mine/UILayoutMineTab.h"
#include "GameUIPopup/Other/SubFunction/MyHome/UILayoutMyHome.h"
#include "GameUIPopup/Other/SubFunction/ShopItem/UILayoutShopItem.h"
#include "GameUIPopup/Other/SubFunction/Town/UILayoutTown.h"

USING_NS_CC;

PopupSubFunction* PopupSubFunction::create(E_TYPE eType)
{
    PopupSubFunction* pRet = new(std::nothrow) PopupSubFunction();
    if ( pRet && pRet->init(eType) )
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

PopupSubFunction::PopupSubFunction() :
_eType(E_TYPE::NONE),

// widget
_uiContents(nullptr)
{
    
}

PopupSubFunction::~PopupSubFunction()
{
    if ( _uiContents != nullptr )
    {
        _uiContents->release();
        _uiContents = nullptr;
    }
}

bool PopupSubFunction::init(E_TYPE eType)
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupSubFunction");
    
    //
    _eType = eType;
        
    //
    initVar();
    initUI();
    
    return true;
}

void PopupSubFunction::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
    }
    
}

#pragma mark - init
void PopupSubFunction::initVar()
{
    if ( _eType == E_TYPE::MINE )
    {
        _uiContents = UILayoutMineTab::create();
        _uiContents->retain();
        
        _title = GAME_TEXT("t_ui_mine");
    }
    else if ( _eType == E_TYPE::MYHOME )
    {
        _uiContents = UILayoutMyHome::create();
        _uiContents->retain();
        
        _title = GAME_TEXT("t_ui_myhome_name");
    }
    else if ( _eType == E_TYPE::SHOPITEM )
    {
        _uiContents = UILayoutShopItem::create();
        _uiContents->retain();
        
        _title = GAME_TEXT("t_ui_item");
    }
    else if ( _eType == E_TYPE::TOWN )
    {
        _uiContents = UILayoutTown::create();
        _uiContents->retain();
        
        _title = GAME_TEXT("t_ui_town_title");
    }
}

void PopupSubFunction::initUI()
{
    // size
    Size sizeContent = _uiContents->getContentSize();
    Size size = Size(sizeContent.width, sizeContent.height + 75 + 25);
    size.height += 50; // uiCurrency

    _spriteContainer->setContentSize(size);
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(25);
    
    // label title
    auto labelTitle = Label::createWithTTF(_title, GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSubFunction::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    if ( _ePlace == E_PLACE::DG_DEVIL_DOM )
    {
        list.push_back(E_ITEMS::D_GOLD);
    }
    else
    {
        list.push_back(E_ITEMS::GOLD);
    }
    
    if ( _eType == E_TYPE::MINE )
    {
        list.push_back(E_ITEMS::MYTHRIL);
    }
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    uiCurrency->setName("UI_CURRENCY");
    _spriteContainer->addChild(uiCurrency);
    
    // sub menu
    _uiContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContents->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(_uiContents);
}

#pragma mark - ui

#pragma mark - draw

#pragma mark - ui : set, get

#pragma mark - callback

#pragma mark - click
void PopupSubFunction::onClickClose(Ref* sender)
{
    hide();
}


#pragma mark - game
void PopupSubFunction::setCurrency(std::vector<E_ITEMS> list)
{
    if ( auto uiCurrency = _spriteContainer->getChildByName<UICurrency*>("UI_CURRENCY"); uiCurrency != nullptr )
    {
        uiCurrency->removeFromParent();
    }
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    uiCurrency->setName("UI_CURRENCY");
    _spriteContainer->addChild(uiCurrency);
}
