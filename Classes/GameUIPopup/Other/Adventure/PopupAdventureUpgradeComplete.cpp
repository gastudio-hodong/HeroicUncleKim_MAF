//
//  PopupAdventureUpgradeComplete.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/04/25.
//

#include "PopupAdventureUpgradeComplete.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureUpgradeComplete* PopupAdventureUpgradeComplete::create(InfoAdventureFurniture* infoFurniture)
{
    PopupAdventureUpgradeComplete* pRet = new(std::nothrow) PopupAdventureUpgradeComplete();
    if ( pRet && pRet->init(infoFurniture) )
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
PopupAdventureUpgradeComplete* PopupAdventureUpgradeComplete::create(InfoAdventureMember* infoMember)
{
    PopupAdventureUpgradeComplete* pRet = new(std::nothrow) PopupAdventureUpgradeComplete();
    if ( pRet && pRet->init(infoMember) )
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

PopupAdventureUpgradeComplete* PopupAdventureUpgradeComplete::create(InfoAdventureRelic* infoRelic)
{
    PopupAdventureUpgradeComplete* pRet = new(std::nothrow) PopupAdventureUpgradeComplete();
    if ( pRet && pRet->init(infoRelic) )
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

PopupAdventureUpgradeComplete::PopupAdventureUpgradeComplete() :
_isTutorial(false)
,_eType(PopupAdventureUpgradeComplete::E_TYPE::NONE)
,_infoFurniture(nullptr)
,_infoMember(nullptr)
,_infoRelic(nullptr)
,_layerContainerContents(nullptr)
,_callbackHide(nullptr)
{
    
}

PopupAdventureUpgradeComplete::~PopupAdventureUpgradeComplete(void)
{
    
}
bool PopupAdventureUpgradeComplete::init(InfoAdventureFurniture* infoFurniture)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _infoFurniture = infoFurniture;
    _eType = PopupAdventureUpgradeComplete::E_TYPE::OFFICE;
    
    // init
    initUi();
    
    return true;
}

bool PopupAdventureUpgradeComplete::init(InfoAdventureMember* infoMember)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _infoMember = infoMember;
    
    _eType = PopupAdventureUpgradeComplete::E_TYPE::MEMBER;
    
    // init
    initUi();
    
    return true;
}

bool PopupAdventureUpgradeComplete::init(InfoAdventureRelic* infoRelic)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    _infoRelic = infoRelic;
    
    _eType = PopupAdventureUpgradeComplete::E_TYPE::RELIC;
    
    // init
    initUi();
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        RefreshManager::getInstance()->addUI(E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE, this);
    
    return true;
}
#pragma mark - Override
void PopupAdventureUpgradeComplete::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if(eType == E_REFRESH::ADVENTURE_TUTORIAL_COMPLETE)
        this->setLocalZOrder(ZORDER_SCENE_POPUP);
}
void PopupAdventureUpgradeComplete::show()
{
    PopupBase::show();
    
    if(AdventureManager::getInstance()->getTutorialStep() < 7)
        this->setLocalZOrder(ZORDER_SCENE_FORCE_TUTORIAL);
}
void PopupAdventureUpgradeComplete::setCallbackHide(const std::function<void(void)>& callback)
{
    _callbackHide = callback;
}
void PopupAdventureUpgradeComplete::setIsTutorial(const bool isTutorial)
{
    _isTutorial = isTutorial;
}
void PopupAdventureUpgradeComplete::initUi()
{
    Size size = Size(729, 415);
    
    // size
    _spriteContainer->setContentSize(size);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, 340) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75) );
    _spriteContainer->addChild(_layerContainerContents);
    
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
            onClickClose(nullptr);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    std::string strTitle = "";
    switch (_eType)
    {
        case PopupAdventureUpgradeComplete::E_TYPE::OFFICE: strTitle = GAME_TEXT("t_ui_advt_office_title_1"); break;
        case PopupAdventureUpgradeComplete::E_TYPE::MEMBER: strTitle = GAME_TEXT("t_ui_advt_levelup_title_1"); break;
        case PopupAdventureUpgradeComplete::E_TYPE::RELIC: strTitle = GAME_TEXT("t_ui_advt_levelup_title_2"); break;
    }
    
    
    // label title
    auto labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 36);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    labelTitle->setDimensions(labelTitle->getContentSize().width * 0.75, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureUpgradeComplete::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);

    //
    switch (_eType)
    {
        case PopupAdventureUpgradeComplete::E_TYPE::OFFICE: uiContentsFurniture(); break;
        case PopupAdventureUpgradeComplete::E_TYPE::MEMBER: uiContentsMember(); break;
        case PopupAdventureUpgradeComplete::E_TYPE::RELIC: uiContentsRelic(); break;
    }
    
}
void PopupAdventureUpgradeComplete::uiContentsFurniture()
{
    _layerContainerContents->removeAllChildren();
    
    auto objProperty = _infoFurniture->getInfoUpgradeAbility();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::POINT);

    auto uiCurrency = UICurrency::create(list, Size(_layerContainerContents->getContentSize().width - 30, 40));
    uiCurrency->setPosition(Vec2(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height-25));
    _layerContainerContents->addChild(uiCurrency, E_ZORER::UI+1);
    
    //
    auto sprIconBox = Sprite::create("Assets/ui_common/box_quest.png");
    sprIconBox->setPosition(_layerContainerContents->getContentSize().width/2 - 70, _layerContainerContents->getContentSize().height - 140);
    _layerContainerContents->addChild(sprIconBox);
    
    auto sprFurniture = Sprite::create(MafUtils::format("Assets/ui/adventure/adventure_furniture_%02d_%d.png", _infoFurniture->getIdx(), _infoFurniture->getImageGrade()));
    sprFurniture->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprFurniture->getTexture()->setTexParameters(texParams);
    sprFurniture->setPosition(sprIconBox->getContentSize()/2);
    sprFurniture->setScale(2);
    sprIconBox->addChild(sprFurniture);
    
    auto lbFurniture = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_office_%d", _infoFurniture->getIdx())), GAME_FONT, 24);
    lbFurniture->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbFurniture->setPosition(sprIconBox->getPosition().x +sprIconBox->getContentSize().width/2 + 20, _layerContainerContents->getContentSize().height - 95);
    lbFurniture->setDimensions(_layerContainerContents->getContentSize().width - 20, lbFurniture->getContentSize().height);
    lbFurniture->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbFurniture->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbFurniture);
    
    auto lbFurnitureLv = Label::createWithTTF(MafUtils::format("Lv.%d", _infoFurniture->getCurrentLv()), GAME_FONT, 24);
    lbFurnitureLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbFurnitureLv->setPosition(lbFurniture->getPosition() + Vec2::DOWN * 40);
    _layerContainerContents->addChild(lbFurnitureLv);
    
    auto spriteLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spriteLine->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 250);
    _layerContainerContents->addChild(spriteLine);
    
    auto lbFurnitureContent = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_office_contents_%d", _infoFurniture->getIdx())), GAME_FONT, 22);
    lbFurnitureContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbFurnitureContent->setPosition(spriteLine->getPosition() + Vec2::UP * 10);
    lbFurnitureContent->setDimensions(_layerContainerContents->getContentSize().width - 20, lbFurnitureContent->getContentSize().height);
    lbFurnitureContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    lbFurnitureContent->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbFurnitureContent);
    
    double fCurrentEffect = 0;
    
    if(objProperty != nullptr)
        fCurrentEffect = objProperty->getEffect();
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)_infoFurniture->getType())) + " %s";
    strEffect = MafUtils::format(strEffect.c_str(),
                                 AdventureManager::getPropertyEffectText(_infoFurniture->getType(), fCurrentEffect).c_str());
    
    auto textFurnitureEffect = ui::CText::create(strEffect, GAME_FONT, 24);
    textFurnitureEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textFurnitureEffect->setPosition(spriteLine->getPosition() + Vec2::DOWN * 10);
    textFurnitureEffect->setTextAreaSize(Size(_layerContainerContents->getContentSize().width - 20, _layerContainerContents->getContentSize().height));
    textFurnitureEffect->setTextVerticalAlignment(TextVAlignment::TOP);
    textFurnitureEffect->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textFurnitureEffect->setTextOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(textFurnitureEffect);
}

