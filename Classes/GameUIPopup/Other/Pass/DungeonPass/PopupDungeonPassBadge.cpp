//
//  PopupDungeonPassBadge.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/13.
//

#include "PopupDungeonPassBadge.h"

#include "GameObject/InfoBadge.h"

#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

PopupDungeonPassBadge* PopupDungeonPassBadge::create(bool bBadgeFree, bool bBadgePur)
{
    PopupDungeonPassBadge* pRet = new(std::nothrow) PopupDungeonPassBadge();
    if ( pRet && pRet->init(bBadgeFree, bBadgePur) )
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

PopupDungeonPassBadge::PopupDungeonPassBadge(void) :
_objPass(nullptr)
{
    
}

PopupDungeonPassBadge::~PopupDungeonPassBadge(void)
{
    if ( _objPass != nullptr )
    {
        _objPass->release();
        _objPass = nullptr;
    }
}

bool PopupDungeonPassBadge::init(bool bBadgeFree, bool bBadgePur)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    _bBadgeFree = bBadgeFree;
    _bBadgePur = bBadgePur;
    
    //
    initVar();
    initUi();

    return true;
}

void PopupDungeonPassBadge::onEnter()
{
    PopupBase::onEnter();
}

#pragma mark - init
void PopupDungeonPassBadge::initVar()
{
    _objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( _objPass != nullptr )
    {
        _objPass->retain();
    }
}

void PopupDungeonPassBadge::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(644, 728) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_complete"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDungeonPassBadge::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(_spriteContainer->getContentSize().width - 50, 140) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerContents);
    
    _layerContainerButton = Layer::create();
    _layerContainerButton->setContentSize( Size(_spriteContainer->getContentSize().width - 50, 88) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerBadge = Layer::create();
    _layerContainerBadge->setContentSize( Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - _layerContainerContents->getContentSize().height - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerBadge->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadge->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerBadge->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerContents->getPosition().y - _layerContainerContents->getContentSize().height);
    _spriteContainer->addChild(_layerContainerBadge);
    
    //
    uiContents();
    uiBadge();
    uiButton();
}

#pragma mark - ui
void PopupDungeonPassBadge::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_10"), GAME_FONT, 24);
    labelText->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
    labelText->setDimensions(_layerContainerContents->getContentSize().width - 50, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(labelText);
}

