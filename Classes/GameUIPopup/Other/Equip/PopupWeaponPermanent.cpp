//
//  PopupWeaponPermanent.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 23/09/2019.
//

#include "PopupWeaponPermanent.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoWeapon.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/WeaponManager.h"

USING_NS_CC;

PopupWeaponPermanent* PopupWeaponPermanent::create(int weaponIdx)
{
    PopupWeaponPermanent* pRet = new(std::nothrow) PopupWeaponPermanent();
    if ( pRet && pRet->init(weaponIdx) )
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

PopupWeaponPermanent::PopupWeaponPermanent(void) :
_bInitUI(false),

_weaponIdx(0),
_callbackResult(nullptr),

_layerContainerInfo(nullptr),
_layerContainerButtons(nullptr),

_uiBtnBuy(nullptr)
{
    
}

PopupWeaponPermanent::~PopupWeaponPermanent(void)
{
    
}

bool PopupWeaponPermanent::init(int weaponIdx)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _weaponIdx = weaponIdx;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void PopupWeaponPermanent::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiButtons();
    }
    
    //
    drawInfo();
    drawButtons();
}

#pragma mark - init
void PopupWeaponPermanent::initVar()
{
    
}

void PopupWeaponPermanent::initUi()
{
    Size size = Size(550, 600);
    
    //
    _spriteContainer->setContentSize( size );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_weapon_permanent_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupWeaponPermanent::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupWeaponPermanent::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25));
    _spriteContainer->addChild(layout);
    
    _layerContainerButtons = ui::Layout::create();
    _layerContainerButtons->setContentSize( Size(size.width, 116) );
    _layerContainerButtons->setIgnoreAnchorPointForPosition(false);
    _layerContainerButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_layerContainerButtons);
    
    _layerContainerInfo = ui::Layout::create();
    _layerContainerInfo->setContentSize( Size(size.width, size.height - _layerContainerButtons->getContentSize().height) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_layerContainerInfo);
}

