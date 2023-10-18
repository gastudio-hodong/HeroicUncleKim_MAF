//
//  PopupStat.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 07/01/2020.
//

#include "PopupStat.h"

#include "ManagerGame/UserInfoManager.h"

#include "GameUIPopup/Other/User/PopupUserNickname.h"
#include "GameUIPopup/Other/Costume/PopupCostumeDefault.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/PetNewDevilManager.h"
#include "ManagerGame/TutorialManager.h"

USING_NS_CC;

PopupStat* PopupStat::create(E_PLACE ePlace)
{
    PopupStat* pRet = new(std::nothrow) PopupStat();
    if ( pRet && pRet->init(ePlace) )
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
PopupStat::PopupStat(void) :
_statNormal(nullptr),
_statDevil(nullptr),
_statCostume(nullptr),
_statTreasure(nullptr),
   
_layerContainerParent(nullptr),
_layerContainerStatNormal(nullptr),
_layerContainerTreasure(nullptr),
_layerContainerCostume(nullptr),
_layerContainerStatDevil(nullptr),
   
_hero(nullptr),
   
_sprPrincessBuff(nullptr),

_labelNick(nullptr)
{
    
}

PopupStat::~PopupStat(void)
{
    unschedule(schedule_selector(PopupStat::scheduleStatDraw));
    for (auto buff : _vecAnimatePrincessBuff)
    {
        buff->release();
    }
}

bool PopupStat::init(E_PLACE ePlace)
{
    if ( !PopupBase::init() )
    {
        return false;
    }
     
    _ePlace = ePlace;
    
    // init
    initVar();
    initUi();
    
    initBuffIcon();
    initHero();
    initPet();
    if ( _ePlace == DG_NORMAL )
    {
        initButton();
        
        MenuItem* item = MenuItem::create();
        item->setTag(0);
        onClickSwitchLayer(item);
    }
    scheduleStatDraw(0.0f);
    schedule(schedule_selector(PopupStat::scheduleStatDraw), 3.0f);
    
    //
    RefreshManager::getInstance()->addUI(E_REFRESH::COSTUME, this);
    
    return true;
}

void PopupStat::onEnter()
{
    PopupBase::onEnter();
}

void PopupStat::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::COSTUME )
    {
        drawHero();
    }
}

#pragma mark - init
void PopupStat::initVar()
{
    if ( _ePlace == DG_NORMAL)
    {
        _statNormal = StatFirstPage::create(DG_NORMAL);
        _layerContainerStatNormal = _statNormal;
    
        _statCostume = StatCostume::create();
        _layerContainerCostume = _statCostume;
        
        _statTreasure = StatTreasure::create(_ePlace);
        _layerContainerTreasure = _statTreasure;
    }
    else if( _ePlace == DG_DEVIL_DOM)
    {
        _statDevil = StatFirstPage::create(DG_DEVIL_DOM);
        _layerContainerStatDevil = _statDevil;
    }
}

void PopupStat::initUi()
{
    Size size = Size(750, Director::getInstance()->getVisibleSize().height - UserInfoManager::getInstance()->_fHeightIphoneX);

    //
    _spriteContainer->setContentSize( size );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPosition(getContentSize().width / 2, 0);
    
    //
    _layerContainerParent = Layer::create();
    _layerContainerParent->setContentSize( Size(size.width, size.height - 75 - 25 - 25) );
    _layerContainerParent->setIgnoreAnchorPointForPosition(false);
    _layerContainerParent->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerParent->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerParent);
    
    if ( _ePlace == DG_NORMAL )
    {
        _layerContainerStatNormal->setPosition( Vec2(_layerContainerParent->getContentSize().width / 2, 0));
        _layerContainerParent->addChild(_layerContainerStatNormal);
        
        _layerContainerTreasure->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0));
        _layerContainerParent->addChild(_layerContainerTreasure);
        
        _layerContainerCostume->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0));
        _layerContainerParent->addChild(_layerContainerCostume);
        
    }
    else if ( _ePlace == DG_DEVIL_DOM )
    {
        _layerContainerStatDevil->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, 0));
        _layerContainerParent->addChild(_layerContainerStatDevil);
    }
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_stat"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    labelTitle->setName("TITLE");
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupStat::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    std::string strNick = AccountManager::getInstance()->getUserNick();
    if ( strNick.length() != 0 )
    {
        strNick = "NICK : " + strNick;
        
        _labelNick = Label::createWithTTF(strNick, GAME_FONT, 20);
        _labelNick->setAnchorPoint(Vec2(0,0.5));
        _labelNick->setPosition(Vec2(15,_spriteContainer->getContentSize().height*0.97));
        _labelNick->setColor(COLOR_COIN);
        _spriteContainer->addChild(_labelNick);
        if ( _labelNick->getContentSize().width > 200 )
        {
            _labelNick->setDimensions(200, _labelNick->getContentSize().height);
            _labelNick->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
            _labelNick->setOverflow(Label::Overflow::SHRINK);
        }
        
        auto itemNick = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/friend/btn_friend_refresh.png"), nullptr, CC_CALLBACK_1(PopupStat::onClickNickChange, this));
        itemNick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        itemNick->setPosition(_labelNick->getPosition().x + _labelNick->getContentSize().width + 10, _spriteContainer->getContentSize().height - 37.5);
        menu->addChild(itemNick);
    }
}

