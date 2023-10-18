//
//  PopupBaitCharge.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/07/13.
//

#include "PopupBaitCharge.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventFishingManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupBaitCharge* PopupBaitCharge::create()
{
    PopupBaitCharge* pRet = new(std::nothrow) PopupBaitCharge();
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

PopupBaitCharge::PopupBaitCharge(void) :
_nCount(1),

_nBaitBuyRemaining(0),
_nBaitBuyMax(0),
_nBaitBuyPrice(0),

_callbackResult(nullptr),


_layerContainerContents(nullptr),
_layerContainerButton(nullptr),
_labelCount1(nullptr),
_labelCount10(nullptr),
_labelPrice(nullptr)
{
    
}

PopupBaitCharge::~PopupBaitCharge(void)
{
    
}

bool PopupBaitCharge::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupBaitCharge::initVar()
{
    _nBaitBuyRemaining = EventFishingManager::getInstance()->getBaitBuyRemaining();
    _nBaitBuyMax = EventFishingManager::getInstance()->getBaitBuyMax();
    _nBaitBuyPrice = EventFishingManager::getInstance()->getBaitBuyPrice();
}

void PopupBaitCharge::initUi()
{
    Size size = Size(455, 430);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(size.width - 50, 89) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - 25 - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_buybait_pop_title"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupBaitCharge::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    // 1 단위
    auto spriteCountBG1 = Sprite::create("Assets/ui/raid/energyblast_charge_1.png");
    spriteCountBG1->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 70);
    _layerContainerContents->addChild(spriteCountBG1);
    {
        _labelCount1 = Label::createWithTTF("1", GAME_FONT, 24);
        _labelCount1->setPosition(spriteCountBG1->getContentSize().width / 2, spriteCountBG1->getContentSize().height / 2);
        spriteCountBG1->addChild(_labelCount1);

        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteCountBG1->addChild(menu);

        auto itemUp = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_up_s.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickArrow, this));
        itemUp->setPosition(spriteCountBG1->getContentSize().width / 2, spriteCountBG1->getContentSize().height + 10 + itemUp->getContentSize().height / 2);
        itemUp->setTouchRect( itemUp->getContentSize() * 2 );
        itemUp->setTag(+1);
        menu->addChild(itemUp);

        auto itemDown = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_down_s.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickArrow, this));
        itemDown->setPosition(spriteCountBG1->getContentSize().width / 2, - 10 - itemDown->getContentSize().height / 2);
        itemDown->setTouchRect( itemDown->getContentSize() * 2 );
        itemDown->setTag(-1);
        menu->addChild(itemDown);
    }
    
    // 10 단위
    auto spriteCountBG10 = Sprite::create("Assets/ui/raid/energyblast_charge_1.png");
    spriteCountBG10->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteCountBG10->setPosition(spriteCountBG1->getPositionX() - spriteCountBG1->getContentSize().width / 2 - 10, _layerContainerContents->getContentSize().height - 70);
    _layerContainerContents->addChild(spriteCountBG10);
    {
        _labelCount10 = Label::createWithTTF("0", GAME_FONT, 24);
        _labelCount10->setPosition(spriteCountBG10->getContentSize().width / 2, spriteCountBG10->getContentSize().height / 2);
        spriteCountBG10->addChild(_labelCount10);
        
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteCountBG10->addChild(menu);
        
        auto itemUp = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_up_s.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickArrow, this));
        itemUp->setPosition(spriteCountBG10->getContentSize().width / 2, spriteCountBG10->getContentSize().height + 10 + itemUp->getContentSize().height / 2);
        itemUp->setTouchRect( itemUp->getContentSize() * 2 );
        itemUp->setTag(+10);
        menu->addChild(itemUp);
        
        auto itemDown = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_down_s.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickArrow, this));
        itemDown->setPosition(spriteCountBG10->getContentSize().width / 2, - 10 - itemDown->getContentSize().height / 2);
        itemDown->setTouchRect( itemDown->getContentSize() * 2 );
        itemDown->setTag(-10);
        menu->addChild(itemDown);
    }
    
    //
    auto spritePriceBG = Sprite::create("Assets/ui/raid/energyblast_charge_2.png");
    spritePriceBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    spritePriceBG->setPosition(spriteCountBG1->getPositionX() + spriteCountBG1->getContentSize().width / 2 + 10, _layerContainerContents->getContentSize().height - 70);
    _layerContainerContents->addChild(spritePriceBG);
    {
        auto spriteIcon = Sprite::create("Assets/icon/icon_coin_2.png");
        spriteIcon->setPosition(spritePriceBG->getContentSize().width / 2, spritePriceBG->getContentSize().height - (41 / 2));
        spritePriceBG->addChild(spriteIcon);
        
        _labelPrice = Label::createWithTTF("10", GAME_FONT, 24);
        _labelPrice->setPosition(spritePriceBG->getContentSize().width / 2, (spritePriceBG->getContentSize().height - 41) / 2);
        _labelPrice->setColor(COLOR_COSTUME_COIN);
        spritePriceBG->addChild(_labelPrice);
    }
    

    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_buybait_pop_1"), GAME_FONT, 20);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    labelText->setDimensions(_layerContainerContents->getContentSize().width * 0.9, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);

}

