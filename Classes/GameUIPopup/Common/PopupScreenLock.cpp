//
//  PopupScreenLock.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 6. 4..
//

#include "PopupScreenLock.h"

#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

PopupScreenLock::PopupScreenLock(void) :
_posTouch(Vec2::ZERO),

_nAnimationType(0),

_layerContainer(nullptr),

_labelTime(nullptr),
_labelTimeFormat(nullptr),
_labelFloor(nullptr),
_spriteBonsik(nullptr),

_animateBonsikAttack(nullptr),
_animateBonsikWalk(nullptr)
{
    
}

PopupScreenLock::~PopupScreenLock(void)
{
    //
    if ( _animateBonsikAttack != nullptr )
    {
        _animateBonsikAttack->release();
        _animateBonsikAttack = nullptr;
    }
    
    if ( _animateBonsikWalk != nullptr )
    {
        _animateBonsikWalk->release();
        _animateBonsikWalk = nullptr;
    }
}

bool PopupScreenLock::init(void)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    setContentSize(UtilsRect::getVisibleSize());
    setTouchEnabled(true);
    
    //
    uiContents();
    
    return true;
}

#pragma mark - touch
bool PopupScreenLock::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size s = this->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if ( rect.containsPoint(locationInNode) )
    {
        // 현재 위치
        _posTouch = touch->getLocation();
        
        // 컨테이너 액션 정지
        _layerContainer->stopAllActions();
        
        return true;
    }
    return false;
}

void PopupScreenLock::onTouchMoved(Touch* touch, Event* event)
{
    float distanceOriginal = MIN(255, MAX(0, touch->getLocation().getDistance(_posTouch)));
    if ( distanceOriginal <= 0 )
    {
        return;
    }
    
    float distanceScale = distanceOriginal / 100.0f + 1;
    distanceScale = MIN(2.5f, MAX(1, distanceScale));
    _layerContainer->setScale(distanceScale);
    
    float distanceOpacity = 255 - distanceOriginal;
    _layerContainer->setOpacity(distanceOpacity);
}

void PopupScreenLock::onTouchEnded(Touch* touch, Event* event)
{
    float distance = touch->getLocation().getDistance(_posTouch);
    if ( distance >= 250 )
    {
        hide();
    }
    else
    {
        onTouchCancelled(touch, event);
    }
}

void PopupScreenLock::onTouchCancelled(Touch* touch, Event* event)
{
    _layerContainer->stopAllActions();
    _layerContainer->setScale(1.0f);
    _layerContainer->setOpacity(255.0f);
    
    
}

#pragma mark - init
void PopupScreenLock::uiContents()
{
    // container
    _layerContainer = LayerColor::create(Color4B(0, 0, 0, 255));
    _layerContainer->setIgnoreAnchorPointForPosition(false);
    _layerContainer->setContentSize( Size(getContentSize().width, getContentSize().height) );
    _layerContainer->setAnchorPoint( Vec2(0.5f, 0.5f) );
    _layerContainer->setPosition( Vec2(getContentSize().width / 2, getContentSize().height / 2) );
    _layerContainer->setCascadeOpacityEnabled(true);
    addChild(_layerContainer);
    
    // time
    _labelTime = Label::createWithTTF("00:00", GAME_FONT_AL, 200);
    _labelTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _labelTime->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2 + 300);
    _labelTime->setColor(Color3B(153, 153, 153));
    _layerContainer->addChild(_labelTime);
    {
        _labelTimeFormat = Label::createWithTTF("", GAME_FONT_AL, 80);
        _labelTimeFormat->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _labelTimeFormat->setPosition(0, _labelTime->getContentSize().height - 40);
        _labelTimeFormat->setColor(Color3B(153, 153, 153));
        _labelTime->addChild(_labelTimeFormat);
    }
    
    
    // floor
    _labelFloor = Label::createWithTTF(" ", GAME_FONT, 50);
    _labelFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _labelFloor->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2 + 300);
    _labelFloor->setColor(Color3B(128, 128, 128));
    _labelFloor->setDimensions(_layerContainer->getContentSize().width * 0.9, _labelFloor->getContentSize().height * 2);
    _labelFloor->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelFloor->setOverflow(Label::Overflow::SHRINK);
    _layerContainer->addChild(_labelFloor);
    
    
    // animation
    auto animationBonsikAttack = Animation::create();
    animationBonsikAttack->setDelayPerUnit(0.15f);
    for (int i = 1; i <= 4; i++)
    {
        animationBonsikAttack->addSpriteFrameWithFile(MafUtils::format("Assets/ui/lock/screensaver_attack_%d.png",i));
    }
    _animateBonsikAttack = Animate::create(animationBonsikAttack);
    _animateBonsikAttack->retain();
    
    auto animationBonsikWalk = Animation::create();
    animationBonsikWalk->setDelayPerUnit(0.15f);
    for (int i = 1; i <= 4; i++)
    {
        animationBonsikWalk->addSpriteFrameWithFile(MafUtils::format("Assets/ui/lock/screensaver_walk_%d.png",i));
    }
    _animateBonsikWalk = Animate::create(animationBonsikWalk);
    _animateBonsikWalk->retain();
    
    _spriteBonsik = Sprite::create("Assets/ui/lock/screensaver_attack_1.png");
    _spriteBonsik->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteBonsik->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2 - 300);
    _layerContainer->addChild(_spriteBonsik);
    
    
    // arrow icon text
    auto labelArrow = Label::createWithTTF(GAME_TEXT("t_ui_lock_msg_3"), GAME_FONT, 34);
    labelArrow->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2 - 400);
    labelArrow->setColor(Color3B(128, 128, 128));
    labelArrow->setDimensions(400, labelArrow->getContentSize().height);
    labelArrow->setOverflow(Label::Overflow::SHRINK);
    labelArrow->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _layerContainer->addChild(labelArrow);
    {
        // arrow icon
        auto spriteArrow = Sprite::create("Assets/ui/lock/screensaver_arrow_1.png");
        spriteArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        spriteArrow->setPosition(-20, labelArrow->getContentSize().height / 2);
        labelArrow->addChild(spriteArrow);
    }
    
    // info text
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_lock_msg_4"), GAME_FONT, 28);
    labelInfo->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2 - 550);
    labelInfo->setDimensions(_layerContainer->getContentSize().width * 0.9, labelInfo->getContentSize().height);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    labelInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelInfo->setColor(Color3B(128, 128, 128));
    _layerContainer->addChild(labelInfo);
    
    //
    uiDrawTime();
    uiDrawFloor();
    uiDrawAnimation();
    schedule(schedule_selector(PopupScreenLock::uiDrawTime), 1.0f, kRepeatForever, 0.0f);
    schedule(schedule_selector(PopupScreenLock::uiDrawFloor));
    schedule(schedule_selector(PopupScreenLock::uiDrawAnimation), 1.0f, kRepeatForever, 0.0f);
}

