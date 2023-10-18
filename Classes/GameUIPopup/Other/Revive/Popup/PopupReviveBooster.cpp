//
//  PopupReviveBooster.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 24/06/2019.
//

#include "PopupReviveBooster.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupReviveBooster* PopupReviveBooster::create()
{
    PopupReviveBooster* pRet = new(std::nothrow) PopupReviveBooster();
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

PopupReviveBooster::PopupReviveBooster(void) :
_onCallbackResult(nullptr)
{
    
}

PopupReviveBooster::~PopupReviveBooster(void)
{
    
}

bool PopupReviveBooster::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    // init
    initUi();
    
    
    return true;
}
void PopupReviveBooster::initUi()
{
    //
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale.png", Rect::ZERO, Rect(70,110,10,10));
    _spriteContainer->setContentSize(Size(497, 423));
    _spriteContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    addChild(_spriteContainer);
    
    //
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_keybooster"), GAME_FONT, 28);
    lbTitle->setAnchorPoint(Vec2(0.5,0.5));
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height - 37.5));
    _spriteContainer->addChild(lbTitle);
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_20"), GAME_FONT, 28);
    lbText->setAnchorPoint(Vec2(0.5,1));
    lbText->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.65));
    lbText->setDimensions(_spriteContainer->getContentSize().width * 0.9, lbText->getContentSize().height);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(lbText);
    
    lbText = Label::createWithTTF(GAME_TEXT("t_ui_revive_msg_21"), GAME_FONT, 20);
    lbText->setAnchorPoint(Vec2(0.5,0));
    lbText->setPosition(Vec2(_spriteContainer->getContentSize().width/2,_spriteContainer->getContentSize().height*0.30));
    lbText->setLineSpacing(-5);
    lbText->setColor(COLOR_KEY);
    lbText->setDimensions(_spriteContainer->getContentSize().width * 0.9, 90);
    lbText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(lbText);
    
    MafNode::MafMenu *menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemNo = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/revive_btn_no.png"), Sprite::create("Assets/ui_common/revive_btn_no.png"), nullptr,
                                                [&](cocos2d::Ref *sender){
                                                    hide();
                                                });
    itemNo->setAnchorPoint(Vec2(0.5,0.5));
    menu->addChild(itemNo);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_no"), GAME_FONT, 26);
        label->setPosition( Vec2(itemNo->getContentSize().width / 2, itemNo->getContentSize().height / 2) );
        label->enableBold();
        itemNo->addChild(label);
    }
    
    auto itemYes = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_booster_buy.png"), Sprite::create("Assets/ui_common/btn_booster_buy.png"), nullptr, CC_CALLBACK_1(PopupReviveBooster::onClickBuyBooster, this));
    itemYes->setAnchorPoint(Vec2(0.5,0.5));
    menu->addChild(itemYes);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_jewel.png");
        spriteIcon->setPosition( Vec2(itemYes->getContentSize().width / 2 - 20, 25) );
        itemYes->addChild(spriteIcon);
    
        auto labelCount = Label::createWithTTF( MafUtils::toString(C_KEY_BOOSTER_GEM), GAME_FONT, 24);
        labelCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
        labelCount->setPosition( Vec2(itemYes->getContentSize().width / 2, 25) );
        itemYes->addChild(labelCount);
    
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_buy"), GAME_FONT, 26);
        labelTitle->setPosition( Vec2(itemYes->getContentSize().width / 2, 60) );
        labelTitle->enableBold();
        itemYes->addChild(labelTitle);
    }
    
    itemNo->setPosition(Vec2(_spriteContainer->getContentSize().width*0.35,_spriteContainer->getContentSize().width*0.15));
    itemYes->setPosition(Vec2(_spriteContainer->getContentSize().width*0.65,_spriteContainer->getContentSize().width*0.15));
}

#pragma mark - set, get, other
void PopupReviveBooster::setCallbackResult(const std::function<void(void)>& callback)
{
    _onCallbackResult = callback;
}

#pragma mark - click
void PopupReviveBooster::onClickBuyBooster(cocos2d::Ref* pSender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    if(ItemsMoneyManager::getInstance()->getGem() < C_KEY_BOOSTER_GEM)
    {
        auto popup = PopupCurrencyDetail::create(E_ITEMS::GEM);
        popup->show();
        
        hide();
        return;
    }
        
    
    ItemsMoneyManager::getInstance()->setGem(-C_KEY_BOOSTER_GEM);
    UserInfoManager::getInstance()->setBuffTime(E_BUFF_TYPE::BUFF_KEY_BONUS,1);
    
    //
    if ( _onCallbackResult != nullptr )
    {
        _onCallbackResult();
    }
    
    // analytics
    MafAnalyticsManager::LogEvent(kAnalEventItemKeyBooster,kRepeatTrue);
    
    hide();
}
