//
//  PopupCloudLoad.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupCloudLoad.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupCloudLoad* PopupCloudLoad::create()
{
    PopupCloudLoad* pRet = new(std::nothrow) PopupCloudLoad();
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

PopupCloudLoad::PopupCloudLoad(void) :
_callbackResult(nullptr),

_layerContainerTopMessage(nullptr),
_layerContainerContentsLeft(nullptr),
_layerContainerContentsRight(nullptr)
{
    
}

PopupCloudLoad::~PopupCloudLoad(void)
{
    
}

bool PopupCloudLoad::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    setHideAll(false);
    setHideBackKey(false);
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - init
void PopupCloudLoad::initVar()
{
    
}

void PopupCloudLoad::initUi()
{
    Size size = Size(658, 680);
    
    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerTopMessage = Layer::create();
    _layerContainerTopMessage->setContentSize( Size(size.width - 50, 105) );
    _layerContainerTopMessage->setIgnoreAnchorPointForPosition(false);
    _layerContainerTopMessage->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTopMessage->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25) );
    _spriteContainer->addChild(_layerContainerTopMessage);
    
    //
    _layerContainerContentsLeft = Layer::create();
    _layerContainerContentsLeft->setContentSize( Size(291, 430) );
    _layerContainerContentsLeft->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsLeft->setAnchorPoint( Vec2::ANCHOR_BOTTOM_RIGHT );
    _layerContainerContentsLeft->setPosition( Vec2(_spriteContainer->getContentSize().width / 2 - 4, 25) );
    _spriteContainer->addChild(_layerContainerContentsLeft);
    
    _layerContainerContentsRight = Layer::create();
    _layerContainerContentsRight->setContentSize( Size(291, 430) );
    _layerContainerContentsRight->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsRight->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    _layerContainerContentsRight->setPosition( Vec2(_spriteContainer->getContentSize().width / 2 + 4, 25) );
    _spriteContainer->addChild(_layerContainerContentsRight);
    
    
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    uiTopMessage();
    uiContentsLeft();
    uiContentsRight();
}

#pragma mark - ui
void PopupCloudLoad::uiTopMessage()
{
    //
    _layerContainerTopMessage->removeAllChildren();
    
    auto labelMessage1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_1"), GAME_FONT, 24);
    labelMessage1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMessage1->setPosition(_layerContainerTopMessage->getContentSize().width / 2, _layerContainerTopMessage->getContentSize().height);
    labelMessage1->setDimensions(_layerContainerTopMessage->getContentSize().width, labelMessage1->getContentSize().height);
    labelMessage1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMessage1->setOverflow(Label::Overflow::SHRINK);
    _layerContainerTopMessage->addChild(labelMessage1);
    
    auto labelMessage2 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_9"), GAME_FONT, 20);
    labelMessage2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelMessage2->setPosition(_layerContainerTopMessage->getContentSize().width / 2, 0);
    labelMessage2->setDimensions(_layerContainerTopMessage->getContentSize().width, labelMessage2->getContentSize().height);
    labelMessage2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMessage2->setOverflow(Label::Overflow::SHRINK);
    labelMessage2->setColor(Color3B(130, 130, 130));
    _layerContainerTopMessage->addChild(labelMessage2);
}