#pragma mark - set, get
void PopupScreenLock::uiDrawTime(float dt)
{
    std::string strTime = "";
    std::string strTimeFormat = "";
    {
        int32_t nLength = 1024 * 100;
        
        time_t t = UtilsDate::getInstance()->getTime();
        struct tm *info = localtime(&t);
        char* buf1 = (char*)malloc(nLength);
        char* buf2 = (char*)malloc(nLength);
        
        
        strftime (buf1, nLength, "%I:%M", info);
        strftime (buf2, nLength, "%p", info);
        strTime = buf1;
        strTimeFormat = buf2;
        free(buf1);
        free(buf2);
    }
    
    _labelTime->setString(strTime);
    _labelTimeFormat->setString(strTimeFormat);
    
}

void PopupScreenLock::uiDrawFloor(float dt)
{
    std::string strFloor = "";
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene->getTag() == UtilsScene::scene_dungeon_devil )
    {
        std::string strTemp = MafUtils::toString(UserInfoManager::getInstance()->getFloor(E_PLACE::DG_DEVIL_DOM));
        MafUtils::addCommaToString(strTemp, 3, ',');
        
        strFloor = MafUtils::format("%sm", strTemp.c_str());
        strFloor.append(StringUtils::format(" %d/%d", UserInfoManager::getInstance()->getFloorCount(E_PLACE::DG_DEVIL_DOM), C_MAX_FLOOR_COUNT));
    }
    else
    {
        strFloor = GAME_TEXTFORMAT("t_ui_town_floor", UserInfoManager::getInstance()->getFloor(E_PLACE::DG_NORMAL));
        strFloor.append(StringUtils::format(" %d/%d", UserInfoManager::getInstance()->getFloorCount(E_PLACE::DG_NORMAL), C_MAX_FLOOR_COUNT));
    }
    _labelFloor->setString(strFloor);
    
}

void PopupScreenLock::uiDrawAnimation(float dt)
{
    int type = 0;
    
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene->getTag() == UtilsScene::scene_dungeon_devil )
    {
        int hitCount = UserInfoManager::getInstance()->getHitCount(E_PLACE::DG_DEVIL_DOM);
        if ( hitCount >= 200 )
        {
            type = 1;
        }
        else
        {
            type = 2;
        }
    }
    else
    {
        int hitCount = UserInfoManager::getInstance()->getHitCount(E_PLACE::DG_NORMAL);
        if ( hitCount >= 200 )
        {
            type = 1;
        }
        else
        {
            type = 2;
        }
    }
    
    if ( _nAnimationType != type )
    {
        _nAnimationType = type;
        _spriteBonsik->stopAllActions();
        
        if ( _nAnimationType == 1 )
        {
            _spriteBonsik->runAction(RepeatForever::create(_animateBonsikAttack->clone()));
        }
        else
        {
            _spriteBonsik->runAction(RepeatForever::create(_animateBonsikWalk->clone()));
        }
    }
}

#pragma mark - set, get

#pragma mark - show,  hide
bool PopupScreenLock::isShow()
{
    bool ret = false;
    
    // notification remove
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOCK) != nullptr )
    {
        ret = true;
    }
    
    return ret;
}

void PopupScreenLock::show()
{
    // notification
    auto node = Director::getInstance()->getNotificationNode();
    if ( node == nullptr )
    {
        return;
    }
    
    if ( node->getChildByTag(TAG_NOTI_LOCK) != nullptr )
    {
        return;
    }
    
    // notification add
    auto popup = PopupScreenLock::create();
    node->addChild(popup, ZORDER_NOTI_LOCK, TAG_NOTI_LOCK);
}

void PopupScreenLock::hide()
{
    auto node = Director::getInstance()->getNotificationNode();
    if ( node != nullptr && node->getChildByTag(TAG_NOTI_LOCK) != nullptr )
    {
        // event
        node->removeChildByTag(TAG_NOTI_LOCK);
    }
}
