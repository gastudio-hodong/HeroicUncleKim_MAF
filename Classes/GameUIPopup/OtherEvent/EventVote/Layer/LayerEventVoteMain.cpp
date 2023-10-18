//
//  LayerEventVoteMain.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/28.
//

#include "LayerEventVoteMain.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/OtherEvent/EventVote/PopupEventVoting.h"

#include "ManagerEvent/EventVoteManager.h"

USING_NS_CC;

LayerEventVoteMain* LayerEventVoteMain::create(cocos2d::Size size)
{
    LayerEventVoteMain* pRet = new(std::nothrow) LayerEventVoteMain();
    if ( pRet && pRet->init(size) )
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

LayerEventVoteMain::LayerEventVoteMain(void) :
_bInitUI(false),

_bResult(false),

_uiContentsVote(nullptr)
{
}

LayerEventVoteMain::~LayerEventVoteMain(void)
{
    EventVoteManager::getInstance()->unSubDelegateInfo(this);
}

bool LayerEventVoteMain::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setClippingEnabled(true);
    setTouchEnabled(true);
    
    EventVoteManager::getInstance()->subDelegateInfo(new MafFunction<void(bool, int)>(CC_CALLBACK_2(LayerEventVoteMain::callbackInfo, this), this));
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerEventVoteMain::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        uiContainer();
        uiVote();
    }
    
    //
    drawVote();
}

void LayerEventVoteMain::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawVote();
    }
}

#pragma mark - init
void LayerEventVoteMain::initVar()
{
    
}

void LayerEventVoteMain::initUi()
{
    
}

#pragma mark - ui
void LayerEventVoteMain::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsVote = ui::Layout::create();
    _uiContentsVote->setContentSize(size);
    _uiContentsVote->setAnchorPoint(Vec2::ZERO);
    _uiContentsVote->setPosition(Vec2::ZERO);
    addChild(_uiContentsVote);
}

