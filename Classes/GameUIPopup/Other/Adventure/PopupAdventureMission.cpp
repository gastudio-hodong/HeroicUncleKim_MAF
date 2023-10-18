//
//  PopupAdventureMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/07.
//

#include "PopupAdventureMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Adventure/Layer/UILayerAdventureMission.h"

#include "ManagerGame/AdventureManager.h"

USING_NS_CC;

PopupAdventureMission* PopupAdventureMission::create()
{
    PopupAdventureMission* pRet = new(std::nothrow) PopupAdventureMission();
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

PopupAdventureMission::PopupAdventureMission(void) :
_lyTopUI(nullptr)
,_lyDailyMission(nullptr)
,_lyWeeklyMission(nullptr)
,_lyBottomUI(nullptr)
{
    
}

PopupAdventureMission::~PopupAdventureMission(void)
{
    AdventureManager::getInstance()->unSubMissionEvent(this);
}

bool PopupAdventureMission::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    // init
    initVar();
    initUi();
    
    AdventureManager::getInstance()->subMissionEvent(new MafFunction<void()>(CC_CALLBACK_0(PopupAdventureMission::refreshUI, this), this));
    return true;
}

#pragma mark - init
void PopupAdventureMission::initVar()
{

}

void PopupAdventureMission::initUi()
{
    Size size = Size(730, 1080);
    
    //
    _spriteContainer->setContentSize( size );
    
    _lyTopUI = LayerColor::create();
    _lyTopUI->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 180));
    _lyTopUI->setIgnoreAnchorPointForPosition(false);
    _lyTopUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyTopUI->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _spriteContainer->addChild(_lyTopUI);
    
    _lyDailyMission = LayerColor::create();
    _lyDailyMission->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 500));
    _lyDailyMission->setIgnoreAnchorPointForPosition(false);
    _lyDailyMission->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyDailyMission->setPosition(_lyTopUI->getPosition() + Vec2::DOWN * (_lyTopUI->getContentSize().height + 5));
    _spriteContainer->addChild(_lyDailyMission);
    
    _lyWeeklyMission = LayerColor::create();
    _lyWeeklyMission->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 175));
    _lyWeeklyMission->setIgnoreAnchorPointForPosition(false);
    _lyWeeklyMission->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyWeeklyMission->setPosition(_lyDailyMission->getPosition() + Vec2::DOWN * (_lyDailyMission->getContentSize().height + 5));
    _spriteContainer->addChild(_lyWeeklyMission);
    
    _lyBottomUI = LayerColor::create();
    _lyBottomUI->setContentSize(Size(_spriteContainer->getContentSize().width - 10, 130));
    _lyBottomUI->setIgnoreAnchorPointForPosition(false);
    _lyBottomUI->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottomUI->setPosition(_lyWeeklyMission->getPosition() + Vec2::DOWN * _lyWeeklyMission->getContentSize().height);
    _spriteContainer->addChild(_lyBottomUI);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_advt_mission_title_1"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAdventureMission::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    uiTop();
    uiDailyMission();
    uiWeeklyMission();
    uiBottom();
}

#pragma mark - ui
void PopupAdventureMission::uiTop()
{
    _lyTopUI->removeAllChildren();
    
    auto sprBG = Sprite::create("Assets/ui/adventure/adventure_mission_top_img.png");
    sprBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBG->setPosition(Vec2(_lyTopUI->getContentSize()/2) - Vec2(0, 5));
    sprBG->setScale(4);
    sprBG->getTexture()->setTexParameters(texParams);
    _lyTopUI->addChild(sprBG);
    
    //480, 55
    
    auto lbInfo = Label::createWithTTF(GAME_TEXT("t_ui_advt_mission_info_1"), GAME_FONT, 22);
    lbInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbInfo->setPosition(30, 50);
    lbInfo->setDimensions(480, 55);
    lbInfo->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbInfo->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    _lyTopUI->addChild(lbInfo);
    
}
void PopupAdventureMission::uiDailyMission()
{
    _lyDailyMission->removeAllChildren();
    
    auto uiMission = UILayerAdventureMission::create(_lyDailyMission->getContentSize(), E_ADVENTURE_MISSION_GROUP::DAILY);
    _lyDailyMission->addChild(uiMission);
}

void PopupAdventureMission::uiWeeklyMission()
{
    _lyWeeklyMission->removeAllChildren();
    
    auto uiMission = UILayerAdventureMission::create(_lyWeeklyMission->getContentSize(), E_ADVENTURE_MISSION_GROUP::WEEKLY);
    _lyWeeklyMission->addChild(uiMission);
}

void PopupAdventureMission::uiBottom()
{
    _lyBottomUI->removeAllChildren();
    
    std::string strBtnPath = "Assets/ui_common/btn_1_9_on.png";
    if(!AdventureManager::getInstance()->isMissionComplete())
        strBtnPath = "Assets/ui_common/btn_1_off.png";
    
    auto btnAllComplete = ui::Button::create(strBtnPath);
    btnAllComplete->setScale9Enabled(true);
    btnAllComplete->setContentSize(Size(698, 88));
    btnAllComplete->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnAllComplete->setPosition(_lyBottomUI->getContentSize()/2);
    btnAllComplete->addClickEventListener(CC_CALLBACK_1(PopupAdventureMission::onClickAllComplete, this));
    btnAllComplete->setTitleText(GAME_TEXT("t_ui_attend_receive_all"));
    btnAllComplete->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnAllComplete->setTitleFontSize(30);
    btnAllComplete->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    btnAllComplete->getTitleLabel()->setDimensions(btnAllComplete->getContentSize().width - 10, btnAllComplete->getContentSize().height - 10);
    btnAllComplete->setZoomScale(0.02f);
    _lyBottomUI->addChild(btnAllComplete);
}

void PopupAdventureMission::refreshUI()
{
    uiBottom();
}
#pragma mark - click
void PopupAdventureMission::onClickClose(Ref* sender)
{
    hide();
}

void PopupAdventureMission::onClickAllComplete(Ref* const sender)
{
    if(!AdventureManager::getInstance()->isMissionComplete())
        return;
    
    PopupLoading::show();
    AdventureManager::getInstance()->requestMissionAllComplete(CC_CALLBACK_2(PopupAdventureMission::callbackMissionAllComplete, this));
}

#pragma mark callback
void PopupAdventureMission::callbackMissionAllComplete(bool bResult, int nResult)
{
    PopupLoading::hide();
    
    if(nResult != 1 && nResult != 8)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        if( nResult == 7 )
        {
            strMessageKey = "t_ui_advt_error_2";
        }
        else if( nResult == 4 )
        {
            strMessageKey = "t_ui_advt_error_5";
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
}
