//
//  UIRaceStaus.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/07/26.
//

#include "UIRaceStatus.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/OtherEvent/EventRace/UI/UISubItemStatus.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/CostumeSkinManager.h"

USING_NS_CC;

UIRaceStatus* UIRaceStatus::create(const int nIdx)
{
    UIRaceStatus* pRet = new(std::nothrow) UIRaceStatus();
    if ( pRet && pRet->init(nIdx) )
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

UIRaceStatus::UIRaceStatus(void):
_nIdx(0)
,_investCount(0)
,_infoHorse(nullptr)
,_lyTop(nullptr)
,_lyBasicStatus(nullptr)
,_lyHiddenStatus(nullptr)
,_lyBetting(nullptr)
,_btnBetReset(nullptr)
,_btnBetting(nullptr)
,_lbBetCount(nullptr)
{
}

UIRaceStatus::~UIRaceStatus(void)
{
    EventRaceManager::getInstance()->unSubRaceReadyEvent(this);
    EventRaceManager::getInstance()->unSubStatusEvent(this);
    EventRaceManager::getInstance()->unSubAdsEvent(this);
    EventRaceManager::getInstance()->unSubInvestmentEvent(this);
}

bool UIRaceStatus::init(const int nIdx)
{
    if (!LayerColor::init())
    {
        return false;
    }
    _nIdx = nIdx;
    
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    // init
    initVar();
    initUI();
    
    EventRaceManager::getInstance()->subRaceReadyEvent(new MafFunction<void()>(CC_CALLBACK_0(UIRaceStatus::onEventClickReady, this), this));
    EventRaceManager::getInstance()->subStatusEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceStatus::onEventClickStatus, this), this));
    EventRaceManager::getInstance()->subAdsEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceStatus::onEventAds, this), this));
    EventRaceManager::getInstance()->subInvestmentEvent(new MafFunction<void(int)>(CC_CALLBACK_1(UIRaceStatus::onEventInvestment, this), this));
    return true;
}
#pragma mark - init
void UIRaceStatus::initVar()
{
    setContentSize(Size(446,858));
    setIgnoreAnchorPointForPosition(false);
}

void UIRaceStatus::initUI()
{
    auto sprBg = ui::Scale9Sprite::create("Assets/ui/race_event/race_betting_box.png",Rect::ZERO, Rect(10,12,12,12));
    sprBg->setContentSize(getContentSize());
    sprBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprBg->setPosition(getContentSize()/2);
    addChild(sprBg);
    
    auto sprArrow = Sprite::create("Assets/ui/race_event/race_betting_box_arrow.png");
    sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    sprArrow->setPosition(6, getContentSize().height - 143/2 - 143 * (_nIdx - 1));
    addChild(sprArrow);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize(Size(getContentSize().width - 40, 80));
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _lyTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(getContentSize().width / 2, getContentSize().height - 10);
    addChild(_lyTop);
    
    _lyBasicStatus = LayerColor::create();
    _lyBasicStatus->setContentSize(Size(getContentSize().width - 40, 242));
    _lyBasicStatus->setIgnoreAnchorPointForPosition(false);
    _lyBasicStatus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBasicStatus->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    addChild(_lyBasicStatus);
    
    _lyHiddenStatus = LayerColor::create();
    _lyHiddenStatus->setContentSize( Size(getContentSize().width - 40, 157) );
    _lyHiddenStatus->setIgnoreAnchorPointForPosition(false);
    _lyHiddenStatus->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _lyHiddenStatus->setPosition( _lyBasicStatus->getPosition() + Vec2::DOWN * _lyBasicStatus->getContentSize().height);
    addChild(_lyHiddenStatus);
    
    _lyBetting = LayerColor::create();
    _lyBetting->setContentSize(Size(getContentSize().width - 40, getContentSize().height - _lyTop->getContentSize().height - _lyBasicStatus->getContentSize().height - _lyHiddenStatus->getContentSize().height - 20));
    _lyBetting->setIgnoreAnchorPointForPosition(false);
    _lyBetting->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lyBetting->setPosition(_lyHiddenStatus->getPosition() + Vec2::DOWN * _lyHiddenStatus->getContentSize().height);
    addChild(_lyBetting);
    
    uiTop();
    uiBasicStatus();
    uiHiddenStatus();
    uiBetting();
}
#pragma mark - UI
void UIRaceStatus::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprIconBox = Sprite::create("Assets/ui/main/main_hero_edge.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(0, _lyTop->getContentSize().height/2);
    _lyTop->addChild(sprIconBox);
    
    auto sprHeroIcon = Sprite::create(MafUtils::format("Assets/icon_hero/skin_%d.png", _infoHorse->getSkinIdx()));
    sprHeroIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprHeroIcon->setPosition(sprIconBox->getContentSize()/2);
    sprHeroIcon->setScale(sprIconBox->getContentSize().width / sprHeroIcon->getContentSize().width);
    sprIconBox->addChild(sprHeroIcon);
    
    //280
    auto objSkin = CostumeSkinManager::getInstance()->getInfoSkin(_infoHorse->getSkinIdx());
    
    auto lbName = Label::createWithTTF(objSkin->getName(), GAME_FONT, 30);
    lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbName->setPosition(sprIconBox->getPosition() + Vec2::RIGHT * (sprIconBox->getContentSize().width + 20));
    lbName->setDimensions(260, _lyTop->getContentSize().height - 10);
    lbName->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbName->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbName->enableBold();
    lbName->setColor(COLOR_TITLE);
    _lyTop->addChild(lbName);
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_x.png");
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnClose->setPosition(Vec2(_lyTop->getContentSize().width, _lyTop->getContentSize().height/2));
    btnClose->addClickEventListener(CC_CALLBACK_1(UIRaceStatus::onClickClose, this));
    _lyTop->addChild(btnClose);
}