void PopupCloudLoad::uiContentsLeft()
{
    //
    _layerContainerContentsLeft->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/transfer/login_data_bg2.png");
    spriteBG->setPosition(_layerContainerContentsLeft->getContentSize().width / 2, _layerContainerContentsLeft->getContentSize().height / 2);
    _layerContainerContentsLeft->addChild(spriteBG);
    
    auto spriteTitleIcon = Sprite::create("Assets/ui/transfer/login_data_icon2.png");
    spriteTitleIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteTitleIcon->setPosition(50, spriteBG->getContentSize().height - 29);
    spriteBG->addChild(spriteTitleIcon);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_3"), GAME_FONT, 32);
    labelTitle->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 29);
    labelTitle->setDimensions(191, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    labelTitle->setColor(Color3B(255, 53, 88));
    labelTitle->enableOutline(Color4B::BLACK, 2);
    spriteBG->addChild(labelTitle);
    
    auto labelMsg1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_8"), GAME_FONT, 24);
    labelMsg1->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 105);
    labelMsg1->setDimensions(spriteBG->getContentSize().width * 0.9, labelMsg1->getContentSize().height);
    labelMsg1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg1->setOverflow(Label::Overflow::SHRINK);
    labelMsg1->setColor(Color3B(255, 53, 88));
    labelMsg1->enableOutline(Color4B::BLACK, 2);
    spriteBG->addChild(labelMsg1);
    
    std::string strSaveDate = UtilsDate::getInstance()->getTimeString(SaveManager::getInstance()->getSaveTime(), UtilsDate::eType::date_real_format, GAME_TEXT("t_ui_checkdata_msg_6"));
    auto labelMsg2 = Label::createWithTTF(strSaveDate, GAME_FONT, 20);
    labelMsg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    labelMsg2->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 170);
    labelMsg2->setDimensions(spriteBG->getContentSize().width * 0.9, labelMsg2->getContentSize().height);
    labelMsg2->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg2->setOverflow(Label::Overflow::SHRINK);
    labelMsg2->setColor(Color3B(203, 203, 203));
    spriteBG->addChild(labelMsg2);
    
    
    {
        int nFloorHigh = SaveManager::getInstance()->getSaveFloorHigh();
        int nFloorTotal = SaveManager::getInstance()->getSaveFloorTotal();
        
        auto labelFoolHigh1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_4"), GAME_FONT, 20);
        labelFoolHigh1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelFoolHigh1->setPosition(20, 135);
        labelFoolHigh1->setColor(Color3B(203, 203, 203));
        spriteBG->addChild(labelFoolHigh1);
        
        auto labelFoolHigh2 = Label::createWithTTF(MafUtils::toString(nFloorHigh), GAME_FONT, 20);
        labelFoolHigh2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        labelFoolHigh2->setPosition(spriteBG->getContentSize().width - 20, 135);
        labelFoolHigh2->setColor(COLOR_COIN);
        spriteBG->addChild(labelFoolHigh2);
        
        auto labelFoolTotal1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_5"), GAME_FONT, 20);
        labelFoolTotal1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelFoolTotal1->setPosition(20, 135);
        labelFoolTotal1->setColor(Color3B(203, 203, 203));
        spriteBG->addChild(labelFoolTotal1);
        
        auto labelFoolTotal2 = Label::createWithTTF(MafUtils::toString(nFloorTotal), GAME_FONT, 20);
        labelFoolTotal2->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        labelFoolTotal2->setPosition(spriteBG->getContentSize().width - 20, 135);
        labelFoolTotal2->setColor(COLOR_COIN);
        spriteBG->addChild(labelFoolTotal2);
    }
    
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteBG->addChild(menu);
        
        auto spriteBtn = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteBtn->setContentSize(Size(275, 80));
        
        auto itemBtn = MafNode::MafMenuItemSprite::create(spriteBtn, nullptr, CC_CALLBACK_1(PopupCloudLoad::onClickLoad, this));
        itemBtn->setPosition(spriteBG->getContentSize().width / 2, itemBtn->getContentSize().height / 2 + 10);
        menu->addChild(itemBtn);
        
        auto labelBtn = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_btn_2"), GAME_FONT, 28);
        labelBtn->setPosition(itemBtn->getContentSize().width / 2, itemBtn->getContentSize().height / 2);
        itemBtn->addChild(labelBtn);
    }
}