void PopupStat::initBuffIcon()
{
    std::vector<std::string> vecBuffPath;
    
    if ( _ePlace == DG_NORMAL)
    {
        vecBuffPath.push_back("Assets/ui_common/icon_rage.png");
        vecBuffPath.push_back("Assets/ui_common/icon_double.png");
        vecBuffPath.push_back("Assets/ui_common/icon_speed.png");
    }
    else if ( _ePlace == DG_DEVIL_DOM)
    {
        vecBuffPath.push_back("Assets/ui_common/icon_buff_atk.png");
        vecBuffPath.push_back("Assets/ui_common/icon_buff_quest.png");
        vecBuffPath.push_back("Assets/ui_common/icon_buff_spd.png");
        vecBuffPath.push_back("Assets/ui_common/icon_buff_atkspd.png");
    }
    
    for ( int i = 0; i < vecBuffPath.size(); i++)
    {
        Sprite* sprite = Sprite::create(vecBuffPath.at(i));
        sprite->setPosition(Vec2(_spriteContainer->getContentSize().width*0.04 + ( sprite->getContentSize().width + 5 ) * i, _spriteContainer->getContentSize().height-100));
        _spriteContainer->addChild(sprite);
        _vecBuffIcon.pushBack(sprite);
    }
}

void PopupStat::initHero()
{
    if ( _ePlace == E_PLACE::DG_NORMAL )
    {
        _hero = UnitBehaviorHero::create(E_PLACE::DG_NORMAL);
        _hero->setMotion();
    }
    else
    {
        _hero = UnitBehaviorHero::create(E_PLACE::DG_DEVIL_DOM);
        _hero->setMotionDevil(true);
    }
    _hero->setTalkShow(false);
    _hero->actionRun();
    _hero->getLayerCharacter()->setScale(0.75);
    auto newPos =  _hero->getLayerCharacter()->getBoundingBox().size.width*0.5;
    _hero->setPosition(Vec2(_spriteContainer->getContentSize().width*0.55+newPos, _spriteContainer->getContentSize().height*0.8));
    _spriteContainer->addChild(_hero, 10);
    
    if ( _ePlace == DG_NORMAL)
    {
        _sprPrincessBuff = Sprite::create("Assets/character_pet/normal/effect/effect_princess_attack_1.png");
        _sprPrincessBuff->setVisible(false);
        _sprPrincessBuff->setAnchorPoint(Vec2(0,0));
        _sprPrincessBuff->setPosition(Vec2(0,0));
        _hero->getLayerCharacter()->addChild(_sprPrincessBuff, 10);
        
        Vector<cocos2d::SpriteFrame*> _vec;
        _vec.clear();
        for(int i=1; i <=4; i++){
            Animation* animation = Animation::create();
            animation->setDelayPerUnit(0.1f);
            if(i == 1){
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_attack_1.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_attack_2.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_attack_3.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_attack_4.png");
            }
            else if(i == 2){
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_move_1.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_move_2.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_move_3.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_move_4.png");
            }
            else if(i == 3){
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_cri_1.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_cri_2.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_cri_3.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_cri_4.png");
            }
            else if(i == 4){
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_all_1.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_all_2.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_all_3.png");
                animation->addSpriteFrameWithFile("Assets/character_pet/normal/effect/effect_princess_all_4.png");
            }
            Animate* animate = Animate::create(animation);
            _vecAnimatePrincessBuff.pushBack(animate);
            animate->retain();
        }
        
        // costume
        {
            
        }
        
    }
    else if ( _ePlace == DG_DEVIL_DOM)
    {
        _hero->setTalkShow(false);
    }

}