void PopupBaitCharge::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);
    
    auto itemConfirm = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupBaitCharge::onClickCharge, this));
    menu->addChild(itemConfirm);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_event_fish_buybait_pop_btn"), GAME_FONT, 24);
        labelText->setPosition(itemConfirm->getContentSize().width / 2, itemConfirm->getContentSize().height / 2);
        itemConfirm->addChild(labelText);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
}


#pragma mark - set, get, sort, add
void PopupBaitCharge::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupBaitCharge::onClickClose(Ref* sender)
{
    hide();
}

void PopupBaitCharge::onClickArrow(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;
    int nTag = item->getTag();
    
    //
    int nCountOld = _nCount;
    _nCount += nTag;
    
    //
    int nMin = 1;
    int nMax = atoi(ItemsManager::getInstance()->getItems(E_ITEMS::COSTUME_COIN).c_str()) / _nBaitBuyPrice;
    nMax = nMax > _nBaitBuyRemaining ? _nBaitBuyRemaining : nMax;
    
    if ( nMax <= 0 )
    {
        nMin = 0;
        nMax = 0;
    }
    
    if ( _nCount < nMin )
    {
        if ( nCountOld > nMin )
            _nCount = nMin;
        else
            _nCount = nMax;
    }
        
    
    if ( _nCount > nMax )
    {
        if ( nCountOld < nMax )
            _nCount = nMax;
        else
            _nCount = nMin;
    }
        
    
    //
    _labelCount1->setString(MafUtils::toString(_nCount%10));
    _labelCount10->setString(MafUtils::toString(_nCount/10));
    _labelPrice->setString(MafUtils::toString(_nCount*_nBaitBuyPrice));
}

void PopupBaitCharge::onClickCharge(cocos2d::Ref* sender)
{
    if ( _nCount <= 0 )
    {
        return;
    }
    
    std::string strItemsName = "";
    auto objItems = ItemsManager::getInstance()->getInfoItems(E_ITEMS::EVENT_BAIT);
    if ( objItems != nullptr )
    {
        strItemsName = objItems->getText();
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_msg_buy_common", strItemsName.c_str(), _nCount));
    popup->addButton(GAME_TEXT("t_ui_cancel"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){

        PopupLoading::show();
        EventFishingManager::getInstance()->requestBuyBait(_nCount, CC_CALLBACK_2(PopupBaitCharge::callbackBuyBait, this));

    });
    popup->show();
    
    
}

#pragma mark - callback
void PopupBaitCharge::callbackBuyBait(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_2");
        if ( nResult == 0 )
            strMessage = GAME_TEXT("t_ui_error_17");
        else if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_error_6");
        else if ( nResult == 6 )
            strMessage = GAME_TEXT("t_ui_cash_msg_9_0");
            
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){

            hide();

        });
        popup->show();
        return;
    }
    
    
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
    }
    
    hide();
}
 
