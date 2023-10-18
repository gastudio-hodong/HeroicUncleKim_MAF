//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupSettings.h"

#include "Common/Observer/IBattleInfo.h"

#include "GameUIPopup/Other/Settings/PopupSettingsAlarm.h"

#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

const int tag_sound_background = 1;
const int tag_sound_effect = 2;
 
PopupSettings* PopupSettings::create()
{
    PopupSettings* pRet = new(std::nothrow) PopupSettings();
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

PopupSettings::PopupSettings(void) :


_layerContainerSound(nullptr),
_layerContainerFunction(nullptr),
_layerContainerButton(nullptr)
{
    
}

PopupSettings::~PopupSettings(void)
{
    
}

bool PopupSettings::init()
{
    if( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    //
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
        cocos2d::Rect rect = _spriteContainer->getBoundingBox();
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())) == false)
        {
            hide();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
#pragma mark - init
void PopupSettings::initVar()
{
    _soundBG = SoundManager::GetSoundBgmVol();
    _soundEF = SoundManager::GetSoundEffectVol();
}

void PopupSettings::initUi()
{
    Size size = Size(632, 848);
    
    // size
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_setting"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSettings::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
     
    _layerContainerSound = LayerColor::create();
    _layerContainerSound->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 150));
    _layerContainerSound->setIgnoreAnchorPointForPosition(false);
    _layerContainerSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerSound->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerSound);
    
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 110));
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerButton->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    _spriteContainer->addChild(_layerContainerButton);
    
    _layerContainerFunction = LayerColor::create();
    _layerContainerFunction->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - _layerContainerSound->getContentSize().height - _layerContainerButton->getContentSize().height));
    _layerContainerFunction->setIgnoreAnchorPointForPosition(false);
    _layerContainerFunction->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerFunction->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerButton->getContentSize().height);
    _spriteContainer->addChild(_layerContainerFunction);
    
    //
    uiDefaultSound();
    uiDefaultFunction();
    uiDefaultButton();
}

#pragma mark - ui
void PopupSettings::uiDefaultSound()
{
    _layerContainerSound->removeAllChildren();
    
    // slider bar(배경음, 효과음)
    std::string strPathBar = "Assets/ui/sound/sound_box_1.png";
    std::string strPathBarProgress = "Assets/ui/sound/sound_box_2.png";
    std::string strPathBall = "Assets/ui/sound/sound_box_control.png";
    
    {
        double posMiddle = _layerContainerSound->getContentSize().height / 2 + 25;
        
        auto nodeText = Label::createWithTTF(GAME_TEXT("t_ui_sound_bg"), GAME_FONT, 28);
        nodeText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        nodeText->setPosition(100, posMiddle);
        nodeText->setDimensions(90, nodeText->getContentSize().height);
        nodeText->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        nodeText->setOverflow(Label::Overflow::SHRINK);
        nodeText->enableBold();
        _layerContainerSound->addChild(nodeText);
        
        auto nodeSlider = ui::Slider::create();
        nodeSlider->setAnchorPoint( Vec2(0.0f, 0.5f) );
        nodeSlider->setPosition(Vec2(100, posMiddle));
        nodeSlider->setTag(tag_sound_background);
        nodeSlider->loadBarTexture(strPathBar);
        nodeSlider->loadProgressBarTexture(strPathBarProgress);
        nodeSlider->loadSlidBallTextures(strPathBall, strPathBall, "");
        nodeSlider->setMaxPercent(100);
        nodeSlider->setPercent(_soundBG * 100);
        nodeSlider->addEventListener(CC_CALLBACK_2(PopupSettings::callbackSound, this));
        _layerContainerSound->addChild(nodeSlider);
    }
    
    {
        double posMiddle = _layerContainerSound->getContentSize().height / 2 - 25;
        
        auto nodeText = Label::createWithTTF(GAME_TEXT("t_ui_sound_effect"), GAME_FONT, 28);
        nodeText->setAnchorPoint( Vec2::ANCHOR_MIDDLE_RIGHT );
        nodeText->setPosition(100, posMiddle);
        nodeText->setDimensions(90, nodeText->getContentSize().height);
        nodeText->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
        nodeText->setOverflow(Label::Overflow::SHRINK);
        nodeText->enableBold();
        _layerContainerSound->addChild(nodeText);
        
        auto nodeSlider = ui::Slider::create();
        nodeSlider->setAnchorPoint( Vec2(0.0f, 0.5f) );
        nodeSlider->setPosition(Vec2(100, posMiddle));
        nodeSlider->setTag(tag_sound_effect);
        nodeSlider->loadBarTexture(strPathBar);
        nodeSlider->loadProgressBarTexture(strPathBarProgress);
        nodeSlider->loadSlidBallTextures(strPathBall, strPathBall, "");
        nodeSlider->setMaxPercent(100);
        nodeSlider->setPercent(_soundEF * 100);
        nodeSlider->addEventListener(CC_CALLBACK_2(PopupSettings::callbackSound, this));
        _layerContainerSound->addChild(nodeSlider);
    }
}

