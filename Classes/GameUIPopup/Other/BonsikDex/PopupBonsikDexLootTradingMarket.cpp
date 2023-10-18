//
//  PopupBonsikDexLootTradingMarket.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupBonsikDexLootTradingMarket.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/TradingMarket/LayerTradingMarket.h"

USING_NS_CC;

PopupBonsikDexLootTradingMarket* PopupBonsikDexLootTradingMarket::create()
{
    PopupBonsikDexLootTradingMarket* pRet = new(std::nothrow) PopupBonsikDexLootTradingMarket();
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

PopupBonsikDexLootTradingMarket::PopupBonsikDexLootTradingMarket() :
_bInitUI(false),

//

// widget
_uiContainer(nullptr)
{
    
}

PopupBonsikDexLootTradingMarket::~PopupBonsikDexLootTradingMarket(void)
{
    
}

bool PopupBonsikDexLootTradingMarket::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupBonsikDexLootTradingMarket");
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupBonsikDexLootTradingMarket::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
    }
    
    //
    
}

#pragma mark - init
void PopupBonsikDexLootTradingMarket::initVar()
{

}

void PopupBonsikDexLootTradingMarket::initUI()
{
    // size
    Size size = Size(718, 662);

    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_msg_35"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBonsikDexLootTradingMarket::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::LOOT_PIECE_1);
    list.push_back(E_ITEMS::LOOT_PIECE_2);
    list.push_back(E_ITEMS::LOOT_PIECE_3);
    list.push_back(E_ITEMS::LOOT_PIECE_4);
    list.push_back(E_ITEMS::LOOT_PIECE_5);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width, 50));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
}

#pragma mark - ui
void PopupBonsikDexLootTradingMarket::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 38;
    size.height = size.height - 75 - 50 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    auto uiContainer = LayerTradingMarket::create(1);
    uiContainer->setContentSize(Size(size.width, size.height));
    uiContainer->setPosition(Vec2(size.width / 2, size.height / 2));
    uiContainer->setCallbackInfo(CC_CALLBACK_1(PopupBonsikDexLootTradingMarket::callbackInfo, this));
    layout->addChild(uiContainer);
    
    _uiContainer = uiContainer;
    
    //
}

#pragma mark - draw
 
#pragma mark - set, get

#pragma mark - callback
void PopupBonsikDexLootTradingMarket::callbackInfo(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
}

#pragma mark - schedule

#pragma mark - click
void PopupBonsikDexLootTradingMarket::onClickClose(Ref* sender)
{
    hide();
}

