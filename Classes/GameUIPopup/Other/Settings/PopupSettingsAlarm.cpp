//
//  lkjkl.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 17/02/2020.
//

#include "PopupSettingsAlarm.h"

#include "Common/Observer/IBattleInfo.h"

#include "ManagerGame/UserInfoConfigManager.h"
#include "ManagerGame/DataManager.h"

USING_NS_CC;

PopupSettingsAlarm* PopupSettingsAlarm::create()
{
    PopupSettingsAlarm* pRet = new(std::nothrow) PopupSettingsAlarm();
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

PopupSettingsAlarm::PopupSettingsAlarm(void) :
_callbackResult(nullptr),

_layerContainerAll(nullptr),
_layerContainerAlarm(nullptr)
{
    
}

PopupSettingsAlarm::~PopupSettingsAlarm(void)
{
    
}

bool PopupSettingsAlarm::init()
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
            if ( _callbackResult != nullptr )
            {
                _callbackResult();
            }
            
            hide();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
#pragma mark - init
void PopupSettingsAlarm::initVar()
{
    
}

void PopupSettingsAlarm::initUi()
{
    Size size = Size(632, 848);
    
    // size
    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_push_noti_all_2"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupSettingsAlarm::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
     
    _layerContainerAll = LayerColor::create();
    _layerContainerAll->setContentSize(Size(_spriteContainer->getContentSize().width - 50, 150));
    _layerContainerAll->setIgnoreAnchorPointForPosition(false);
    _layerContainerAll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _layerContainerAll->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_layerContainerAll);
    
    _layerContainerAlarm = LayerColor::create();
    _layerContainerAlarm->setContentSize(Size(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 75 - _layerContainerAll->getContentSize().height - 25));
    _layerContainerAlarm->setIgnoreAnchorPointForPosition(false);
    _layerContainerAlarm->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _layerContainerAlarm->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerAlarm);
    
    //
    uiDefaultAll();
    uiDefaultAlarm();
}

#pragma mark - ui
void PopupSettingsAlarm::uiDefaultAll()
{
    _layerContainerAll->removeAllChildren();
    
    bool bCheck = UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::ALL);
    
    auto nodeLine = ui::ImageView::create("Assets/ui_common/base_line_1_1.png");
    nodeLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    nodeLine->setPosition(Vec2(_layerContainerAll->getContentSize().width / 2, 0));
    _layerContainerAll->addChild(nodeLine);
    
    // text
    auto nodeText = ui::CText::create(getAlarmText((int)E_ALARM_ACTIVE::ALL), GAME_FONT, 32);
    nodeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    nodeText->setPosition(Vec2(0, _layerContainerAll->getContentSize().height / 2));
    nodeText->setTextAreaSize(Size(350, nodeText->getContentSize().height));
    nodeText->setTextOverflow(Label::Overflow::SHRINK);nodeText->setTextColor(Color4B::YELLOW);
    nodeText->enableBold();
    _layerContainerAll->addChild(nodeText);
    
    // checkbox
    auto nodeCheck = ui::CheckBox::create("Assets/ui/mission/mission_off.png", "Assets/ui/mission/mission_on.png");
    nodeCheck->setPosition(Vec2(_layerContainerAll->getContentSize().width - 100, _layerContainerAll->getContentSize().height / 2));
    nodeCheck->setSelected(bCheck);
    nodeCheck->addEventListener(CC_CALLBACK_2(PopupSettingsAlarm::callbackFunctionAll, this));
    nodeCheck->setTag((int)E_ALARM_ACTIVE::ALL);
    _layerContainerAll->addChild(nodeCheck);
}