void PopupSettings::uiDefaultFunction()
{
    _layerContainerFunction->removeAllChildren();
        
    std::vector<E_FUNCTION_ACTIVE> listType = {
        E_FUNCTION_ACTIVE::SPEECH,
        E_FUNCTION_ACTIVE::BATTERY_AUTO,
        E_FUNCTION_ACTIVE::DAMAGE,
        E_FUNCTION_ACTIVE::ATTACK_SKIN,
        E_FUNCTION_ACTIVE::ATTACK_WEAPON,
        E_FUNCTION_ACTIVE::OFFLINE,
    };
    
    double nWidth = _layerContainerFunction->getContentSize().width;
    double nHeight = _layerContainerFunction->getContentSize().height / (int)listType.size();
    
    double posX = 0;
    double posY = _layerContainerFunction->getContentSize().height;
    for ( int i = 0; i < listType.size(); i++ )
    {
        E_FUNCTION_ACTIVE eType = listType[i];
        
        
        std::string strTitle = getFunctionText((int)eType);
        bool bCheck = UserInfoConfigManager::getInstance()->isFunction(eType);
        
        // node
        auto nodeChild = Layer::create();
        nodeChild->setContentSize(Size(nWidth, nHeight));
        nodeChild->setIgnoreAnchorPointForPosition(false);
        nodeChild->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        nodeChild->setPosition(posX, posY);
        _layerContainerFunction->addChild(nodeChild);
        
        posY -= nodeChild->getContentSize().height;
        
        // text
        auto nodeText = ui::CText::create(strTitle, GAME_FONT, 32);
        nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        nodeText->setPosition(Vec2(0, nodeChild->getContentSize().height / 2));
        nodeText->setTextAreaSize(Size(350, nodeText->getContentSize().height));
        nodeText->setTextOverflow(Label::Overflow::SHRINK);
        nodeText->enableBold();
        nodeChild->addChild(nodeText);
        
        // checkbox
        auto nodeCheck = ui::CheckBox::create("Assets/ui/mission/mission_off.png", "Assets/ui/mission/mission_on.png");
        nodeCheck->setPosition(Vec2(nodeChild->getContentSize().width - 100, nodeChild->getContentSize().height / 2));
        nodeCheck->setSelected(bCheck);
        nodeCheck->setTag((int)eType);
        nodeCheck->addEventListener(CC_CALLBACK_2(PopupSettings::callbackFunction, this));
        nodeChild->addChild(nodeCheck);
    }
    
    
}

void PopupSettings::uiDefaultButton()
{
    _layerContainerButton->removeAllChildren();
    
    
    bool bCheck = false;
    
    E_ALARM_ACTIVE listType[] = {
        E_ALARM_ACTIVE::PRISON_ENTER,
        E_ALARM_ACTIVE::JELLY,
    };
    for ( int i = 0; i < sizeof(listType) / sizeof(E_ALARM_ACTIVE); i++ )
    {
        E_ALARM_ACTIVE eType = listType[i];
        
        bCheck = UserInfoConfigManager::getInstance()->isAlarm(eType);
        if ( bCheck == true )
        {
            break;
        }
    }
    
    auto nodeSetting = ui::Button::create("Assets/ui_common/btn_1_2_on.png");
    nodeSetting->setContentSize(Size(306, 90));
    nodeSetting->setPosition(Vec2(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2));
    nodeSetting->setScale9Enabled(true);
    nodeSetting->setCapInsets(Rect(30, 32, 2, 2));
    nodeSetting->addClickEventListener(CC_CALLBACK_1(PopupSettings::onClickAlarmSetting, this));
    _layerContainerButton->addChild(nodeSetting);
    {
        std::string strPathIcon = "Assets/ui/menu/icon_push.png";
        if ( bCheck == false )
        {
            strPathIcon = "Assets/ui/menu/icon_push_off.png";
        }
        
        auto nodeContents = UtilsWidget::getInstance()->createIconText(strPathIcon, GAME_TEXT("t_ui_push_noti_all_2"), nodeSetting->getContentSize().width - 40);
        nodeContents->setPosition(Vec2(nodeSetting->getContentSize().width / 2, nodeSetting->getContentSize().height / 2));
        nodeSetting->addChild(nodeContents);
    }
}