void PopupStat::initPet(){

    Pet* pet;
    if ( _ePlace == DG_NORMAL)
    {
        if(PetNewManager::getInstance()->getPet(E_PET::CHICK_1))
        {
            pet = Chick::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-70,-5));
            _hero->getLayerCharacter()->addChild(pet,1);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::HAMSTER_2))
        {
            pet = Hamster::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0.5,0));
            pet->setPosition(Vec2(-60,0));
            _hero->getLayerCharacter()->addChild(pet,1);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::DRAGON_3))
        {
            pet = Dragon::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-105,10));
            _hero->getLayerCharacter()->addChild(pet,0);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::PRINCESS_4))
        {
            pet = Princess::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-245,0));
            _hero->getLayerCharacter()->addChild(pet,2);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::PURIRING_5))
        {
            pet = Puriring::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0.5,0));
            pet->setPosition(Vec2(-105.5,8));
            _hero->getLayerCharacter()->addChild(pet,2);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::HONG_6))
        {
            pet = Hong::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(100,10));
            _hero->getLayerCharacter()->addChild(pet,0);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::HANDSOME_8))
        {
            pet = Handsome::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0.5,0));
            pet->setPosition(Vec2(220,0));
            _hero->getLayerCharacter()->addChild(pet,0);
        }
        
        if(PetNewManager::getInstance()->getPet(E_PET::MOLE_7))
        {
            pet = Mole::create();
            pet->setPlace(E_PLACE::DG_NORMAL);
            pet->setAnchorPoint(Vec2(0.5,0));
            pet->setPosition(Vec2(160,0));
            _hero->getLayerCharacter()->addChild(pet,5);
        }
    }
    else if(_ePlace == DG_DEVIL_DOM)
    {
        if(PetNewDevilManager::getInstance()->getPet(E_PET::D_1_PENGUIN)){
            pet = Penguin::create();
            pet->setPlace(E_PLACE::DG_DEVIL_DOM);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-120,36));
            pet->setScale(DevilScale);
            _hero->getLayerCharacter()->addChild(pet,1);
        }
        
        if(PetNewDevilManager::getInstance()->getPet(E_PET::D_2_HEDGEHOG)){
            pet = Hedgehog::create();
            pet->setPlace(E_PLACE::DG_DEVIL_DOM);
            pet->setAnchorPoint(Vec2(0.5,0));
            pet->setPosition(Vec2(-90,36));
            pet->setScale(DevilScale);
            _hero->getLayerCharacter()->addChild(pet,1);
        }
        
        if(PetNewDevilManager::getInstance()->getPet(E_PET::D_3_GATEKEEPER)){
            pet = Gatekeeper::create();
            pet->setPlace(E_PLACE::DG_DEVIL_DOM);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-165,24));
            pet->setScale(DevilScale);
            _hero->getLayerCharacter()->addChild(pet,1);
        }
        
        if(PetNewDevilManager::getInstance()->getPet(E_PET::D_4_FLASH)){
            pet = Flash::create();
            pet->setPlace(E_PLACE::DG_DEVIL_DOM);
            pet->setAnchorPoint(Vec2(0,0));
            pet->setPosition(Vec2(-240,45));
            pet->setScale(DevilScale);
            _hero->getLayerCharacter()->addChild(pet,1);
        }

    }
}