void UIRaceStatus::uiBasicStatus()
{
    _lyBasicStatus->removeAllChildren();
    
    auto listStat = _infoHorse->getListBasicStat();
    Vec2 pos = Vec2(_lyBasicStatus->getContentSize().width/2, _lyBasicStatus->getContentSize().height);
    for(auto obj : listStat)
    {
        auto uiSubItem = UISubItemStatus::create(Size(412, 40), obj);
        uiSubItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiSubItem->setPosition(pos);
        _lyBasicStatus->addChild(uiSubItem);
        
        pos += Vec2::DOWN * 48;
    }
}

void UIRaceStatus::uiHiddenStatus()
{
    _lyHiddenStatus->removeAllChildren();
    
    auto sprHiddenBox = Sprite::create("Assets/ui/race_event/race_info_hidden_title.png");
    sprHiddenBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprHiddenBox->setPosition(_lyHiddenStatus->getContentSize().width/2, _lyHiddenStatus->getContentSize().height);
    _lyHiddenStatus->addChild(sprHiddenBox);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_race_hidden_stat_0"), GAME_FONT, 30);
    lbTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbTitle->setPosition(sprHiddenBox->getContentSize()/2);
    lbTitle->setDimensions(sprHiddenBox->getContentSize().width - 140, sprHiddenBox->getContentSize().height - 10);
    lbTitle->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbTitle->setOverflow(cocos2d::Label::Overflow::SHRINK);
    lbTitle->enableBold();
    lbTitle->setColor(Color3B(145,155,170));
    sprHiddenBox->addChild(lbTitle);
    
    auto listStat = _infoHorse->getListHiddenStat();
    Vec2 pos = Vec2(_lyHiddenStatus->getContentSize().width/2, sprHiddenBox->getPositionY() - sprHiddenBox->getContentSize().height - 10);
    for(auto obj : listStat)
    {
        auto uiSubItem = UISubItemStatus::create(Size(412, 40), obj);
        uiSubItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiSubItem->setPosition(pos);
        _lyHiddenStatus->addChild(uiSubItem);
        
        pos += Vec2::DOWN * 48;
    }
}

