//
//  PopupManaAMPReset.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/24.
//

#include "PopupManaAMPReset.h"

#include "GameUI/UI/UIReddot.h"

#include "GameObject/InfoItems.h"
#include "GameObject/InfoSetting.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/DefenseManager.h"


USING_NS_CC;

PopupManaAMPReset* PopupManaAMPReset::create(int number)
{
    PopupManaAMPReset* pRet = new(std::nothrow) PopupManaAMPReset();
    if ( pRet && pRet->init(number) )
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

PopupManaAMPReset::PopupManaAMPReset():
_number(0),
_lyTop(nullptr),
_lyBottom(nullptr)
{
    _onReset.Clear();
}

PopupManaAMPReset::~PopupManaAMPReset(void)
{
    _onReset.Clear();
}

bool PopupManaAMPReset::init(int number)
{
    if (!PopupBase::init())
    {
        return false;
    }
    _number = number;
    _bHideBackKey = false;
    initVar();
    initUI();
    
    return true;
}

void PopupManaAMPReset::initVar()
{
    Size size = Size(560, 530);
    
    _spriteContainer->setContentSize(size);
}

void PopupManaAMPReset::initUI()
{
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_reset_1"), GAME_FONT, 28);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(lbTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupManaAMPReset::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 254));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTop);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTop->getContentSize().height));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _spriteContainer->addChild(_lyBottom);
    
    uiTop();
    uiBottom();
}

#pragma mark - ui
void PopupManaAMPReset::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto box = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_4.png");
    box->setCapInsets(Rect(10,10,2,2));
    box->setContentSize(Size(518, 230));
    box->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(box);
    
    
    auto lbReset = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_defense_manaamp_reset_2", _number), GAME_FONT, 28);
    lbReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbReset->setPosition(Vec2(box->getContentSize()/2) + Vec2::UP * 10 + Vec2::RIGHT * 10);
    lbReset->setDimensions(box->getContentSize().width - 10, box->getContentSize().height - 10);
    lbReset->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbReset->setOverflow(Label::Overflow::SHRINK);
    _lyTop->addChild(lbReset);
}

void PopupManaAMPReset::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto btnReset = ui::Button::create("Assets/ui_common/btn_gift_bg.png");
    btnReset->setScale9Enabled(true);
    btnReset->setContentSize(Size(250,120));
    btnReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnReset->setPosition(Vec2(_lyBottom->getContentSize()/2));
    btnReset->setZoomScale(0.05f);
    btnReset->addClickEventListener(CC_CALLBACK_1(PopupManaAMPReset::onClickReset, this));
    _lyBottom->addChild(btnReset);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_defense_manaamp_reset_1"), GAME_FONT, 26);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnReset->getContentSize().width/2, btnReset->getContentSize().height - btnReset->getContentSize().height/4);
        lbText->setDimensions(btnReset->getContentSize().width - 10, btnReset->getContentSize().height/2);
        lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbText->setOverflow(Label::Overflow::SHRINK);
        btnReset->addChild(lbText);
        
        auto item = ItemsManager::getInstance()->getInfoItems(E_ITEMS::NEW_RAID_DRAGON_BALL);
        auto sprCurrencyIcon = Sprite::create(item->getPath());
        sprCurrencyIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprCurrencyIcon->setPosition(sprCurrencyIcon->getContentSize().width + 40, btnReset->getContentSize().height/4);
        btnReset->addChild(sprCurrencyIcon);
        
        int cost = 100;
        if(auto setting = DefenseManager::getInstance()->getInfoSetting("defense_manaamp_reset"))
            cost = setting->getValueToInt();
        
        auto lbCount = Label::createWithTTF(std::to_string(cost), GAME_FONT, 26);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        lbCount->setPosition(sprCurrencyIcon->getPosition() + Vec2::RIGHT * sprCurrencyIcon->getContentSize().width/2);
        lbCount->setDimensions(100, btnReset->getContentSize().height/2);
        lbCount->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        btnReset->addChild(lbCount);
    }
    
}

#pragma mark - click
void PopupManaAMPReset::onClickClose(Ref* sender)
{
    hide();
}
void PopupManaAMPReset::onClickReset(Ref* sender)
{
    _onReset.Invoke();
    hide();
}
#pragma mark - utils