void PopupStat::initButton()
{
    auto menu = Menu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerParent->addChild(menu);
    for ( int i = 0; i < 3; i++ )
    {
        auto sprite = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_2_on.png");
        sprite->setContentSize(Size(210,51));
        
        auto button = MafNode::MafMenuItemSprite::create(sprite, nullptr, CC_CALLBACK_1(PopupStat::onClickSwitchLayer, this));
        button->setPosition(_layerContainerParent->getPositionX() + ((button->getContentSize().width + 10) * ( i - 1 )),_layerContainerParent->getContentSize().height * 0.85);
        button->setTag(i);
        _vecButton.pushBack(button);
        menu->addChild(button);
        
        auto label = Label::createWithTTF(GAME_TEXT(MafUtils::format("t_ui_menu_stat_button_%d",i+1)), GAME_FONT, 24);
        label->setPosition(button->getContentSize().width/2, button->getContentSize().height/2);
        label->setDimensions(button->getContentSize().width * 0.9, label->getContentSize().height);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        label->setOverflow(Label::Overflow::SHRINK);
        label->setName("LABEL");
        button->addChild(label);
    }
    
    //
    auto itemCostume = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/main/icon_btn_costume.png"), nullptr, CC_CALLBACK_1(PopupStat::onClickButton, this));
    itemCostume->setPosition(_layerContainerParent->getContentSize().width - 60, _layerContainerParent->getContentSize().height - 50);
    itemCostume->setName("COSTUME");
    itemCostume->setScale(1.1);
    menu->addChild(itemCostume);
    {
        if ( TutorialManager::getInstance()->getOpenData(E_STEPWISE_OPEN_TYPE::ST_SPECIAL_MISSION, 38) == false )
        {
            auto itemCostumeLock = ui::Button::create("Assets/ui/main/icon_btn_costume_lock.png");
            itemCostumeLock->setPosition(Vec2(itemCostume->getContentSize().width / 2, itemCostume->getContentSize().height / 2));
            itemCostumeLock->setScale(1.1);
            itemCostumeLock->getRendererNormal()->getTexture()->setTexParameters(texParams);
            itemCostume->addChild(itemCostumeLock);
        }
        
        //
        auto uiTextName = ui::CText::create(GAME_TEXT("t_ui_costume"), GAME_FONT, 16);
        uiTextName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiTextName->setPosition(Vec2(itemCostume->getContentSize().width / 2, 20));
        uiTextName->enableOutline(Color4B::BLACK, 2);
        uiTextName->setTextAreaSize(Size(itemCostume->getContentSize().width, uiTextName->getContentSize().height));
        uiTextName->setTextOverflow(Label::Overflow::SHRINK);
        uiTextName->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTextName->setLineSpacing(-10);
        itemCostume->addChild(uiTextName);
    }
    
}

void PopupStat::drawHero()
{
    _hero->setChangeParts();
    _hero->setMotion();
    _hero->actionRun();
}

#pragma mark - click
void PopupStat::onClickClose(cocos2d::Ref *sender)
{
    hide();
}

void PopupStat::onClickButton(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_SELECT);
    
    //
    MenuItem *item = (MenuItem*)sender;
    
    //
    std::string name = item->getName();
    if ( name == "QUEST" )
    {
        
    }
    else
    {
        if ( AccountManager::getInstance()->getUserIdx() == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
        
        if ( name == "COSTUME" )
        {
            auto popup = PopupCostumeDefault::create();
            popup->show();
        }
    }
}

void PopupStat::onClickNickChange(cocos2d::Ref *sender)
{
    auto popup = PopupUserNickname::create();
    popup->setCallbackResult(CC_CALLBACK_0(PopupStat::callbackNickName, this));
    popup->show();
}