void PopupDungeonPassBadge::uiBadge()
{
    _layerContainerBadge->removeAllChildren();
    
    //
    auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_11"), GAME_FONT, 24);
    labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelText->setPosition(_layerContainerBadge->getContentSize().width / 2, 0);
    labelText->setDimensions(_layerContainerBadge->getContentSize().width - 50, labelText->getContentSize().height);
    labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelText->setOverflow(Label::Overflow::SHRINK);
    _layerContainerBadge->addChild(labelText);
    
    //
    cocos2d::Vector<Sprite*> listReward;
    if ( _bBadgeFree == true )
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/pass_bar_1_1.png");
        listReward.pushBack(spriteBG);
        
        //
        auto objBadge = BadgeManager::getInstance()->getBadge(_objPass->getBadgeFree());
        
        auto spriteItemsBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteItemsBG->setContentSize(Size(82, 82));
        spriteItemsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemsBG->setPosition(25, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteItemsBG);
        if ( objBadge != nullptr )
        {
            auto spriteIcon = Sprite::create(objBadge->getPath());
            spriteIcon->setPosition(spriteItemsBG->getContentSize()/2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteItemsBG->addChild(spriteIcon);
            
            //
            auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
            labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelTitle->setPosition(220, spriteBG->getContentSize().height / 2);
            labelTitle->setDimensions(220, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelTitle);
            
            auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
            labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            labelEffect->setPosition(220, spriteBG->getContentSize().height / 2);
            labelEffect->setDimensions(220, labelEffect->getContentSize().height);
            labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelEffect->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelEffect);
        }
    }
    
    if ( _bBadgePur == true )
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/pass_bar_2_1.png");
        listReward.pushBack(spriteBG);
        
        //
        auto objBadge = BadgeManager::getInstance()->getBadge(_objPass->getBadgePur());
        
        auto spriteItemsBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteItemsBG->setContentSize(Size(82, 82));
        spriteItemsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemsBG->setPosition(25, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteItemsBG);
        if ( objBadge != nullptr )
        {
            auto spriteIcon = Sprite::create(objBadge->getPath());
            spriteIcon->setPosition(spriteItemsBG->getContentSize()/2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteItemsBG->addChild(spriteIcon);
            
            //
            auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
            labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelTitle->setPosition(220, spriteBG->getContentSize().height / 2);
            labelTitle->setDimensions(220, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelTitle);
            
            auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
            labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            labelEffect->setPosition(220, spriteBG->getContentSize().height / 2);
            labelEffect->setDimensions(220, labelEffect->getContentSize().height);
            labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelEffect->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelEffect);
        }
    }
    
    
    
    //
    double nWidth = 345;
    double nHeight = 113 * listReward.size() + (10 * (listReward.size() - 1));
    
    auto layerReward = LayerColor::create(Color4B::BLUE);
    layerReward->setContentSize(Size(nWidth, nHeight));
    layerReward->setIgnoreAnchorPointForPosition(false);
    _layerContainerBadge->addChild(layerReward);
    if ( _objPass != nullptr && _objPass->isBuyPass() == true )
    {
        layerReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        layerReward->setPosition(_layerContainerBadge->getContentSize().width / 2, _layerContainerBadge->getContentSize().height / 2 + 25);
    }
    else if ( _objPass != nullptr && _objPass->isBuyPass() == false )
    {
        layerReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        layerReward->setPosition(_layerContainerBadge->getContentSize().width / 2, _layerContainerBadge->getContentSize().height - 25);
    }
    
    for ( int i = 0; i < listReward.size(); i++ )
    {
        double posX = layerReward->getContentSize().width / 2;
        double posY = layerReward->getContentSize().height - 113 * i - 10 * i;
        
        auto spriteBG = listReward.at(i);
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        spriteBG->setPosition(posX, posY);
        layerReward->addChild(spriteBG);
    }
        
    //
    if ( _objPass != nullptr && _objPass->isBuyPass() == false )
    {
        auto spriteBG = Sprite::create("Assets/ui/purchase/pass_bar_2_1.png");
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        spriteBG->setPosition(_layerContainerBadge->getContentSize().width / 2, 75);
        _layerContainerBadge->addChild(spriteBG);

        //
        auto objBadge = BadgeManager::getInstance()->getBadge(_objPass->getBadgePur());
        
        auto spriteItemsBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
        spriteItemsBG->setContentSize(Size(82, 82));
        spriteItemsBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteItemsBG->setPosition(25, spriteBG->getContentSize().height / 2);
        spriteBG->addChild(spriteItemsBG);
        if ( objBadge != nullptr )
        {
            auto spriteIcon = Sprite::create(objBadge->getPath());
            spriteIcon->setPosition(spriteItemsBG->getContentSize()/2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteItemsBG->addChild(spriteIcon);
            
            //
            auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
            labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelTitle->setPosition(220, spriteBG->getContentSize().height / 2);
            labelTitle->setDimensions(220, labelTitle->getContentSize().height);
            labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTitle->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelTitle);
            
            auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
            labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            labelEffect->setPosition(220, spriteBG->getContentSize().height / 2);
            labelEffect->setDimensions(220, labelEffect->getContentSize().height);
            labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelEffect->setOverflow(Label::Overflow::SHRINK);
            spriteBG->addChild(labelEffect);
        }
        
        //
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_14"), GAME_FONT, 24);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelText->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height + 5);
        labelText->setDimensions(spriteBG->getContentSize().width - 50, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        labelText->setColor(Color3B(255, 255, 0));
        spriteBG->addChild(labelText);
    }
    
}

void PopupDungeonPassBadge::uiButton()
{
    _layerContainerButton->removeAllChildren();


    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);

    if ( _objPass != nullptr && _objPass->isBuyPass() == false )
    {
        auto spritePurchase = ui::Scale9Sprite::create("Assets/ui_common/btn_1_3_on.png", Rect::ZERO, Rect(33,33,2,2));
        spritePurchase->setContentSize(Size(212, 88));
        
        auto itemPurchase = MafNode::MafMenuItemSprite::create(spritePurchase, nullptr, CC_CALLBACK_1(PopupDungeonPassBadge::onClickPurchase, this));
        menu->addChild(itemPurchase);
        {
            auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_8"), GAME_FONT, 24);
            labelText->setPosition(itemPurchase->getContentSize().width / 2, itemPurchase->getContentSize().height / 2);
            labelText->setDimensions(itemPurchase->getContentSize().width * 0.95, labelText->getContentSize().height);
            labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelText->setOverflow(Label::Overflow::SHRINK);
            itemPurchase->addChild(labelText);
        }
    }
    
    auto spriteClose = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteClose->setContentSize(Size(212, 88));
    
    auto itemClose = MafNode::MafMenuItemSprite::create(spriteClose, nullptr, CC_CALLBACK_1(PopupDungeonPassBadge::onClickClose, this));
    menu->addChild(itemClose);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 24);
        labelText->setPosition(itemClose->getContentSize().width / 2, itemClose->getContentSize().height / 2);
        labelText->setDimensions(itemClose->getContentSize().width * 0.95, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemClose->addChild(labelText);
    }

    //
    menu->alignItemsHorizontallyWithPadding(10);
    
}

#pragma mark - draw

#pragma mark - set, get
void PopupDungeonPassBadge::setCallbackPurchase(std::function<void(void)> callback)
{
    _callbackPurchase = callback;
}

#pragma mark - click
void PopupDungeonPassBadge::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupDungeonPassBadge::onClickPurchase(cocos2d::Ref* sender)
{
    if ( _callbackPurchase != nullptr )
    {
        _callbackPurchase();
    }
    
    //
    hide();
}
 
#pragma mark - network

#pragma mark - callback
