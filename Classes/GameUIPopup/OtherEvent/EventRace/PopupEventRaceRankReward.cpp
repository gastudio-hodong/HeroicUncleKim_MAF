//
//  PopupEventRaceRankReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/16.
//

#include "PopupEventRaceRankReward.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventRaceManager.h"
#include "ManagerGame/DialogManager.h"
#include "ManagerGame/RankingManager.h"

USING_NS_CC;

PopupEventRaceRankReward* PopupEventRaceRankReward::create(Vector<InfoItems*> list)
{
    PopupEventRaceRankReward* pRet = new(std::nothrow) PopupEventRaceRankReward();
    if ( pRet && pRet->init(list) )
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

PopupEventRaceRankReward::PopupEventRaceRankReward():
_lyBottom(nullptr)
,_lyMiddle(nullptr)
,_lyTop(nullptr)
{
    _listReward.clear();
}

PopupEventRaceRankReward::~PopupEventRaceRankReward(void)
{
    _listReward.clear();
}
bool PopupEventRaceRankReward::init(Vector<InfoItems*> list)
{
    if (!PopupBase::init())
    {
        return false;
    }
    _listReward = list;
    initVar();
    
    return true;
}
void PopupEventRaceRankReward::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    RankingManager::getInstance()->requestEventRaceRankInfo(0, CC_CALLBACK_3(PopupEventRaceRankReward::callbackInfo, this));
    
    _spriteContainer->setVisible(false);
}
#pragma mark - ui
void PopupEventRaceRankReward::initVar()
{
    _spriteContainer->setContentSize(Size(500,530));
}

void PopupEventRaceRankReward::initUI()
{
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    _lyTop = LayerColor::create();
    _lyTop->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 100));
    _lyTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyTop->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 75);
    _lyTop->setIgnoreAnchorPointForPosition(false);
    _spriteContainer->addChild(_lyTop);
    
    _lyMiddle = LayerColor::create();
    _lyMiddle->setContentSize( Size(_spriteContainer->getContentSize().width - 10, 220));
    _lyMiddle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyMiddle->setPosition(_lyTop->getPosition() + Vec2::DOWN * _lyTop->getContentSize().height);
    _lyMiddle->setIgnoreAnchorPointForPosition(false);
    _spriteContainer->addChild(_lyMiddle);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize( Size(_spriteContainer->getContentSize().width - 10, _spriteContainer->getContentSize().height - 75 - _lyTop->getContentSize().height - _lyMiddle->getContentSize().height));
    _lyBottom->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP);
    _lyBottom->setPosition(_lyMiddle->getPosition() + Vec2::DOWN * _lyMiddle->getContentSize().height);
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _spriteContainer->addChild(_lyBottom);
    
    uiTop();
    uiMiddle();
    uiBottom();
}

void PopupEventRaceRankReward::uiTop()
{
    _lyTop->removeAllChildren();
    
    auto sprRankBox = ui::Scale9Sprite::create("Assets/ui/race_event/race_reward_title.png",Rect::ZERO, Rect(67, 0, 22, 40));
    sprRankBox->setContentSize(Size(_lyTop->getContentSize().width - 40, 60));
    sprRankBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprRankBox->setPosition(_lyTop->getContentSize()/2);
    _lyTop->addChild(sprRankBox);
    
    int rank = 0;
    
    auto objRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::EVENT_RACE);
    if(objRank != nullptr)
        rank = objRank->getRank();
    
    auto lbRank = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_rank_now", rank), GAME_FONT, 24);
    lbRank->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbRank->setPosition(sprRankBox->getContentSize()/2);
    lbRank->setDimensions(sprRankBox->getContentSize().width - 80, sprRankBox->getContentSize().height - 10);
    lbRank->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbRank->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprRankBox->addChild(lbRank);
    
}

void PopupEventRaceRankReward::uiMiddle()
{
    _lyMiddle->removeAllChildren();
    
    
    int size = (int)_listReward.size();
    
    float width = 110;
    float posX = _lyMiddle->getContentSize().width/2 - width * (size/2) - 15 * (size/2);
    
    for(int i = 0; i < _listReward.size(); ++i)
    {
        auto objItem = _listReward.at(i);
        
        auto sprBox = Sprite::create("Assets/ui_common/box_reward.png");
        sprBox->setIgnoreAnchorPointForPosition(false);
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        sprBox->setPosition(posX, _lyMiddle->getContentSize().height - 20);
        _lyMiddle->addChild(sprBox);
        
        if(size % 2 == 0)
            sprBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            
        posX += width + 15;
        
        auto sprIcon = Sprite::create(objItem->getPath());
        sprIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprIcon->setPosition(sprBox->getContentSize()/2);
        sprBox->addChild(sprIcon);
        if(objItem->getIdx() == (int)E_ITEMS::WEAPON_SPIRIT)
            sprIcon->setScale(0.85f);
        
        std::string strCount = "x" + objItem->getCount();
        
        if(objItem->getIdx() == (int)E_ITEMS::WEAPON_SPIRIT && atoi(objItem->getCount().c_str()) <= 0)
            strCount = "x1";
        
        auto lbCount = Label::createWithTTF(strCount, GAME_FONT, 25);
        lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
        lbCount->setDimensions(sprBox->getContentSize().width, lbCount->getContentSize().height);
        lbCount->setPosition(sprBox->getContentSize().width/2, 5);
        lbCount->setOverflow(Label::Overflow::SHRINK);
        sprBox->addChild(lbCount);
    }
    
    auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_randombox_msg_4"), GAME_FONT, 28);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbText->setPosition(_lyMiddle->getContentSize().width/2, 15);
    lbText->setDimensions(_lyMiddle->getContentSize().width - 10, lbText->getContentSize().height);
    lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
    _lyMiddle->addChild(lbText);
}

void PopupEventRaceRankReward::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    
    auto btnClose = ui::Button::create("Assets/ui_common/btn_empty.png");
    btnClose->setScale9Enabled(true);
    btnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    btnClose->setPosition(_lyBottom->getContentSize()/2);
    btnClose->setZoomScale(0.02f);
    btnClose->addClickEventListener(CC_CALLBACK_1(PopupEventRaceRankReward::onClickClose, this));
    _lyBottom->addChild(btnClose);
    {
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 30);
        lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbText->setPosition(btnClose->getContentSize()/2);
        lbText->setDimensions(btnClose->getContentSize().width - 10, btnClose->getContentSize().height - 10);
        lbText->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        lbText->setOverflow(cocos2d::Label::Overflow::SHRINK);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnClose->addChild(lbText);
    }
}

#pragma mark - Utils
void PopupEventRaceRankReward::onClickClose(Ref* sender)
{
    auto objRank = RankingManager::getInstance()->getMyRankInfoFromLocalType(E_RANK_LOCAL_TYPE::EVENT_RACE);
    if(objRank != nullptr)
    {
        if(objRank->getRank() == 1)
            DialogManager::getInstance()->showDialog("EVENT_RACE_1ST_ENDING");
    }
    hide();
}

#pragma mark - Callback
void PopupEventRaceRankReward::callbackInfo(bool bResult, std::vector<RankInfo*>, std::vector<RankInfo*>)
{
    PopupLoading::hide();
    
    if(!bResult)
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    _spriteContainer->setVisible(true);
    initUI();
}