void PopupStat::onClickSwitchLayer(cocos2d::Ref* sender)
{
    int tag = ((MenuItem*)sender)->getTag();
    
    if ( tag == 0 )
    {
        _layerContainerStatNormal->setVisible(true);
        _layerContainerCostume->setVisible(false);
        _layerContainerTreasure->setVisible(false);
    }
    else if ( tag == 1 )
    {
        _layerContainerStatNormal->setVisible(false);
        _layerContainerCostume->setVisible(false);
        _layerContainerTreasure->setVisible(true);
    }
    else if ( tag == 2 )
    {
        _layerContainerStatNormal->setVisible(false);
        _layerContainerCostume->setVisible(true);
        _layerContainerTreasure->setVisible(false);
    }
    
    
    ((Label*)_spriteContainer->getChildByName("TITLE"))->setString(StringUtils::format("%s(%d/3)",GAME_TEXT("t_ui_menu_stat").c_str(),tag+1));
    
    for ( auto button : _vecButton)
    {
        if(button->getTag() == tag)
        {
            auto sprite = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_2_on.png");
            sprite->setContentSize(Size(210,51));
            button->setNormalImage(sprite);
            button->getChildByName("LABEL")->setColor(Color3B(255,255,255));
        }
        else
        {
            auto sprite = ui::Scale9Sprite::create("Assets/ui_common/btn_tap_2_off.png");
            sprite->setContentSize(Size(210,51));
            button->setNormalImage(sprite);
            button->getChildByName("LABEL")->setColor(Color3B(104,104,104));
        }
        button->reorderChild(button->getChildByName("LABEL"), 50);
    }
    
    
}

#pragma mark - schedule
void PopupStat::scheduleStatDraw(float dt)
{
    if(_ePlace == DG_NORMAL)
    {
        _statNormal->redrawLayer();
        
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_ATTACK_BONUS) > 0)
        {
            _vecBuffIcon.at(0)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(0)->setOpacity(100);
        }
        
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_COIN_BONUS) > 0)
        {
            _vecBuffIcon.at(1)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(1)->setOpacity(100);
        }
        
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_MOVE_BONUS) > 0)
        {
           _vecBuffIcon.at(2)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(2)->setOpacity(100);
        }
        
        
        if (_eSaveBuff != UserInfoManager::getInstance()->getPetBuffPriencess(_ePlace))
        {
            _eSaveBuff = UserInfoManager::getInstance()->getPetBuffPriencess(_ePlace);
            _sprPrincessBuff->stopAllActions();
            
            if (_eSaveBuff == E_PRINCESS_BUFF::BUFF_ATTACK)
            {
                _sprPrincessBuff->setVisible(true);
                _sprPrincessBuff->runAction(RepeatForever::create(_vecAnimatePrincessBuff.at(0)));
            }
            else if (_eSaveBuff == E_PRINCESS_BUFF::BUFF_MOVE)
            {
                _sprPrincessBuff->setVisible(true);
                _sprPrincessBuff->runAction(RepeatForever::create(_vecAnimatePrincessBuff.at(1)));
            }
            else if (_eSaveBuff == E_PRINCESS_BUFF::BUFF_CRITICAL)
            {
                _sprPrincessBuff->setVisible(true);
                _sprPrincessBuff->runAction(RepeatForever::create(_vecAnimatePrincessBuff.at(2)));
            }
            else if (_eSaveBuff == E_PRINCESS_BUFF::BUFF_ALL)
            {
                _sprPrincessBuff->setVisible(true);
                _sprPrincessBuff->runAction(RepeatForever::create(_vecAnimatePrincessBuff.at(3)));
            }
            else
            {
                _sprPrincessBuff->setVisible(false);
            }
        }
    }
    else if(_ePlace == DG_DEVIL_DOM)
    {
        _statDevil->redrawLayer();
        
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_D_ATK_DMG) > 0)
        {
            _vecBuffIcon.at(0)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(0)->setOpacity(100);
        }
            
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_D_QUEST_GOLD) > 0)
        {
            _vecBuffIcon.at(1)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(1)->setOpacity(100);
        }
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_D_MOVE_SPD) > 0)
        {
            _vecBuffIcon.at(2)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(2)->setOpacity(100);
        }
        if (UserInfoManager::getInstance()->getBuffTime(E_BUFF_TYPE::BUFF_D_ATK_SPD) > 0)
        {
            _vecBuffIcon.at(3)->setOpacity(255);
        }
        else
        {
            _vecBuffIcon.at(3)->setOpacity(100);
        }
    }
}


#pragma mark - callback
void PopupStat::callbackNickName()
{
    std::string strNick = AccountManager::getInstance()->getUserNick();
    strNick = "NICK : " + strNick;
    
    _labelNick->setString(strNick);
}
