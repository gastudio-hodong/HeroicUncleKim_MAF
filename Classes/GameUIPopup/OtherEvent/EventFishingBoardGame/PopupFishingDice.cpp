//
//  PopupFishingDice.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/10/15.
//

#include "PopupFishingDice.h"

#include "GameUIPopup/OtherEvent/EventFishingBoardGame/PopupFishingReward.h"

#include "ManagerEvent/EventFishingManager.h"

USING_NS_CC;

PopupFishingDice* PopupFishingDice::create(int nDice)
{
    PopupFishingDice* pRet = new(std::nothrow) PopupFishingDice();
    if ( pRet && pRet->init(nDice) )
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

PopupFishingDice::PopupFishingDice() :
_nDice(0),

_callbackClose(nullptr)
{
    
}

PopupFishingDice::~PopupFishingDice()
{
    
}

bool PopupFishingDice::init(int nDice)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _nDice = nDice;
    
    //
    setHideBackKey(false);
    setHideAll(false);
    
    //
    initVar();
    initUI();
    
    return true;
    
}

#pragma mark - init
void PopupFishingDice::initVar()
{
    
}

void PopupFishingDice::initUI()
{
    Size size = Size(500, 580);
    
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    std::string strTitle = GAME_TEXT("t_ui_event_dice_result_1");
    if ( _nDice <= 0 )
    {
        strTitle = GAME_TEXT("t_ui_event_dice_outpnt_1");
    }
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    if ( _nDice <= 0 )
    {
        uiError();
    }
    else
    {
        uiDice();
    }
}

#pragma mark - ui
void PopupFishingDice::uiDice()
{
    //
    auto layerContents = Layer::create();
    layerContents->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - 25 - 25));
    layerContents->setIgnoreAnchorPointForPosition(false);
    layerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    layerContents->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(layerContents);
    
    
    //
    auto spriteDice = Sprite::create("Assets/ui/event/dice01_1.png");
    spriteDice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteDice->setPosition(layerContents->getContentSize().width / 2,  layerContents->getContentSize().height / 2 - 32 - 50);
    layerContents->addChild(spriteDice, 10);
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1f);
        for(int i = 1; i <= 10; i++)
        {
            std::string stdPath = MafUtils::format("Assets/ui/event/dice01_%d.png", i);
            animation->addSpriteFrameWithFile(stdPath);
        }
        animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/event/dice02_%d.png", _nDice));
        
        spriteDice->runAction(Animate::create(animation));
    }
    
    //
    auto action1 = DelayTime::create(1.1);
    auto action2 = CallFuncN::create([=](Node *sender){
        
        auto spriteEffect = Sprite::create("Assets/ui/event/event_fx1_1.png");
        spriteEffect->setPosition(sender->getContentSize().width/2,  sender->getContentSize().height/2);
        sender->addChild(spriteEffect);
        
        auto labelText = MafNode::createBmFont(MafUtils::toString(_nDice), GAME_BM_FONT_AL, 100);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelText->setPosition(sender->getContentSize().width / 2, 25 );
        labelText->setColor(Color3B(255, 225, 0));
        sender->addChild(labelText);
    });
    auto action3 = DelayTime::create(0.5);
    auto action4 = CallFunc::create([=](){
        
        if ( _callbackClose != nullptr )
        {
            _callbackClose();
        }
        
        hide();
    });
    auto seq = Sequence::create(action1, action2, action3, action4, nullptr);
    layerContents->runAction(seq);
    
}

void PopupFishingDice::uiError()
{
    //
    auto layerButton = Layer::create();
    layerButton->setContentSize(Size(_spriteContainer->getContentSize().width - 50, 89));
    layerButton->setIgnoreAnchorPointForPosition(false);
    layerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    layerButton->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 25) );
    _spriteContainer->addChild(layerButton);
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(layerButton->getContentSize().width / 2, layerButton->getContentSize().height / 2);
        layerButton->addChild(menu);
        
        // item
        auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/popup_btn_no.png"), nullptr, CC_CALLBACK_1(PopupFishingDice::onClickFishing, this));
        menu->addChild(itemClose);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_outpnt_btn_1"), GAME_FONT, 24);
            labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
            labelText->setOverflow(Label::Overflow::SHRINK);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setDimensions(itemClose->getContentSize().width - 20, labelText->getContentSize().height);
            labelText->setColor( Color3B(255,216,0) );
            itemClose->addChild(labelText);
        }
        
        //
        menu->alignItemsHorizontallyWithPadding(10);
    }
    
    //
    auto layerContents = Layer::create();
    layerContents->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - 25 - 25 - layerButton->getContentSize().height - 25));
    layerContents->setIgnoreAnchorPointForPosition(false);
    layerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    layerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25);
    _spriteContainer->addChild(layerContents);
    {
        auto spriteBG = Sprite::create("Assets/ui/event/event_game_img_1_2.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBG->setPosition(layerContents->getContentSize().width / 2, layerContents->getContentSize().height);
        layerContents->addChild(spriteBG);
        
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_event_dice_outpnt_2"), GAME_FONT, 25);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelText->setPosition(layerContents->getContentSize().width / 2, 0);
        labelText->setDimensions(layerContents->getContentSize().width - 10, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        layerContents->addChild(labelText);
    }
}

#pragma mark - set, get, sort, add
void PopupFishingDice::setCallbackClose(std::function<void(void)> callback)
{
    _callbackClose = callback;
}

#pragma mark - click
void PopupFishingDice::onClickFishing(cocos2d::Ref* sender)
{
    if ( _callbackClose != nullptr )
    {
        _callbackClose();
    }
    
    hide();
}
 
