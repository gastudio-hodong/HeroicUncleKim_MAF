//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupBonsikDexDefault.h"

#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/BonsikDex/Layer/LayerDexEquip.h"
#include "GameUIPopup/Other/BonsikDex/Layer/LayerDexMonster.h"
#include "GameUIPopup/Other/BonsikDex/Layer/LayerDexLoots.h"

#include "ManagerGame/DataManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

PopupBonsikDexDefault* PopupBonsikDexDefault::create(E_TYPE eType)
{
    PopupBonsikDexDefault* pRet = new(std::nothrow) PopupBonsikDexDefault();
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

PopupBonsikDexDefault::PopupBonsikDexDefault(void) :
_eType(E_TYPE::EQUIP),

_layerContainerNow(nullptr),
_layerContainerDexEquip(nullptr),
_layerContainerDexMonster(nullptr),
_layerContainerDexLoots(nullptr)

{
    
}

PopupBonsikDexDefault::~PopupBonsikDexDefault(void)
{
    
}

bool PopupBonsikDexDefault::init(E_TYPE eType)
{
    if( !PopupBase::init() )
    {
        return false;
    }
    _eType = eType;
    //
    initVar();
    initUi();
    
    return true;
}
#pragma mark - init
void PopupBonsikDexDefault::initVar()
{
    
}

void PopupBonsikDexDefault::initUi()
{
    Size size = Size(getContentSize().width, 1327);
    
    // size
    _spriteContainer->setContentSize( size );
    _spriteContainer->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_bonsikdex"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBonsikDexDefault::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerMenu = LayerColor::create();
    _layerContainerMenu->setContentSize( Size(_spriteContainer->getContentSize().width - 12, 80) );
    _layerContainerMenu->setIgnoreAnchorPointForPosition(false);
    _layerContainerMenu->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerMenu->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerMenu);
    
    
    //
    uiMenu();
}
#pragma mark - ui
void PopupBonsikDexDefault::uiMenu()
{
    //
    _layerContainerMenu->removeAllChildren();
    
    //
    E_TYPE listMenu[] = {E_TYPE::EQUIP, E_TYPE::MONSTER, E_TYPE::LOOTS};
    std::string listMenuTextKey[] = {"t_ui_bonsikdex_menu_1", "t_ui_bonsikdex_menu_2", "t_ui_loots_title_1"};
        
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setName("MENU");
    _layerContainerMenu->addChild(menu);
        
    float nMenuWidth = _layerContainerMenu->getContentSize().width / (sizeof(listMenu) / sizeof(int));
    for ( int i = 0; i < sizeof(listMenu) / sizeof(int); i++ )
    {
        std::string strPathButton = "Assets/ui_common/btn_tap_off.png";
        if ( _eType == listMenu[i] )
        {
            strPathButton = "Assets/ui_common/btn_tap_on.png";
        }
            
        auto spriteButtonOn = ui::Scale9Sprite::create(strPathButton, Rect::ZERO, Rect(35,25,2,10));
        spriteButtonOn->setContentSize(Size(nMenuWidth, spriteButtonOn->getContentSize().height));
        auto spriteButtonSelect = ui::Scale9Sprite::create(strPathButton, Rect::ZERO, Rect(35,25,2,10));
        spriteButtonSelect->setContentSize(Size(nMenuWidth, spriteButtonOn->getContentSize().height));
            
        auto itemButton = MafNode::MafMenuItemSprite::create(spriteButtonOn, spriteButtonSelect, nullptr, CC_CALLBACK_1(PopupBonsikDexDefault::onClickMenu, this));
        itemButton->setPosition(nMenuWidth * i + itemButton->getContentSize().width /2, itemButton->getContentSize().height / 2 );
        itemButton->setTag((int)listMenu[i]);
        menu->addChild(itemButton);
            
        auto labelButton = Label::createWithTTF(GAME_TEXT(listMenuTextKey[i]), GAME_FONT, 24);
        labelButton->setPosition(itemButton->getContentSize().width / 2, itemButton->getContentSize().height / 2);
        labelButton->setTextColor(Color4B(86, 86, 86, 255));
        labelButton->setDimensions(itemButton->getContentSize().width * 0.9, labelButton->getContentSize().height);
        labelButton->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelButton->setOverflow(Label::Overflow::SHRINK);
        labelButton->setName("LABEL");
        itemButton->addChild(labelButton);
        if ( _eType == listMenu[i] )
        {
            labelButton->setTextColor(Color4B(255, 255, 255, 255));
        }
        
        if ( listMenu[i] == E_TYPE::MONSTER )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(itemButton->getContentSize().width - 20, itemButton->getContentSize().height - 20);
            uiReddot->setVisible(false);
            itemButton->addChild(uiReddot);
            
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::DEX_MONSTER_LEVELUP, uiReddot);
        }
        
        if ( listMenu[i] == E_TYPE::LOOTS )
        {
            auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
            uiReddot->setPosition(itemButton->getContentSize().width - 20, itemButton->getContentSize().height - 20);
            uiReddot->setVisible(false);
            itemButton->addChild(uiReddot);
            
            RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::DEX_LOOTS_LEVELUP, uiReddot);
        }
    }
    
    //
    drawMenu();
}