void PopupSettingsAlarm::uiDefaultAlarm()
{
    _layerContainerAlarm->removeAllChildren();
        
    std::vector<E_ALARM_ACTIVE> listType = {
        E_ALARM_ACTIVE::PRISON_ENTER,
        E_ALARM_ACTIVE::JELLY,
    };
    
    double nWidth = _layerContainerAlarm->getContentSize().width;
    double nHeight = _layerContainerAlarm->getContentSize().height / listType.size();
    
    double posX = 0;
    double posY = _layerContainerAlarm->getContentSize().height;
    
    bool bCheckAll = UserInfoConfigManager::getInstance()->isAlarm(E_ALARM_ACTIVE::ALL);
    for ( int i = 0; i < listType.size(); i++ )
    {
        E_ALARM_ACTIVE eType = listType[i];
        
        
        std::string strTitle = getAlarmText((int)eType);
        bool bCheck = UserInfoConfigManager::getInstance()->isAlarmOriginal(eType);
        
        // node
        auto nodeChild = Layer::create();
        nodeChild->setContentSize(Size(nWidth, nHeight));
        nodeChild->setIgnoreAnchorPointForPosition(false);
        nodeChild->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        nodeChild->setPosition(posX, posY);
        _layerContainerAlarm->addChild(nodeChild);
        
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
        std::string strPathOff = "Assets/ui/mission/mission_off.png";
        std::string strPathOn = "Assets/ui/mission/mission_on.png";
        if ( bCheckAll == false )
        {
            strPathOn = "Assets/ui/mission/mission_on_2.png";
        }
        
        auto nodeCheck = ui::CheckBox::create(strPathOff, strPathOn);
        nodeCheck->setPosition(Vec2(nodeChild->getContentSize().width - 100, nodeChild->getContentSize().height / 2));
        nodeCheck->setSelected(bCheck);
        nodeCheck->setTag((int)eType);
        nodeCheck->setZoomScale(0.05f);
        nodeCheck->addEventListener(CC_CALLBACK_2(PopupSettingsAlarm::callbackFunction, this));
        nodeCheck->setTouchEnabled(bCheckAll);
        nodeChild->addChild(nodeCheck);
    }
}

#pragma mark - click
void PopupSettingsAlarm::onClickClose(Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
    }
    
    hide();
}

#pragma mark - callback
void PopupSettingsAlarm::callbackFunctionAll(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type)
{
    auto nodeCheck = dynamic_cast<ui::CheckBox*>(sender);
    
    //
    E_ALARM_ACTIVE eType = (E_ALARM_ACTIVE)nodeCheck->getTag();
    
    //
    bool bCheck = UserInfoConfigManager::getInstance()->isAlarm(eType);
    bCheck = !bCheck;
    UserInfoConfigManager::getInstance()->setAlarm(eType, bCheck);
    
    //
    std::vector<E_ALARM_ACTIVE> listType = {
        E_ALARM_ACTIVE::PRISON_ENTER,
        E_ALARM_ACTIVE::JELLY,
    };
    
    for ( int i = 0; i < listType.size(); i++ )
    {
        E_ALARM_ACTIVE eType = listType[i];
        
        setAlarm((int)eType, false);
    }
    
    //
    uiDefaultAlarm();
}

void PopupSettingsAlarm::callbackFunction(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType type)
{
    auto nodeCheck = dynamic_cast<ui::CheckBox*>(sender);
    
    //
    E_ALARM_ACTIVE eType = (E_ALARM_ACTIVE)nodeCheck->getTag();
    
    //
    bool bCheck = UserInfoConfigManager::getInstance()->isAlarm(eType);
    bCheck = !bCheck;
    UserInfoConfigManager::getInstance()->setAlarm(eType, bCheck);
    
   
    //
    setAlarm((int)eType, bCheck);
}

#pragma mark - game
void PopupSettingsAlarm::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

std::string PopupSettingsAlarm::getAlarmText(int nTag)
{
    std::string strResult = "";
    switch ((E_ALARM_ACTIVE)nTag) {
        case E_ALARM_ACTIVE::ALL:               strResult = "t_ui_push_noti_all_1";               break;
        case E_ALARM_ACTIVE::PRISON_ENTER:      strResult = "t_ui_push_noti_prison_enter";                      break;
        case E_ALARM_ACTIVE::JELLY:             strResult = "t_ui_push_noti_prison_jelly";     break;
        default:
            break;
    }
    
    return GAME_TEXT(strResult);
}

void PopupSettingsAlarm::setAlarm(int nTag, bool bCheck)
{
    std::vector<std::string> listPush;
    
    
    E_ALARM_ACTIVE eType = (E_ALARM_ACTIVE)nTag;
    if ( eType == E_ALARM_ACTIVE::PRISON_ENTER )
    {
        listPush.push_back("prison");
        
        if ( bCheck == false )
        {
            MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_ENTER);
        }
    }
    if ( eType == E_ALARM_ACTIVE::JELLY )
    {
        listPush.push_back("jelly");
        
        if ( bCheck == false )
        {
            MafNative::CancelLocalNotification(E_NOTI::NOTI_PRISON_JELLY);
        }
    }
        
    //
    DataManager::getInstance()->requestPushInfo(listPush);
}