void PopupWeaponPermanent::uiInfo()
{
    auto layout = _layerContainerInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiIconBG = ui::ImageView::create("Assets/ui/mine/week_weaponbox.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiIconBG->setPosition(Vec2(size.width / 2, size.height));
    uiIconBG->setName("UI_ICONBG");
    layout->addChild(uiIconBG);
    
    // 라인
    auto uiLine = ui::ImageView::create("Assets/ui/mine/week_weaponline.png");
    uiLine->setPosition(Vec2(size.width/2, size.height - 200));
    layout->addChild(uiLine);
    
    //
    auto uiTip = ui::CText::create(GAME_TEXT("t_ui_weapon_permanent_2"), GAME_FONT, 24);
    uiTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiTip->setPosition(Vec2(size.width / 2, 25));
    layout->addChild(uiTip);
    {
        auto uiIcon = ui::ImageView::create("Assets/ui/mail/icon_tip.png");
        uiIcon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiIcon->setPosition(Vec2(0, uiTip->getContentSize().height));
        uiTip->addChild(uiIcon);
    }
    
    
}

void PopupWeaponPermanent::uiButtons()
{
    auto layout = _layerContainerButtons;
    Size size = layout->getContentSize();
    
    _uiBtnBuy = ui::Button::create("Assets/ui_common/btn_weaponkeep.png");
    _uiBtnBuy->setPosition(Vec2(size.width / 2, size.height / 2));
    _uiBtnBuy->addClickEventListener(CC_CALLBACK_1(PopupWeaponPermanent::onClickPermanent, this));
    layout->addChild(_uiBtnBuy);
}

#pragma mark - draw
void PopupWeaponPermanent::drawInfo()
{
    auto layout = _layerContainerInfo;
    
    if ( auto uiIconBG = layout->getChildByName<ui::ImageView*>("UI_ICONBG"); uiIconBG != nullptr )
    {
        uiIconBG->removeAllChildren();
        
        auto objWeapon = WeaponManager::getInstance()->getInfoWeapon(_weaponIdx);
        
        int weaponIdx = objWeapon->getIdx();
        std::string pathIcon = WeaponManager::getInstance()->getImagePath(_weaponIdx);
        std::string nameText = MafUtils::format("%d. %s", weaponIdx, objWeapon->getName().c_str());
        
        if ( objWeapon->getLegend() > 1 )
        {
            uiIconBG->loadTexture("Assets/ui/mine/week_weaponbox_2_1.png");
        }
        else if ( objWeapon->getLegend() > 0 )
        {
            uiIconBG->loadTexture("Assets/ui/mine/week_weaponbox.png");
        }
        
        if ( objWeapon->getLegend() > 1 )
        {
            auto uiEffect = ui::ImageView::create("Assets/ui/mine/week_weaponbox_2_2.png");
            uiEffect->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
            uiEffect->runAction(RepeatForever::create(RotateBy::create(10, 360)));
            uiIconBG->addChild(uiEffect);
        }
        
        //
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIcon->setScale((uiIconBG->getContentSize().width - 20) / uiIcon->getContentSize().width);
        uiIconBG->addChild(uiIcon);
        
        //
        auto uiName = ui::CText::create(nameText, GAME_FONT, 26);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiName->setPosition(Vec2(uiIconBG->getContentSize().width / 2, -10));
        uiName->setTextColor(Color4B(COLOR_COIN));
        uiIconBG->addChild(uiName);
        if ( objWeapon->getLegend() > 1 )
        {
            uiName->setTextColor(Color4B(0, 245, 225, 255));
        }
        else if ( objWeapon->getLegend() > 0 )
        {
            uiName->setTextColor(Color4B(212, 34, 204, 255));
        }
            
    }
}

void PopupWeaponPermanent::drawButtons()
{
    auto layout = _layerContainerInfo;
    
    _uiBtnBuy->removeAllChildren();
    
    int weaponIdx = _weaponIdx;
    
    auto itemGoldNeed = WeaponManager::getInstance()->getBuyPrice(weaponIdx+BUY_WEAPON_PERMANENT_GOLD_GRADE, 0);
    auto itemGoldNow = ItemsMoneyManager::getInstance()->getGold();
    
    auto itemGemNeed = BUY_WEAPON_PERMANENT_GEM;
    auto itemGemNow = ItemsMoneyManager::getInstance()->getGem();
    
    std::string pathBtn = "Assets/ui_common/btn_weaponkeep.png";
    if ( MafUtils::compareStr(itemGoldNow, itemGoldNeed) == false )
    {
        pathBtn = "Assets/ui_common/btn_weaponkeep_off.png";
    }
    
    if ( itemGemNow < itemGemNeed )
    {
        pathBtn = "Assets/ui_common/btn_weaponkeep_off.png";
    }
    _uiBtnBuy->loadTextureNormal(pathBtn);
    
    //
    {
        auto uiItemIcon = ui::ImageView::create("Assets/icon/menu_coin.png");
        uiItemIcon->setPosition(Vec2(_uiBtnBuy->getContentSize().width * 0.175, _uiBtnBuy->getContentSize().height * 0.75));
        _uiBtnBuy->addChild(uiItemIcon);

        itemGoldNeed = MafUtils::convertNumberToShort(itemGoldNeed);
        auto uiItemCount = ui::CText::create(itemGoldNeed, GAME_FONT, 24);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(_uiBtnBuy->getContentSize().width * 0.35, _uiBtnBuy->getContentSize().height * 0.75));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        _uiBtnBuy->addChild(uiItemCount);
    }
    
    {
        auto uiItemIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiItemIcon->setPosition(Vec2(_uiBtnBuy->getContentSize().width * 0.175, _uiBtnBuy->getContentSize().height * 0.3));
        _uiBtnBuy->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(MafUtils::format("%d",itemGemNeed), GAME_FONT, 24);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(_uiBtnBuy->getContentSize().width * 0.35, _uiBtnBuy->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_GEM));
        _uiBtnBuy->addChild(uiItemCount);
    }
}

#pragma mark - set, get
void PopupWeaponPermanent::setCallbackResult(const std::function<void(int)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupWeaponPermanent::onClickClose(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    hide();
}

void PopupWeaponPermanent::onClickPermanent(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int weaponIdx = _weaponIdx;
    
    auto itemGoldNeed = WeaponManager::getInstance()->getBuyPrice(weaponIdx+BUY_WEAPON_PERMANENT_GOLD_GRADE, 0);
    auto itemGoldNow = ItemsMoneyManager::getInstance()->getGold();
    
    auto itemGemNeed = BUY_WEAPON_PERMANENT_GEM;
    auto itemGemNow = ItemsMoneyManager::getInstance()->getGem();
    
    if ( MafUtils::compareStr(itemGoldNow, itemGoldNeed) == false )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GOLD);
        popup->show();
        
        hide();
        return;
    }
    
    if ( itemGemNow < itemGemNeed )
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        
        hide();
        return;
    }
    
    if ( _callbackResult != nullptr )
    {
        _callbackResult(_weaponIdx);
    }
    
    hide();
}
