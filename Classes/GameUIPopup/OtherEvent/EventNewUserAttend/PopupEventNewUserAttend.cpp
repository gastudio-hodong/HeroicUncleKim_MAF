//
//  PopupEventNewUserAttend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/03.
//

#include "PopupEventNewUserAttend.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UIReddot.h"

#include "ManagerEvent/EventNewUserAttendManager.h"
#include "ManagerGame/UserInfoManager.h"

USING_NS_CC;

PopupEventNewUserAttend* PopupEventNewUserAttend::create()
{
    PopupEventNewUserAttend* pRet = new(std::nothrow) PopupEventNewUserAttend();
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

PopupEventNewUserAttend::PopupEventNewUserAttend(void):
_isRewardCondition(0),
_nTodayReward(0),
_nEndTime(0),
_lyTopBanner(nullptr),
_lyReward(nullptr),
_lyPopupBottomUI(nullptr)
{
}

PopupEventNewUserAttend::~PopupEventNewUserAttend(void)
{
}

#pragma mark -init
bool PopupEventNewUserAttend::init()
{
    if ( !PopupBase::init())
    {
        return false;
    }
    
    _listInfoReward = EventNewUserAttendManager::getInstance()->getListInfoReward();
    _nEndTime = EventNewUserAttendManager::getInstance()->getEndTime();
    _isRewardCondition = EventNewUserAttendManager::getInstance()->isRewardCondition();
    
    initVar();
    initUI();
    
    return true;
}

void PopupEventNewUserAttend::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    EventNewUserAttendManager::getInstance()->requestInfo(CC_CALLBACK_2(PopupEventNewUserAttend::callbackInfo, this));
}

void PopupEventNewUserAttend::initVar()
{
    _spriteContainer->setTexture("Assets/ui_common/popup_mini_scale2_4.png");
    _spriteContainer->setCapInsets(Rect(27,27,6,6));
    _spriteContainer->setContentSize(Size(730,422));
}
void PopupEventNewUserAttend::initUI()
{
    //650
    Size size = _spriteContainer->getContentSize();
    
    _lyTopBanner = LayerColor::create();
    _lyTopBanner->setContentSize( Size(size.width, 172));
    _lyTopBanner->setIgnoreAnchorPointForPosition(false);
    _lyTopBanner->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyTopBanner->setPosition(_spriteContainer->getPosition() + Vec2::UP * _spriteContainer->getContentSize().height/2);
    addChild(_lyTopBanner);
    
    _lyReward = LayerColor::create();
    _lyReward->setContentSize( Size(size.width - 10, 190));
    _lyReward->setIgnoreAnchorPointForPosition(false);
    _lyReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyReward->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(_lyReward);
    
    _lyPopupBottomUI = LayerColor::create();
    _lyPopupBottomUI->setContentSize( Size(size.width - 10, _spriteContainer->getContentSize().height - _lyReward->getContentSize().height));
    _lyPopupBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyPopupBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyPopupBottomUI->setPosition( _lyReward->getPosition() + Vec2::DOWN * _lyReward->getContentSize().height);
    _spriteContainer->addChild(_lyPopupBottomUI);
    
    uiTopBanner();
    uiPopupTable();
    uiPopupBottom();
}

#pragma mark -ui
void PopupEventNewUserAttend::uiRefresh()
{
    uiTopBanner();
    uiPopupTable();
    uiPopupBottom();
}