void LayerEventVoteMain::uiVote()
{
    auto layout = _uiContentsVote;
    auto size = layout->getContentSize();
    
    //타이틀
    auto uiTitleBox = ui::ImageView::create("Assets/ui/event/minority_event_bar.png");
    uiTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiTitleBox->setPosition(Vec2(size.width/2, size.height - 25));
    ((ui::Scale9Sprite*)uiTitleBox->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiTitleBox->setScale(3);
    layout->addChild(uiTitleBox);

    auto uiTitle = ui::CText::create("", GAME_FONT, 30);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiTitle->setPosition(uiTitleBox->getPosition() + Vec2::DOWN * (uiTitleBox->getContentSize().height / 2 * uiTitleBox->getScale() - 3));
    uiTitle->setTextAreaSize(Size(size.width - 50, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setName("UI_TITLE");
    layout->addChild(uiTitle);
    {
        auto callFunc = CallFunc::create([&, uiTitle](){
            time_t nTimeEnd = EventVoteManager::getInstance()->getEndVoteTime();
            int64_t nTimeNow = UtilsDate::getInstance()->getTime();
            
            int nTimeDiff = (int)(nTimeEnd - nTimeNow);
            if(nTimeDiff <= 0)
            {
                EventVoteManager::getInstance()->requestInfo(true);
                uiTitle->stopAllActions();
                return;
            }
            int h = nTimeDiff / 3600;
            int m = nTimeDiff % 3600 / 60;
            int s = nTimeDiff % 3600 % 60;
            
            if(h < 0)
                h=0;
            if(m < 0)
                m=0;
            if(s < 0)
                s=0;
            
            std::string strTime = MafUtils::format("%02d : %02d : %02d", h, m, s);
            if(EventVoteManager::getInstance()->isBreakTime())
                strTime = GAME_TEXTFORMAT("t_ui_event_vote_title_6", strTime.c_str());
            else
                strTime = GAME_TEXTFORMAT("t_ui_event_vote_title_3", strTime.c_str());
            uiTitle->setString(strTime);
        });
        auto seq = Sequence::create(callFunc, DelayTime::create(1), nullptr);
        uiTitle->runAction(RepeatForever::create(seq));
    }
    
    //투표 후보 UI
    
    auto list = EventVoteManager::getInstance()->getListVoteCandidate();
    for(int i = 0; i <  list.size(); ++i)
    {
        auto obj = list.at(i);
        if(obj == nullptr)
            continue;
        int idx = obj->getIdx();
        
        auto layoutCandidate = ui::Layout::create();
        layoutCandidate->setContentSize(Size(140, 400));
        layoutCandidate->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        layoutCandidate->setPosition(Vec2(10, 20) + Vec2::RIGHT * i * (layoutCandidate->getContentSize().width + 5));
        layoutCandidate->setName("UI_LAYOUT_CANDIDATE_" + std::to_string(i+1));
        layout->addChild(layoutCandidate);
        
        auto uiBox = ui::ImageView::create("Assets/ui/event/minority_event_vote_box.png");
        uiBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiBox->setPosition(layoutCandidate->getContentSize()/2);
        ((ui::Scale9Sprite*)uiBox->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiBox->setScale(2);
        layoutCandidate->addChild(uiBox);
        
        auto uiEdge = ui::ImageView::create("Assets/ui/event/minority_event_vote_box_best.png");
        uiEdge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiEdge->setPosition(uiBox->getContentSize()/2);
        ((ui::Scale9Sprite*)uiEdge->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiEdge->setName("UI_EDGE");
        uiEdge->setVisible(false);
        uiBox->addChild(uiEdge);
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 0; i <= 5; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/minority_event_seal%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        auto uiBedge = ui::ImageView::create("Assets/ui/event/minority_event_seal00.png");
        uiBedge->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        uiBedge->setPosition(Vec2(layoutCandidate->getContentSize().width + 5, layoutCandidate->getContentSize().height + 5));
        uiBedge->getVirtualRenderer()->stopAllActions();
        uiBedge->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiBedge->setName("UI_BEDGE");
        uiBedge->setVisible(false);
        layoutCandidate->addChild(uiBedge);
        
        std::string strTitle = "No." + std::to_string(idx);
        
        auto uiTitleText = ui::CText::create(strTitle, GAME_FONT, 26);
        uiTitleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiTitleText->setPosition(Vec2(layoutCandidate->getContentSize().width/2, layoutCandidate->getContentSize().height - 30));
        uiTitleText->setTextAreaSize(Size(layoutCandidate->getContentSize().width - 20, uiTitleText->getContentSize().height));
        uiTitleText->setTextOverflow(Label::Overflow::SHRINK);
        uiTitleText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        layoutCandidate->addChild(uiTitleText);
        
        //득표율
        auto uiPercentText = ui::CText::create(MafUtils::format("%.2f%%", EventVoteManager::getInstance()->getPercentageOfCurrentVotes(obj->getIdx())), GAME_FONT, 30);
        uiPercentText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiPercentText->setPosition(Vec2(layoutCandidate->getContentSize().width/2, layoutCandidate->getContentSize().height - 92));
        uiPercentText->setTextAreaSize(Size(layoutCandidate->getContentSize().width - 20, uiPercentText->getContentSize().height));
        uiPercentText->setTextOverflow(Label::Overflow::SHRINK);
        uiPercentText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiPercentText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiPercentText->setName("UI_PERCENT");
        layoutCandidate->addChild(uiPercentText);
        
        //보상
        auto uiRewardBox = ui::ImageView::create("Assets/ui/return_event/return_reward_box.png");
        uiRewardBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiRewardBox->setPosition(Vec2(layoutCandidate->getContentSize().width/2, 210));
        layoutCandidate->addChild(uiRewardBox);
        {
            auto objItem = obj->getReward();
            if(objItem == nullptr)
                continue;
            
            if (auto uiItemIcon = ui::ImageView::create(objItem->getPath()); uiItemIcon != nullptr )
            {
                uiItemIcon->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, uiRewardBox->getContentSize().height / 2 + 10));
                ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
                uiRewardBox->addChild(uiItemIcon);
                
                std::string strItemsCount = "";
                
                if((E_ITEMS)objItem->getIdx() != E_ITEMS::COSTUME_SKIN)
                {
                    strItemsCount = "x" + objItem->getCount();
                    if (objItem->isString())
                        MafUtils::convertNumberToShort(strItemsCount);
                }

                auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 20);
                uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                uiCount->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, 10));
                uiCount->setTextAreaSize(Size(uiRewardBox->getContentSize().width - 10, uiCount->getContentSize().height));
                uiCount->setTextOverflow(Label::Overflow::SHRINK);
                uiCount->setTextVerticalAlignment(TextVAlignment::BOTTOM);
                uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiRewardBox->addChild(uiCount);
            }
        }
        
        //내 투표
        auto uiTextVoting = ui::CText::create(std::to_string(obj->getVoteCount()), GAME_FONT, 30);
        uiTextVoting->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiTextVoting->setPosition(Vec2(layoutCandidate->getContentSize().width/2, 140));
        uiTextVoting->setTextAreaSize(Size(120, uiTextVoting->getContentSize().height));
        uiTextVoting->setTextOverflow(Label::Overflow::SHRINK);
        uiTextVoting->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTextVoting->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTextVoting->setName("UI_VOTING");
        layoutCandidate->addChild(uiTextVoting);
        
        //투표 버튼
        auto uiBtnVote = ui::Button::create("Assets/ui_common/btn_booster_buy.png");
        uiBtnVote->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBtnVote->setScale9Enabled(true);
        uiBtnVote->setContentSize(Size(layoutCandidate->getContentSize().width - 20, 73));
        uiBtnVote->setPositionType(ui::Widget::PositionType::PERCENT);
        uiBtnVote->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(layoutCandidate->getContentSize(), layoutCandidate->getContentSize().width/2, 20));
        uiBtnVote->setTag(obj->getIdx());
        uiBtnVote->setName("UI_BTN_VOTING");
        uiBtnVote->addClickEventListener(CC_CALLBACK_1(LayerEventVoteMain::onClickVote, this));
        layoutCandidate->addChild(uiBtnVote);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_vote_title_5"), GAME_FONT, 20);
            uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiText->setPosition(uiBtnVote->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiBtnVote->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBtnVote->addChild(uiText);
        }
    }
}