void PopupAdventureUpgradeComplete::uiContentsMember()
{
    _layerContainerContents->removeAllChildren();
    auto objProperty = _infoMember->getInfoUpgradeAbility();
    
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::POINT);

    auto uiCurrency = UICurrency::create(list, Size(_layerContainerContents->getContentSize().width - 30, 40));
    uiCurrency->setPosition(Vec2(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height-25));
    _layerContainerContents->addChild(uiCurrency, E_ZORER::UI+1);
    
    //
    auto sprIconBox = Sprite::create("Assets/ui_common/box_quest.png");
    sprIconBox->setPosition(_layerContainerContents->getContentSize().width/2 - 70, _layerContainerContents->getContentSize().height - 140);
    _layerContainerContents->addChild(sprIconBox);
    
    std::string strIconPath = "Assets/ui/adventure/icon_crew_%d_%d.png";
    if(_infoMember->getCurrentLv() == 0)
    {
        strIconPath = MafUtils::format(strIconPath.c_str(), _infoMember->getImageIdx(), 1);
    }
    else
    {
        strIconPath = MafUtils::format(strIconPath.c_str(), _infoMember->getImageIdx(), _infoMember->getImageGrade());
    }
    
    auto sprMember = Sprite::create(strIconPath);
    sprMember->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprMember->getTexture()->setTexParameters(texParams);
    sprMember->setPosition(sprIconBox->getContentSize()/2);
    sprMember->setScale(2);
    sprIconBox->addChild(sprMember);
    
    auto lbMember = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_member_%d", _infoMember->getIdx())), GAME_FONT, 24);
    lbMember->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMember->setPosition(sprIconBox->getPosition().x +sprIconBox->getContentSize().width/2 + 20, _layerContainerContents->getContentSize().height - 95);
    lbMember->setDimensions(_layerContainerContents->getContentSize().width - 20, lbMember->getContentSize().height);
    lbMember->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbMember->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbMember);
    
    auto lbMemberLv = Label::createWithTTF(MafUtils::format("Lv.%d", _infoMember->getCurrentLv()), GAME_FONT, 24);
    lbMemberLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbMemberLv->setPosition(lbMember->getPosition() + Vec2::DOWN * 40);
    _layerContainerContents->addChild(lbMemberLv);
    
    auto spriteLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spriteLine->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 250);
    _layerContainerContents->addChild(spriteLine);
    
    auto lbMemberContent = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_member_%d", _infoMember->getIdx())), GAME_FONT, 22);
    lbMemberContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbMemberContent->setPosition(spriteLine->getPosition() + Vec2::UP * 10);
    lbMemberContent->setDimensions(_layerContainerContents->getContentSize().width - 20, lbMemberContent->getContentSize().height);
    lbMemberContent->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
    lbMemberContent->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbMemberContent);
    
    double fCurrentEffect = 0;
    
    if(objProperty != nullptr)
        fCurrentEffect = objProperty->getEffect();
    
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)_infoMember->getType())) + " %s";
    strEffect = MafUtils::format(strEffect.c_str(),
                                 AdventureManager::getPropertyEffectText(_infoMember->getType(), fCurrentEffect).c_str());
    
    auto textMemberEffect = ui::CText::create(strEffect, GAME_FONT, 24);
    textMemberEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textMemberEffect->setPosition(spriteLine->getPosition() + Vec2::DOWN * 10);
    textMemberEffect->setTextAreaSize(Size(_layerContainerContents->getContentSize().width - 20, _layerContainerContents->getContentSize().height));
    textMemberEffect->setTextVerticalAlignment(TextVAlignment::TOP);
    textMemberEffect->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textMemberEffect->setTextOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(textMemberEffect);
}