void PopupEventNewUserAttend::uiTopBanner()
{
    _lyTopBanner->removeAllChildren();
    
    auto sprBg = Sprite::create("Assets/ui/return_event/newbie_top_bg.png");
    sprBg->getTexture()->setTexParameters(texParams);
    sprBg->setScale(3);
    sprBg->setPosition(_lyTopBanner->getContentSize()/2);
    _lyTopBanner->addChild(sprBg);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_newbie_event_title"), GAME_FONT, 32);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbTitle->setPosition( 20, _lyTopBanner->getContentSize().height - 4);
    lbTitle->setDimensions(_lyTopBanner->getContentSize().width- 30, lbTitle->getContentSize().height);
    lbTitle->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->enableBold();
    lbTitle->setColor(Color3B(255,236,76));
    _lyTopBanner->addChild(lbTitle);
    
    auto lbInfo = Label::createWithTTF(GAME_TEXT("t_ui_newbie_event_msg_1"), GAME_FONT, 24);
    lbInfo->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    lbInfo->setPosition( 20, _lyTopBanner->getContentSize().height - 65);
    lbInfo->setDimensions(340, 60);
    lbInfo->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::TOP);
    lbInfo->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyTopBanner->addChild(lbInfo);
    
    auto lbTimer = Label::createWithTTF("", GAME_FONT, 22);
    lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbTimer->setPosition( _lyTopBanner->getContentSize().width/2, 5);
    lbTimer->setDimensions(_lyTopBanner->getContentSize().width - 20, lbTimer->getContentSize().height);
    lbTimer->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
    lbTimer->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTimer->setColor(COLOR_TITLE);
    _lyTopBanner->addChild(lbTimer);
    {
        auto action1 = DelayTime::create(60);
        auto action2 = CallFuncN::create([&, lbTimer](Ref* sender){
            long time = _nEndTime - UtilsDate::getInstance()->getTime();//서버에서 주는 시간으로 변경 필요
            
            int day = (int)(time / 86400);
            int hour = time % 86400 / 3600;
            int min = time % 3600 / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            lbTimer->setString(GAME_TEXTFORMAT("t_ui_token_text_eventtime",day, hour, min));
            
        });
        auto seq = Sequence::create(action2, action1, NULL);
        lbTimer->runAction(RepeatForever::create(seq));
    }
}

void PopupEventNewUserAttend::uiPopupTable()
{
    _lyReward->removeAllChildren();
    
    auto sprTableBox = ui::Scale9Sprite::create("Assets/ui/return_event/return_reward_bg_box.png");
    sprTableBox->setContentSize(Size(692, 150));
    sprTableBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTableBox->setPosition(_lyReward->getContentSize().width/2, _lyReward->getContentSize().height - 30);
    _lyReward->addChild(sprTableBox);
    
    auto layerScroll = ui::ScrollView::create();
    layerScroll->setBounceEnabled(false);
    layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    layerScroll->setContentSize(Size(sprTableBox->getContentSize().width - 40, sprTableBox->getContentSize().height));
    layerScroll->setIgnoreAnchorPointForPosition(false);
    layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerScroll->setPosition(Vec2(sprTableBox->getContentSize()/ 2));
    layerScroll->setSwallowTouches(false);
    sprTableBox->addChild(layerScroll);
    
    double innerWidth = 0;
    double innerHeight = layerScroll->getContentSize().height;
    
    for ( int i = 0; i < _listInfoReward.size(); i++ )
    {
        auto objInfoReward = EventNewUserAttendManager::getInstance()->getEventNewAttendReward(i+1);
        if(objInfoReward == nullptr)
            continue;
        
        auto listItems = objInfoReward->getListReward();
        auto objItem = listItems.at(0);
        
        double posX = 80 * i + 15 * i;
        innerWidth = posX + 80;
        
        auto spriteItemBG = ui::Scale9Sprite::create("Assets/ui/return_event/return_reward_box.png");
        spriteItemBG->setContentSize(Size(80,80));
        spriteItemBG->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        spriteItemBG->setPosition(posX, 20);
        layerScroll->addChild(spriteItemBG);
        if(i != _listInfoReward.size() - 1)
        {
            std::string strGaugePath = "Assets/ui/return_event/return_reward_gauge.png";
            if(i + 1 < _nTodayReward)
                strGaugePath = "Assets/ui/return_event/return_reward_gauge_blue.png";
            
            auto sprGauge = ui::Scale9Sprite::create(strGaugePath);
            sprGauge->setContentSize(Size(15, sprGauge->getContentSize().height));
            sprGauge->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            sprGauge->setPosition(spriteItemBG->getPosition() + Vec2::RIGHT * spriteItemBG->getContentSize().width + Vec2::UP * spriteItemBG->getContentSize().height/2);
            layerScroll->addChild(sprGauge);
        }
        
        auto lbDay = Label::createWithTTF(MafUtils::format("Day %d", i+1), GAME_FONT, 18);
        lbDay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbDay->setPosition(spriteItemBG->getPosition() + Vec2::RIGHT * spriteItemBG->getContentSize().width/2 + Vec2::UP * (spriteItemBG->getContentSize().height + 5));
        lbDay->setDimensions(spriteItemBG->getContentSize().width - 5, lbDay->getContentSize().height);
        lbDay->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
        lbDay->setOverflow(cocos2d::Label::Overflow::SHRINK);
        layerScroll->addChild(lbDay);
        if((i+1) % 7 == 0)
            lbDay->setColor(COLOR_TITLE);
        
        auto spriteItems = Sprite::create(objItem->getPath());
        if ( spriteItems != nullptr )
        {
            spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 5);
            spriteItems->getTexture()->setTexParameters(texParams);
        
            switch((E_ITEMS)objItem->getIdx())
            {
                case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
            }
            spriteItemBG->addChild(spriteItems);
        }
        
        std::string strItemsCount = objItem->getCount();
        if ( objItem != nullptr && objItem->isString() == true )
        {
            MafUtils::convertNumberToShort(strItemsCount);
        }
        auto labelCount = Label::createWithTTF("x" + strItemsCount, GAME_FONT, 16);
        labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 5);
        labelCount->setDimensions(spriteItemBG->getContentSize().width, labelCount->getContentSize().height);
        labelCount->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::BOTTOM);
        labelCount->setOverflow(cocos2d::Label::Overflow::SHRINK);
        spriteItemBG->addChild(labelCount);
        
        if(i+1 <= _nTodayReward)
        {
            auto const spriteDisable = ui::Scale9Sprite::create("Assets/ui/return_event/return_reward_black.png");
            spriteDisable->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2);
            spriteItemBG->addChild(spriteDisable);
            
            auto sprCheck = Sprite::create("Assets/ui/return_event/return_reward_check.png");
            sprCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCheck->setPosition(spriteItemBG->getContentSize() / 2);
            spriteItemBG->addChild(sprCheck);
                
                auto sprFx = ui::Scale9Sprite::create("Assets/ui/return_event/return_reward_box_edge.png");
                sprFx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                sprFx->setPosition(spriteItemBG->getContentSize()/2);
                spriteItemBG->addChild(sprFx);
        }
        
    }
    layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
    
}