void UIRaceStatus::uiBetting()
{
    _lyBetting->removeAllChildren();
    
    auto sprHiddenBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_betting_box_line.png", Rect::ZERO, Rect(10, 0, 30, 14));
    sprHiddenBox->setContentSize(Size(_lyBetting->getContentSize().width, sprHiddenBox->getContentSize().height));
    sprHiddenBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprHiddenBox->setPosition(_lyBetting->getContentSize().width/2, _lyBetting->getContentSize().height);
    _lyBetting->addChild(sprHiddenBox);
    
    std::vector<int> listBtn = {100, 1000, 10000};
    float posY = sprHiddenBox->getPositionY() - sprHiddenBox->getContentSize().height - 17;
    uiBetButtonSet(listBtn, posY);
    
    listBtn.clear();
    listBtn = {100000, -10, -50};
    posY -= 82;
    uiBetButtonSet(listBtn, posY);
    
    posY -= 82;
    
    auto sprBetBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_coin_count_box.png",Rect::ZERO, Rect(3, 3, 50, 50));
    sprBetBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    sprBetBox->setPosition(Vec2(0, posY));
    sprBetBox->setContentSize(Size(282, 60));
    _lyBetting->addChild(sprBetBox);
    {
        auto sprCoinIcon = Sprite::create("Assets/icon/icon_race_coin.png");
        sprCoinIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        sprCoinIcon->setScale(0.75f);
        sprCoinIcon->setPosition(10, sprBetBox->getContentSize().height/2);
        sprBetBox->addChild(sprCoinIcon);
        
        std::string strCurCount = MafUtils::doubleToString(_infoHorse->getInvest());
        MafUtils::convertNumberToShort(strCurCount);
        std::string strPlusCount = MafUtils::doubleToString(_investCount);
        MafUtils::convertNumberToShort(strPlusCount);
        
        _lbBetCount = ui::CText::create(MafUtils::format("%s {#00e600:+ %s}", strCurCount.c_str(), strPlusCount.c_str()), GAME_FONT, 30);
        _lbBetCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        _lbBetCount->setPosition(Vec2(sprBetBox->getContentSize().width - 10, sprBetBox->getContentSize().height/2));
        _lbBetCount->setTextAreaSize(Size(215, sprBetBox->getContentSize().height - 10));
        _lbBetCount->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        _lbBetCount->setTextHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
        _lbBetCount->setTextOverflow(cocos2d::Label::Overflow::SHRINK);
        _lbBetCount->enableBold();
        _lbBetCount->setColor(Color3B(250,255,205));
        sprBetBox->addChild(_lbBetCount);
    }
    
    auto btnBetMax = ui::Button::create("Assets/ui/race_event/race_btn_blue.png");
    btnBetMax->setScale9Enabled(true);
    btnBetMax->setContentSize(Size(120, 60));
    btnBetMax->getRendererNormal()->setCapInsets(Rect(8, 8, 49, 49));
    btnBetMax->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btnBetMax->setPosition(sprBetBox->getPosition() + Vec2::RIGHT * (sprBetBox->getContentSize().width + 9));
    btnBetMax->addClickEventListener(CC_CALLBACK_1(UIRaceStatus::onClickBet, this));
    btnBetMax->setTag(-1);
    btnBetMax->setZoomScale(0.05f);
    {
        auto lbText = Label::createWithTTF("MAX", GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnBetMax->getContentSize()/2);
        lbText->setDimensions(btnBetMax->getContentSize().width - 10, btnBetMax->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnBetMax->addChild(lbText);
    }
    _lyBetting->addChild(btnBetMax);
    
    _btnBetReset = ui::Button::create("Assets/ui/race_event/race_btn_gary.png");
    _btnBetReset->setScale9Enabled(true);
    _btnBetReset->setContentSize(Size(190, 64));
    _btnBetReset->getRendererNormal()->setCapInsets(Rect(8, 8, 49, 49));
    _btnBetReset->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _btnBetReset->setPosition(Vec2(_lyBetting->getContentSize().width/2 - 10, 20));
    _btnBetReset->addClickEventListener(CC_CALLBACK_1(UIRaceStatus::onClickBetReset, this));
    _btnBetReset->setZoomScale(0.05f);
    {
        auto lbText = Label::createWithTTF("RESET", GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(_btnBetReset->getContentSize()/2);
        lbText->setDimensions(_btnBetReset->getContentSize().width - 10, _btnBetReset->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        _btnBetReset->addChild(lbText);
    }
    _lyBetting->addChild(_btnBetReset);
    
    _btnBetting = ui::Button::create("Assets/ui/race_event/race_btn_gary.png");
    _btnBetting->setScale9Enabled(true);
    _btnBetting->setContentSize(Size(190, 64));
    _btnBetting->getRendererNormal()->setCapInsets(Rect(8, 8, 49, 49));
    _btnBetting->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _btnBetting->setPosition(Vec2(_lyBetting->getContentSize().width/2 + 10, 20));
    _btnBetting->addClickEventListener(CC_CALLBACK_1(UIRaceStatus::onClickBetComplete, this));
    _btnBetting->setZoomScale(0.02f);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_event_race_bet_1"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(_btnBetting->getContentSize()/2);
        lbText->setDimensions(_btnBetting->getContentSize().width - 10, _btnBetting->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        _btnBetting->addChild(lbText);
    }
    _lyBetting->addChild(_btnBetting);
    
    if(EventRaceManager::getInstance()->isEventEndSeason())
        _btnBetting->setEnabled(false);
    
}
void UIRaceStatus::uiBetButtonSet(std::vector<int> listBetCount, float posY)
{
    float width = (_lyBetting->getContentSize().width + 20) / listBetCount.size() - 15;
    float posX = _lyBetting->getContentSize().width/2 - width * (listBetCount.size()/2) - 15 * (listBetCount.size()/2);
    if(listBetCount.size() % 2 == 0)
        posX += 8;
    for(int i = 0; i < listBetCount.size(); ++i)
    {
        auto obj = listBetCount[i];
        std::string strCount = MafUtils::doubleToString(obj);
        if(obj < 0)
            strCount = MafUtils::doubleToString(obj * -1) + "%";
        else
            MafUtils::convertNumberToShort(strCount);
        
        auto btnBet = ui::Button::create("Assets/ui/race_event/race_btn_blue.png");
        btnBet->setScale9Enabled(true);
        btnBet->setContentSize(Size(width, 64));
        btnBet->getRendererNormal()->setCapInsets(Rect(8, 8, 49, 49));
        btnBet->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btnBet->setPosition(Vec2(posX, posY));
        btnBet->addClickEventListener(CC_CALLBACK_1(UIRaceStatus::onClickBet, this));
        btnBet->setZoomScale(0.05f);
        btnBet->setTag(obj);
        {
            auto lbText = Label::createWithTTF("+ " + strCount, GAME_FONT, 30);
            lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            lbText->setPosition(btnBet->getContentSize()/2);
            lbText->setDimensions(btnBet->getContentSize().width - 10, btnBet->getContentSize().height - 10);
            lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
            lbText->enableOutline(Color4B::BLACK, 2);
            btnBet->addChild(lbText);
            
            
        }
        _lyBetting->addChild(btnBet);
        
        
        if(listBetCount.size() % 2 == 0)
            btnBet->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        posX += width + 15;
    }
}
#pragma mark - Utils
void UIRaceStatus::hide()
{
    removeFromParent();
}

#pragma mark - Click
void UIRaceStatus::onClickClose(Ref* const sender)
{
    EventRaceManager::getInstance()->invokeStatusEvent(0);
}

void UIRaceStatus::onClickBet(Ref* const sender)
{
    auto node = (Node*)sender;
    
    double nPlusCount = node->getTag();
    if(nPlusCount == -1)
        nPlusCount = EventRaceManager::getInstance()->getCoin();
    else if(nPlusCount < -1)
    {
        nPlusCount *= -1;
        
        nPlusCount = EventRaceManager::getInstance()->getCoin() * nPlusCount * 0.01f;
        nPlusCount = std::round(nPlusCount);
    }
    
    _investCount = _investCount + nPlusCount;
    
    if(_investCount >= EventRaceManager::getInstance()->getCoin())
        _investCount = EventRaceManager::getInstance()->getCoin();
        
    std::string strCurCount = MafUtils::doubleToString(_infoHorse->getInvest());
    MafUtils::convertNumberToShort(strCurCount);
    std::string strPlusCount = MafUtils::doubleToString(_investCount);
    MafUtils::convertNumberToShort(strPlusCount);
    
    _lbBetCount->setString(MafUtils::format("%s {#00e600:+ %s}", strCurCount.c_str(), strPlusCount.c_str()));
    
    SoundManager::SoundEffectStart(SOUND_BOX_CREATE, E_SOUND_TYPE::CONTENTS);
    
    if(_investCount > 0)
    {
        if(_btnBetting != nullptr)
        {
            _btnBetting->getRendererNormal()->setTexture("Assets/ui/race_event/race_btn_green.png");
            _btnBetting->setScale9Enabled(true);
            _btnBetting->getRendererNormal()->setContentSize(_btnBetting->getContentSize());
        }
        if(_btnBetReset != nullptr)
        {
            _btnBetReset->getRendererNormal()->setTexture("Assets/ui/race_event/race_btn_red.png");
            _btnBetReset->setScale9Enabled(true);
            _btnBetReset->getRendererNormal()->setContentSize(_btnBetReset->getContentSize());
        }
    }
}

void UIRaceStatus::onClickBetReset(Ref* const sender)
{
    _investCount = 0;
    
    std::string strCurCount = MafUtils::doubleToString(_infoHorse->getInvest());
    MafUtils::convertNumberToShort(strCurCount);
    std::string strPlusCount = MafUtils::doubleToString(_investCount);
    MafUtils::convertNumberToShort(strPlusCount);
    
    _lbBetCount->setString(MafUtils::format("%s {#00e600:+ %s}", strCurCount.c_str(), strPlusCount.c_str()));
    SoundManager::SoundEffectStart(SOUND_BOX_CREATE, E_SOUND_TYPE::CONTENTS);
    
    if(_investCount <= 0)
    {
        if(_btnBetting != nullptr)
        {
            _btnBetting->getRendererNormal()->setTexture("Assets/ui/race_event/race_btn_gary.png");
            _btnBetting->setScale9Enabled(true);
            _btnBetting->getRendererNormal()->setContentSize(_btnBetting->getContentSize());
        }
        if(_btnBetReset != nullptr)
        {
            _btnBetReset->getRendererNormal()->setTexture("Assets/ui/race_event/race_btn_gary.png");
            _btnBetReset->setScale9Enabled(true);
            _btnBetReset->getRendererNormal()->setContentSize(_btnBetReset->getContentSize());
        }
    }
}

void UIRaceStatus::onClickBetComplete(Ref* const sender)
{
    if(_investCount <= 0)
        return;
    
    if(EventRaceManager::getInstance()->isEventEndSeason())
    {
        EventRaceManager::getInstance()->invokeEventOverEvent();
        return;
    }
    
    PopupLoading::show();
    EventRaceManager::getInstance()->requestInvestment(CC_CALLBACK_2(UIRaceStatus::callbackInvestment, this), _infoHorse->getSkinIdx(), _investCount);
    SoundManager::SoundEffectStart(SOUND_BOX_CREATE, E_SOUND_TYPE::CONTENTS);
}

#pragma mark - Callback
void UIRaceStatus::callbackInvestment(bool bResult, int nResult)
{
    PopupLoading::hide();
    if(nResult != 1)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        return;
    }
    
    SoundManager::SoundEffectStart(SOUND_BUFF, E_SOUND_TYPE::CONTENTS);
    _investCount = 0;
    EventRaceManager::getInstance()->invokeInvestmentEvent(_nIdx);
}

#pragma mark - Event
void UIRaceStatus::onEventClickReady()
{
    runAction(Sequence::create(DelayTime::create(0.01), CallFunc::create(CC_CALLBACK_0(UIRaceStatus::hide, this)), nullptr));
}

void UIRaceStatus::onEventClickStatus(const int nIdx)
{
    if(_nIdx != nIdx)
        runAction(Sequence::create(DelayTime::create(0.01), CallFunc::create(CC_CALLBACK_0(UIRaceStatus::hide, this)), nullptr));
}

void UIRaceStatus::onEventAds(int)
{
    uiHiddenStatus();
}
void UIRaceStatus::onEventInvestment(const int nIdx)
{
    _infoHorse = EventRaceManager::getInstance()->getInfoHorse(_nIdx);
    uiBetting();
}