#pragma mark - ui : draw
void LayerEventVoteMain::drawVote()
{
    auto layout = _uiContentsVote;
    auto list = EventVoteManager::getInstance()->getListVoteCandidate();
    for(auto obj : list)
    {
        if(auto layoutCandidate = (ui::Layout*)utils::findChild(layout, "UI_LAYOUT_CANDIDATE_" + std::to_string(obj->getIdx())); layoutCandidate != nullptr)
        {
            if(auto uiVoting = (ui::CText*)utils::findChild(layoutCandidate, "UI_VOTING"); uiVoting != nullptr)
            {
                uiVoting->setString(std::to_string(obj->getVoteCount()));
            }
            if(auto uiPercent = (ui::CText*)utils::findChild(layoutCandidate, "UI_PERCENT"); uiPercent != nullptr)
            {
                uiPercent->setString(MafUtils::format("%.2f%%", EventVoteManager::getInstance()->getPercentageOfCurrentVotes(obj->getIdx())));
            }
            if(auto uiBtnVoting = (ui::Button*)utils::findChild(layoutCandidate, "UI_BTN_VOTING"); uiBtnVoting != nullptr)
            {
                if(EventVoteManager::getInstance()->isBreakTime())
                    uiBtnVoting->setEnabled(false);
            }
            if(auto uiEdge = (ui::ImageView*)utils::findChild(layoutCandidate, "UI_EDGE"); uiEdge != nullptr)
            {
                if(EventVoteManager::getInstance()->getCandidateRank(obj) == 1)
                    uiEdge->setVisible(true);
                else
                    uiEdge->setVisible(false);
            }
            if(auto uiBedge = (ui::ImageView*)utils::findChild(layoutCandidate, "UI_BEDGE"); uiBedge != nullptr)
            {
                if(EventVoteManager::getInstance()->getCandidateRank(obj) == 1)
                    uiBedge->setVisible(true);
                else
                    uiBedge->setVisible(false);
            }
        }
    }
    
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerEventVoteMain::onClickVote(Ref* sender)
{
    auto node = (Node*)sender;
    
    auto popup = PopupEventVoting::create(node->getTag());
    popup->show();
}


#pragma mark - callback
void LayerEventVoteMain::callbackInfo(const bool bResult, const int nResult)
{
    if(nResult == 1 || nResult == 20001)
    {
        auto layout = _uiContentsVote;
        if(auto uiTitle = (ui::CText*)utils::findChild(layout, "UI_TITLE"); uiTitle != nullptr)
        {
            auto callFunc = CallFunc::create([&, uiTitle](){
                time_t nTimeEnd = EventVoteManager::getInstance()->getEndVoteTime();
                int64_t nTimeNow = UtilsDate::getInstance()->getTime();
                
                int nTimeDiff = (int)(nTimeEnd - nTimeNow);
                if(nTimeDiff <= 0)
                {
                    EventVoteManager::getInstance()->requestInfo(true);
                    uiTitle->stopAllActions();
                    return;
                }
                int h = nTimeDiff / 3600;
                int m = nTimeDiff % 3600 / 60;
                int s = nTimeDiff % 3600 % 60;
                
                if(h < 0)
                    h=0;
                if(m < 0)
                    m=0;
                if(s < 0)
                    s=0;
                
                std::string strTime = MafUtils::format("%02d : %02d : %02d", h, m, s);
                if(EventVoteManager::getInstance()->isBreakTime())
                    strTime = GAME_TEXTFORMAT("t_ui_event_vote_title_6", strTime.c_str());
                else
                    strTime = GAME_TEXTFORMAT("t_ui_event_vote_title_3", strTime.c_str());
                uiTitle->setString(strTime);
            });
            auto seq = Sequence::create(callFunc, DelayTime::create(1), nullptr);
            uiTitle->runAction(RepeatForever::create(seq));
        }
    }
    
}
#pragma mark - schedule

#pragma mark - game
void LayerEventVoteMain::onDrawInfo()
{
    drawVote();
}