void PopupEventNewUserAttend::uiPopupBottom()
{
    _lyPopupBottomUI->removeAllChildren();
    
    auto lbInfo = Label::createWithTTF(GAME_TEXT("t_ui_return_event_msg_2"), GAME_FONT, 24);
    lbInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbInfo->setPosition(_lyPopupBottomUI->getContentSize().width / 2,
                            _lyPopupBottomUI->getContentSize().height - 10);
    lbInfo->setDimensions(_lyPopupBottomUI->getContentSize().width - 50, _lyPopupBottomUI->getContentSize().height - 90 - 30 - 25);
    lbInfo->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
    lbInfo->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyPopupBottomUI->addChild(lbInfo);
    
    auto btnClose = ui::Button::create("Assets/ui_common/popup_btn_no.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnClose->setPosition(Vec2(_lyPopupBottomUI->getContentSize().width/2, 30));
    btnClose->setZoomScale(0.05f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupEventNewUserAttend::onClickClose, this));
    btnClose->setTitleText(GAME_TEXT("t_ui_confirm"));
    btnClose->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnClose->getTitleLabel()->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
    btnClose->getTitleLabel()->enableBold();
    btnClose->setTitleFontSize(30);
    _lyPopupBottomUI->addChild(btnClose);
}
#pragma mark -click
void PopupEventNewUserAttend::onClickClose(Ref* const sender)
{
    hide();
}

#pragma mark -callback
void PopupEventNewUserAttend::callbackInfo(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1 )
    {
        std::string strMessageKey = "t_ui_error_30";
        if(nResult == 2 )
            strMessageKey = "t_ui_draw_msg_1";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    _nTodayReward = EventNewUserAttendManager::getInstance()->getToday();
    _listInfoReward = EventNewUserAttendManager::getInstance()->getListInfoReward();
    _nEndTime = EventNewUserAttendManager::getInstance()->getEndTime();
    _isRewardCondition = EventNewUserAttendManager::getInstance()->isRewardCondition();
    
    if(_isRewardCondition)
    {
        PopupLoading::show();
        EventNewUserAttendManager::getInstance()->requestReward(CC_CALLBACK_2(PopupEventNewUserAttend::callbackReward, this));
    }
    else
        uiRefresh();
}

void PopupEventNewUserAttend::callbackReward(const bool bResult, const int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1 )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    UserInfoManager::getInstance()->setGiftHave(true);
    
    _nTodayReward = EventNewUserAttendManager::getInstance()->getToday();
    _listInfoReward = EventNewUserAttendManager::getInstance()->getListInfoReward();
    _nEndTime = EventNewUserAttendManager::getInstance()->getEndTime();
    
    uiRefresh();
    _isRewardCondition = EventNewUserAttendManager::getInstance()->isRewardCondition();
}