#pragma mark - set, get, other
void PopupBonsikDexDefault::drawMenu()
{
    if ( _layerContainerNow != nullptr )
    {
        _layerContainerNow->setVisible(false);
    }
    
    if ( _eType == E_TYPE::EQUIP )
    {
        if ( _layerContainerDexEquip == nullptr )
        {
            auto layer = LayerDexEquip::create();
            layer->setContentSize(Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - _layerContainerMenu->getContentSize().height - 25));
            layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            layer->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
            _spriteContainer->addChild(layer);

            _layerContainerDexEquip = layer;
        }
        _layerContainerNow = _layerContainerDexEquip;
        _layerContainerNow->setVisible(true);
    }
    else if ( _eType == E_TYPE::MONSTER )
    {
        if ( _layerContainerDexMonster == nullptr )
        {
            auto layer = LayerDexMonster::create();
            layer->setContentSize(Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - _layerContainerMenu->getContentSize().height - 25));
            layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            layer->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
            _spriteContainer->addChild(layer);

            _layerContainerDexMonster = layer;
        }
        _layerContainerNow = _layerContainerDexMonster;
        _layerContainerNow->setVisible(true);
    }
    else if ( _eType == E_TYPE::LOOTS )
    {
        if ( _layerContainerDexLoots == nullptr )
        {
            auto layer = LayerDexLoots::create(Size(_spriteContainer->getContentSize().width - 12, _spriteContainer->getContentSize().height - 75 - _layerContainerMenu->getContentSize().height - 25));
            layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            layer->setPosition(_spriteContainer->getContentSize().width / 2, 25);
            layer->setCallbackInfo(CC_CALLBACK_1(PopupBonsikDexDefault::callbackLootInfo, this));
            _spriteContainer->addChild(layer);

            _layerContainerDexLoots = layer;
        }
        _layerContainerNow = _layerContainerDexLoots;
        _layerContainerNow->setVisible(true);
    }
}

void PopupBonsikDexDefault::callbackLootInfo(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    
}

#pragma mark - click
void PopupBonsikDexDefault::onClickClose(Ref* sender)
{
    hide();
}

void PopupBonsikDexDefault::onClickMenu(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    
    PopupBonsikDexDefault::E_TYPE nType = (E_TYPE)item->getTag();
    if ( _eType == nType )
    {
        return;
    }
    
    if ( nType == E_TYPE::LOOTS && AccountManager::getInstance()->getUserIdx() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    _eType = nType;
    
    //
    uiMenu();
}
 