void PopupAdventureUpgradeComplete::uiContentsRelic()
{
    _layerContainerContents->removeAllChildren();
    
    //
    auto sprIconBox = Sprite::create("Assets/ui_common/box_quest.png");
    sprIconBox->setPosition(_layerContainerContents->getContentSize().width/2 - 70, _layerContainerContents->getContentSize().height - 140);
    _layerContainerContents->addChild(sprIconBox);
    
    std::string strIconPath = MafUtils::format("Assets/ui/adventure/adventure_relic_%d.png", _infoRelic->getIdx());
    
    auto sprRelic = Sprite::create(strIconPath);
    sprRelic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprRelic->getTexture()->setTexParameters(texParams);
    sprRelic->setPosition(sprIconBox->getContentSize()/2);
    sprRelic->setScale(2);
    sprIconBox->addChild(sprRelic);
    
    auto lbRelic = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_advt_relic_name_%d", _infoRelic->getIdx())), GAME_FONT, 24);
    lbRelic->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbRelic->setPosition(sprIconBox->getPosition().x +sprIconBox->getContentSize().width/2 + 20, _layerContainerContents->getContentSize().height - 95);
    lbRelic->setDimensions(_layerContainerContents->getContentSize().width - 20, lbRelic->getContentSize().height);
    lbRelic->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    lbRelic->setOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(lbRelic);
    
    auto lbRelicLv = Label::createWithTTF(MafUtils::format("Lv.%d", _infoRelic->getCurrentLv()), GAME_FONT, 24);
    lbRelicLv->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbRelicLv->setPosition(lbRelic->getPosition() + Vec2::DOWN * 40);
    _layerContainerContents->addChild(lbRelicLv);
    
    auto spriteLine = Sprite::create("Assets/ui/mine/week_weaponline.png");
    spriteLine->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height - 250);
    _layerContainerContents->addChild(spriteLine);
    
    double fCurrentEffect = _infoRelic->getAbilityEffect();
    std::string strEffect = GAME_TEXT(MafUtils::format("t_ui_advt_property_type_%d", (int)_infoRelic->getType())) + " %s";
    strEffect = MafUtils::format(strEffect.c_str(),
                                 AdventureManager::getPropertyEffectText(_infoRelic->getType(), fCurrentEffect).c_str());
    
    auto textRelicEffect = ui::CText::create(strEffect, GAME_FONT, 24);
    textRelicEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    textRelicEffect->setPosition(spriteLine->getPosition() + Vec2::DOWN * 10);
    textRelicEffect->setTextAreaSize(Size(_layerContainerContents->getContentSize().width - 20, _layerContainerContents->getContentSize().height));
    textRelicEffect->setTextVerticalAlignment(TextVAlignment::TOP);
    textRelicEffect->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    textRelicEffect->setTextOverflow(Label::Overflow::SHRINK);
    _layerContainerContents->addChild(textRelicEffect);
}
#pragma mark - click
void PopupAdventureUpgradeComplete::onClickClose(Ref* sender)
{
    if(!_isTutorial)
        RefreshManager::getInstance()->refreshUI(E_REFRESH::ADVENTURE);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
    
    if(_callbackHide != nullptr)
        _callbackHide();
    hide();
}
