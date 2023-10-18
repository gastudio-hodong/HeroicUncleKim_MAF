//
//  PopupDungeonPassBadgeInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/07/22.
//

#include "PopupDungeonPassBadgeInfo.h"

#include "GameObject/InfoBadge.h"

#include "GameUIPopup/Other/Pass/DungeonPass/PopupDungeonPass.h"

#include "ManagerGame/BadgeManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

PopupDungeonPassBadgeInfo* PopupDungeonPassBadgeInfo::create()
{
    PopupDungeonPassBadgeInfo* pRet = new(std::nothrow) PopupDungeonPassBadgeInfo();
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

PopupDungeonPassBadgeInfo::PopupDungeonPassBadgeInfo(void):
_layerContainerContents(nullptr)
{
    
}

PopupDungeonPassBadgeInfo::~PopupDungeonPassBadgeInfo(void)
{
}

bool PopupDungeonPassBadgeInfo::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();

    return true;
}

void PopupDungeonPassBadgeInfo::initVar()
{
    Size size = Size(500, 480);
    
    //
    _spriteContainer->setContentSize(size);
    
}

void PopupDungeonPassBadgeInfo::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_pass_guide_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupDungeonPassBadgeInfo::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // 372
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(_spriteContainer->getContentSize() - Size(0,75));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerContents);
    
    //
    uiContents();
}

void PopupDungeonPassBadgeInfo::uiContents()
{
    _layerContainerContents->removeAllChildren();
    
    auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    
    if(objPass == nullptr)
        return;
    
    auto sprFreeBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprFreeBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprFreeBox->setContentSize(Size(100, 100));
    sprFreeBox->setPosition(_layerContainerContents->getContentSize().width/3.5, _layerContainerContents->getContentSize().height - 40);
    _layerContainerContents->addChild(sprFreeBox);
    {
        auto objBadge = BadgeManager::getInstance()->getBadge(objPass->getBadgeFree());
        
        auto sprIcon = Sprite::create(objBadge->getPath());
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprFreeBox->getContentSize()/2);
        sprFreeBox->addChild(sprIcon);
        
        auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelTitle->setPosition(sprFreeBox->getPosition() + Vec2::DOWN * (sprFreeBox->getContentSize().height + 5));
        labelTitle->setDimensions(220, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelTitle);
        
        auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
        labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelEffect->setPosition(labelTitle->getPosition() + Vec2::DOWN * labelTitle->getContentSize().height);
        labelEffect->setDimensions(220, labelEffect->getContentSize().height);
        labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        labelEffect->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelEffect);
    }
    
    auto sprPurBox = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
    sprPurBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprPurBox->setContentSize(Size(100, 100));
    sprPurBox->setPosition(_layerContainerContents->getContentSize().width - _layerContainerContents->getContentSize().width/3.5, _layerContainerContents->getContentSize().height - 40);
    _layerContainerContents->addChild(sprPurBox);
    {
        auto objBadge = BadgeManager::getInstance()->getBadge(objPass->getBadgePur());
        
        auto sprIcon = Sprite::create(objBadge->getPath());
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprFreeBox->getContentSize()/2);
        sprPurBox->addChild(sprIcon);
        
        auto labelTitle = Label::createWithTTF(objBadge->getName(), GAME_FONT, 18);
        labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelTitle->setPosition(sprPurBox->getPosition() + Vec2::DOWN * (sprPurBox->getContentSize().height + 5) );
        labelTitle->setDimensions(220, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelTitle);
        
        auto labelEffect = Label::createWithTTF(objBadge->getBuffText(), GAME_FONT, 18);
        labelEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        labelEffect->setPosition(labelTitle->getPosition() + Vec2::DOWN * labelTitle->getContentSize().height);
        labelEffect->setDimensions(220, labelEffect->getContentSize().height);
        labelEffect->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        labelEffect->setOverflow(Label::Overflow::SHRINK);
        _layerContainerContents->addChild(labelEffect);
    }
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_pass_guide"), GAME_FONT, 24);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbContent->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbContent->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbContent->setDimensions(_layerContainerContents->getContentSize().width - 10, 80);
    lbContent->setPosition(_layerContainerContents->getContentSize().width/2, 150);
    _layerContainerContents->addChild(lbContent);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerContents->addChild(menu);
    
    auto sprGoDungeonPass = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png");
    sprGoDungeonPass->setContentSize(Size(332, 86));
    
    auto itemGoDungeonPass = MafNode::MafMenuItemSprite::create(sprGoDungeonPass, nullptr, CC_CALLBACK_1(PopupDungeonPassBadgeInfo::onClickDungeonPass, this));
    itemGoDungeonPass->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemGoDungeonPass->setPosition(_layerContainerContents->getContentSize().width/2, 10);
    menu->addChild(itemGoDungeonPass);
    {
        auto lbBtn = Label::createWithTTF(GAME_TEXT("t_ui_pass_guide_btn"), GAME_FONT, 24);
        lbBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbBtn->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbBtn->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbBtn->setDimensions(itemGoDungeonPass->getContentSize().width, itemGoDungeonPass->getContentSize().height);
        lbBtn->setPosition(itemGoDungeonPass->getContentSize()/2);
        itemGoDungeonPass->addChild(lbBtn);
    }
}

void PopupDungeonPassBadgeInfo::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupDungeonPassBadgeInfo::onClickDungeonPass(cocos2d::Ref* sender)
{
    auto popup = PopupDungeonPass::create();
    popup->show();
    
    hide();
}

