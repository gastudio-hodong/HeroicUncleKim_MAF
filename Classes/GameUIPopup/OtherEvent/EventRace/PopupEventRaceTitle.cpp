//
//  PopupEventRaceTitle.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/04.
//

#include "PopupEventRaceTitle.h"

#include "GameObject/Event/InfoActivatedEvent.h"

#include "GameUIPopup/OtherEvent/EventRace/PopupEventRace.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerEvent/EventManager.h"
#include "ManagerGame/DialogManager.h"

USING_NS_CC;

PopupEventRaceTitle* PopupEventRaceTitle::create()
{
    PopupEventRaceTitle* pRet = new(std::nothrow) PopupEventRaceTitle();
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

PopupEventRaceTitle::PopupEventRaceTitle()
{
}

PopupEventRaceTitle::~PopupEventRaceTitle(void)
{
    
}
bool PopupEventRaceTitle::init()
{
    if (!PopupBase::init(PopupBaseType::TRANSPARENT))
    {
        return false;
    }
    
    SoundManager::ChangeSoundState(E_SOUND_TYPE::CONTENTS);
    SoundManager::SoundBgmStart(SOUND_EVENT_RACE_BGM_1, E_SOUND_TYPE::CONTENTS);
    
    _bHideBackKey = false;
    
    initVar();
    initUI();
    
    DialogManager::getInstance()->showDialog("EVENT_RACE_OPENNING");
    
    return true;
}
#pragma mark - ui
void PopupEventRaceTitle::initVar()
{
    
}

void PopupEventRaceTitle::initUI()
{
    Size size = getContentSize();
    if(size.width >= 750)
    {
        size.width = 750;
    }
    
    auto lyContainerParent = ui::Layout::create();
    lyContainerParent->setContentSize(size);
    lyContainerParent->setClippingEnabled(true);
    lyContainerParent->setIgnoreAnchorPointForPosition(false);
    lyContainerParent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lyContainerParent->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    addChild(lyContainerParent);
    
    auto sprBg = Sprite::create("Assets/ui/race_event/race_title_bg.png");
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(lyContainerParent->getContentSize()/2);
    sprBg->getTexture()->setTexParameters(texParams);
    sprBg->setScale(4.f);
    lyContainerParent->addChild(sprBg);
    
    auto sprPost = Sprite::create("Assets/ui/race_event/race_title_ani00.png");
    sprPost->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprPost->setPosition(lyContainerParent->getContentSize()/2);
    sprPost->getTexture()->setTexParameters(texParams);
    sprPost->setScale(3.f);
    lyContainerParent->addChild(sprPost);
    
    
    std::string strTitlePath = "Assets/ui/race_event/race_title_en.png";
    if(TextManager::getInstance()->getLang().compare("ko") == 0)
        strTitlePath = "Assets/ui/race_event/race_title.png";
    else if(TextManager::getInstance()->getLang().compare("jp") == 0)
        strTitlePath = "Assets/ui/race_event/race_title_jn.png";
    else if(TextManager::getInstance()->getLang().compare("zh_tw") == 0)
        strTitlePath = "Assets/ui/race_event/race_title_ta.png";
    
    auto sprTitle = Sprite::create(strTitlePath);
    sprTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprTitle->setPosition(Vec2(lyContainerParent->getContentSize()/2) + Vec2::DOWN * 280);
    sprTitle->setScale(4.f);
    sprTitle->getTexture()->setTexParameters(texParams);
    sprTitle->setOpacity(0);
    lyContainerParent->addChild(sprTitle);
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.1f);
    for(int i = 0; i <= 15; ++i)
    {
        auto frame = Sprite::create(MafUtils::format("Assets/ui/race_event/race_title_ani%02d.png", i));
        frame->getTexture()->setTexParameters(texParams);
        anim->addSpriteFrame(frame->getSpriteFrame());
    }
    auto animate = Animate::create(anim);
    auto func = CallFunc::create([&, sprTitle](){
        sprTitle->runAction(FadeIn::create(0.5f));
    });
    auto seq = Sequence::create(animate, func, nullptr);
    sprPost->runAction(seq);
    
    auto btnStart = ui::Button::create("Assets/ui/race_event/race_title_btn.png");
    btnStart->setScale9Enabled(true);
    btnStart->getRendererNormal()->setCapInsets(Rect(13, 20, 10, 90));
    btnStart->setContentSize(Size(350, 120));
    btnStart->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnStart->setPosition(Vec2(lyContainerParent->getContentSize().width/2, 110));
    btnStart->setZoomScale(0.02f);
    btnStart->addClickEventListener(CC_CALLBACK_1(PopupEventRaceTitle::onClickStart, this));
    {
        auto lbText = Label::createWithTTF("Go", GAME_FONT, 50);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(Vec2(btnStart->getContentSize()/2) + Vec2::UP * 5);
        lbText->setDimensions(btnStart->getContentSize().width - 10, btnStart->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        lbText->enableBold();
        btnStart->addChild(lbText);
    }
    lyContainerParent->addChild(btnStart);
    
    auto uiTimeBG = ui::ImageView::create("Assets/ui/race_event/race_title_noti_1.png");
    uiTimeBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTimeBG->setPosition(Vec2(lyContainerParent->getContentSize().width / 2, lyContainerParent->getContentSize().height - 70));
    lyContainerParent->addChild(uiTimeBG);
    {
        auto callFunc = CallFuncN::create([=](Ref* sender){
            
            InfoActivatedEvent* objEvent = nullptr;
            if(EventRaceManager::getInstance()->isEventEndSeason())
                objEvent = EventManager::getInstance()->getActivatedEvent(34);
            else
                objEvent = EventManager::getInstance()->getActivatedEvent(33);
            
            if(objEvent == nullptr)
            {
                ((ui::CText*)sender)->stopAllActions();
                return;
            }
                
            long time = objEvent->getEventEnd() - UtilsDate::getInstance()->getTime();
            int day = (int)(time / (24 * 3600));
            if(day >0)
                time = time % (24 * 3600);
            int hour = (int)time / 3600;
            int min = (time % 3600) / 60;
            
            if(day < 0)
                day = 0;
            if(hour < 0)
                hour = 0;
            if(min < 0)
                min = 0;
            
            std::string strText = GAME_TEXTFORMAT("t_ui_event_timer_1", MafUtils::format("%02d(D) : %02d(H) : %02d(M)", day, hour, min).c_str());
            if(objEvent->getIdx() == 34)
                strText = GAME_TEXTFORMAT("t_ui_event_reward_timer_2", MafUtils::format("%02d(D) : %02d(H) : %02d(M)", day, hour, min).c_str());
            
            ((ui::CText*)sender)->setString(strText);
        });
        
        auto uiTime = ui::CText::create(" ", GAME_FONT, 38);
        uiTime->setPosition(Vec2(uiTimeBG->getContentSize().width/2, uiTimeBG->getContentSize().height / 2));
        uiTime->setTextAreaSize(Size(uiTimeBG->getContentSize().width - 10, uiTime->getContentSize().height));
        uiTime->setTextOverflow(Label::Overflow::SHRINK);
        uiTime->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTime->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTime->enableOutline(Color4B::BLACK, 2);
        uiTime->enableBold();
        uiTime->runAction(RepeatForever::create(Sequence::create(callFunc, DelayTime::create(60.f), nullptr)));
        uiTimeBG->addChild(uiTime);
    }
    
    //
    auto uiGameEnd = ui::CText::create(GAME_TEXT("t_ui_event_race_end_msg"), GAME_FONT, 50);
    if ( uiGameEnd->getContentSize().width > lyContainerParent->getContentSize().width )
    {
        uiGameEnd->setTextAreaSize(Size(lyContainerParent->getContentSize().width - 100, uiGameEnd->getContentSize().height));
        uiGameEnd->setTextOverflow(Label::Overflow::SHRINK);
        uiGameEnd->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiGameEnd->setTextHorizontalAlignment(TextHAlignment::CENTER);
    }
    uiGameEnd->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiGameEnd->setPosition(Vec2(lyContainerParent->getContentSize().width / 2, lyContainerParent->getContentSize().height - 200));
    uiGameEnd->enableOutline(Color4B::BLACK, 2);
    uiGameEnd->enableBold();
    uiGameEnd->setVisible(false);
    lyContainerParent->addChild(uiGameEnd);
    {
        auto action0 = DelayTime::create(2.0);
        auto action1 = MoveBy::create(1.0, Vec2(-lyContainerParent->getContentSize().width, 0));
        auto action2 = MoveBy::create(0, Vec2(lyContainerParent->getContentSize().width * 2, 0));
        auto action3 = MoveBy::create(1.0, Vec2(-lyContainerParent->getContentSize().width, 0));
        auto seq = Sequence::create(action0, action1, action2, action3, NULL);
        uiGameEnd->runAction(RepeatForever::create(seq));
    }
    if ( EventManager::getInstance()->isActivatedEvent(33) == false )
    {
        uiGameEnd->setVisible(true);
    }
    
    //
    auto fxPollen = ParticleSystemQuad::create("Assets/Particle/particle_pollen.plist");
    fxPollen->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    fxPollen->setPosition(lyContainerParent->getContentSize()/2);
    fxPollen->setScale(1.5);
    lyContainerParent->addChild(fxPollen);
}

#pragma mark - event
void PopupEventRaceTitle::onClickStart(Ref* sender)
{
    auto popup = PopupEventRace::create();
    popup->show();
    
    hide();
}
void PopupEventRaceTitle::onClickClose(Ref* sender)
{
    SoundManager::ChangeSoundState(E_SOUND_TYPE::SCENE);
    hide();
}