void PopupCloudLoad::uiContentsRight()
{
    //
    _layerContainerContentsRight->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/transfer/login_data_bg1.png");
    spriteBG->setPosition(_layerContainerContentsRight->getContentSize().width / 2, _layerContainerContentsRight->getContentSize().height / 2);
    _layerContainerContentsRight->addChild(spriteBG);
    
    auto spriteTitleIcon = Sprite::create("Assets/ui/transfer/login_data_icon1.png");
    spriteTitleIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    spriteTitleIcon->setPosition(50, spriteBG->getContentSize().height - 29);
    spriteBG->addChild(spriteTitleIcon);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_2"), GAME_FONT, 32);
    labelTitle->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 29);
    labelTitle->setDimensions(191, labelTitle->getContentSize().height);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setOverflow(Label::Overflow::SHRINK);
    labelTitle->setColor(Color3B(65, 210, 255));
    labelTitle->enableOutline(Color4B::BLACK, 2);
    spriteBG->addChild(labelTitle);
    
    auto labelMsg1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_7"), GAME_FONT, 24);
    labelMsg1->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 105);
    labelMsg1->setDimensions(spriteBG->getContentSize().width * 0.9, labelMsg1->getContentSize().height);
    labelMsg1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelMsg1->setOverflow(Label::Overflow::SHRINK);
    labelMsg1->setColor(Color3B(65, 210, 255));
    labelMsg1->enableOutline(Color4B::BLACK, 2);
    spriteBG->addChild(labelMsg1);
    
    
    {
        int nFloorHigh = UserInfoManager::getInstance()->getHighFloor(E_PLACE::DG_NORMAL);
        int nFloorTotal = UserInfoManager::getInstance()->getTotalFloor();
        
        auto labelFoolHigh1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_4"), GAME_FONT, 20);
        labelFoolHigh1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        labelFoolHigh1->setPosition(20, 135);
        labelFoolHigh1->setColor(Color3B(203, 203, 203));
        spriteBG->addChild(labelFoolHigh1);
        
        auto labelFoolHigh2 = Label::createWithTTF(MafUtils::toString(nFloorHigh), GAME_FONT, 20);
        labelFoolHigh2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        labelFoolHigh2->setPosition(spriteBG->getContentSize().width - 20, 135);
        labelFoolHigh2->setColor(COLOR_COIN);
        spriteBG->addChild(labelFoolHigh2);
        
        auto labelFoolTotal1 = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_msg_5"), GAME_FONT, 20);
        labelFoolTotal1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelFoolTotal1->setPosition(20, 135);
        labelFoolTotal1->setColor(Color3B(203, 203, 203));
        spriteBG->addChild(labelFoolTotal1);
        
        auto labelFoolTotal2 = Label::createWithTTF(MafUtils::toString(nFloorTotal), GAME_FONT, 20);
        labelFoolTotal2->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        labelFoolTotal2->setPosition(spriteBG->getContentSize().width - 20, 135);
        labelFoolTotal2->setColor(COLOR_COIN);
        spriteBG->addChild(labelFoolTotal2);
    }
    
    {
        // menu
        auto menu = MafNode::MafMenu::create();
        menu->setAnchorPoint(Vec2::ZERO);
        menu->setPosition(Vec2::ZERO);
        spriteBG->addChild(menu);
        
        auto spriteBtn = ui::Scale9Sprite::create("Assets/ui_common/btn_1_2_on.png", Rect::ZERO, Rect(33,33,2,2));
        spriteBtn->setContentSize(Size(275, 80));
        
        auto itemBtn = MafNode::MafMenuItemSprite::create(spriteBtn, nullptr, CC_CALLBACK_1(PopupCloudLoad::onClickNow, this));
        itemBtn->setPosition(spriteBG->getContentSize().width / 2, itemBtn->getContentSize().height / 2 + 10);
        menu->addChild(itemBtn);
        
        auto labelBtn = Label::createWithTTF(GAME_TEXT("t_ui_checkdata_btn_1"), GAME_FONT, 28);
        labelBtn->setPosition(itemBtn->getContentSize().width / 2, itemBtn->getContentSize().height / 2);
        itemBtn->addChild(labelBtn);
    }
}


#pragma mark - set, get, sort, add
void PopupCloudLoad::setCallbackResult(const std::function<void(void)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupCloudLoad::onClickClose(Ref* sender)
{
    hide();
}
 
void PopupCloudLoad::onClickLoad(cocos2d::Ref* sender)
{
    SaveManager::getInstance()->loadCloudData();
    
    // 불러오기를 할때 서버에 로그를 남김.
    SaveManager::getInstance()->requestLoadFinish();
}

void PopupCloudLoad::onClickNow(cocos2d::Ref* sender)
{
    if ( _callbackResult != nullptr )
    {
        _callbackResult();
    }
    
    hide();
}