#pragma mark - set, get, other
 

#pragma mark - click
void PopupSettings::onClickClose(Ref* sender)
{
    hide();
}

void PopupSettings::onClickAlarmSetting(Ref* sender)
{
    auto popup = PopupSettingsAlarm::create();
    popup->setCallbackResult(CC_CALLBACK_0(PopupSettings::callbackAlarm, this));
    popup->show();
}

#pragma mark - callback
void PopupSettings::callbackSound(Ref* sender, ui::Slider::EventType type)
{
    // volume
    auto slider = dynamic_cast<ui::Slider*>(sender);
    float volume = slider->getPercent() / 100.0f;
    
    if ( type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED )
    {

    }
    else if ( type == ui::Slider::EventType::ON_SLIDEBALL_UP)
    {
        if ( slider->getTag() == tag_sound_background )
        {
            SoundManager::SetSoundBgmVol(volume);
            _soundBG = volume;
        }
        else if ( slider->getTag() == tag_sound_effect )
        {
            SoundManager::SetSoundEffectVol(volume);
            _soundEF = volume;
        }
    }
}

void PopupSettings::callbackFunction(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type)
{
    auto nodeCheck = dynamic_cast<ui::CheckBox*>(sender);
    
    //
    E_FUNCTION_ACTIVE eType = (E_FUNCTION_ACTIVE)nodeCheck->getTag();
    
    //
    bool bCheck = UserInfoConfigManager::getInstance()->isFunction(eType);
    bCheck = !bCheck;
    UserInfoConfigManager::getInstance()->setFunction(eType, bCheck);
    
    //
    setFunction((int)eType, bCheck);
    
}

void PopupSettings::callbackAlarm()
{
    uiDefaultButton();
}

#pragma mark - game
std::string PopupSettings::getFunctionText(int nTag)
{
    std::string strResult = "";
    switch ((E_FUNCTION_ACTIVE)nTag) {
        case E_FUNCTION_ACTIVE::SPEECH:           strResult = "t_ui_speech_bubble";               break;
        case E_FUNCTION_ACTIVE::BATTERY_AUTO:     strResult = "t_ui_battery_auto";                break;
        case E_FUNCTION_ACTIVE::DAMAGE:           strResult = "t_ui_damage";                      break;
        case E_FUNCTION_ACTIVE::ATTACK_SKIN:      strResult = "t_ui_menu_skin_attack_effect";     break;
        case E_FUNCTION_ACTIVE::ATTACK_WEAPON:    strResult = "t_ui_menu_weapon_attack_effect";   break;
        case E_FUNCTION_ACTIVE::OFFLINE:          strResult = "t_ui_offline_reward";              break;
        default:
            break;
    }
    
    return GAME_TEXT(strResult);
}

void PopupSettings::setFunction(int nTag, bool bCheck)
{
    E_FUNCTION_ACTIVE eType = (E_FUNCTION_ACTIVE)nTag;
    if ( eType == E_FUNCTION_ACTIVE::SPEECH )
    {
        if ( bCheck == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_menu_bubble_off"), GAME_TEXT("t_ui_menu_msg_15"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        
        auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
        if ( uiBattle != nullptr )
        {
            auto hero = uiBattle->getUnitHero();
            if ( hero != nullptr )
            {
                hero->setTalkShow(bCheck);
            }
        }
    }
    else if ( eType == E_FUNCTION_ACTIVE::ATTACK_SKIN )
    {
        auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
        if ( uiBattle != nullptr )
        {
            auto hero = uiBattle->getUnitHero();
            if ( hero != nullptr )
            {
                hero->setShowSkinAttackEffect(bCheck);
            }
        }
    }
    else if ( eType == E_FUNCTION_ACTIVE::ATTACK_WEAPON )
    {
        auto uiBattle = dynamic_cast<IBattleInfo*>(UserInfoManager::getInstance()->getBattleUI());
        if ( uiBattle != nullptr )
        {
            auto hero = uiBattle->getUnitHero();
            if ( hero != nullptr )
            {
                hero->setShowWeaponAttackEffect(bCheck);
            }
        }
    }
    else if ( eType == E_FUNCTION_ACTIVE::OFFLINE )
    {
        if ( bCheck == false )
        {
            MafNative::CancelLocalNotification(E_NOTI::NOTI_OFFLINE);
        }
    }
}
