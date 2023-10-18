//
//  PopupEventVoting.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/28.
//

#include "PopupEventVoting.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerEvent/EventVoteManager.h"

USING_NS_CC;

PopupEventVoting* PopupEventVoting::create(const int nIdx)
{
    PopupEventVoting* pRet = new(std::nothrow) PopupEventVoting();
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

PopupEventVoting::PopupEventVoting():
_bInitUI(false)
,_nIdx(0)
,_nCount(0)
,_uiTop(nullptr)
,_uiVote(nullptr)
{
}

PopupEventVoting::~PopupEventVoting(void)
{
}
bool PopupEventVoting::init(const int nIdx)
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    _nIdx = nIdx;
    initVar();
    initUI();
    return true;
}

void PopupEventVoting::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiTop();
        uiVote();
    }
    
    drawVote();
}

#pragma mark - ui
void PopupEventVoting::initVar()
{
    double nWidth = 516;
    double nHeight = 508;
    
    // size
    _spriteContainer->setContentSize( Size(nWidth, nHeight) );
}

void PopupEventVoting::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_vote_popup_title_1"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
}

void PopupEventVoting::uiContainer()
{
    auto layout = ui::Layout::create();
    layout->setContentSize(_spriteContainer->getContentSize() - Size(10, 75));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    _uiTop = ui::Layout::create();
    _uiTop->setContentSize(Size(layout->getContentSize().width, 148));
    _uiTop->setIgnoreAnchorPointForPosition(false);
    _uiTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiTop->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height));
    _uiTop->setClippingEnabled(false);
    layout->addChild(_uiTop);
    
    _uiVote =  ui::Layout::create();
    _uiVote->setContentSize(Size(layout->getContentSize().width, layout->getContentSize().height - _uiTop->getContentSize().height));
    _uiVote->setIgnoreAnchorPointForPosition(false);
    _uiVote->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiVote->setClippingEnabled(false);
    _uiVote->setPosition( _uiTop->getPosition() + Vec2::DOWN * _uiTop->getContentSize().height);
    layout->addChild(_uiVote);
    
}

void PopupEventVoting::uiTop()
{
    auto layout = _uiTop;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
    uiBG->setScale9Enabled(true);
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBG->setPosition(layout->getContentSize()/2);
    uiBG->setContentSize(Size(508, 148));
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_vote_popup_msg_1", _nIdx), GAME_FONT, 24);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiText->setPosition(Vec2(size/2));
    uiText->setTextAreaSize(Size(size.width - 10, size.height/2));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::TOP);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText);
}
void PopupEventVoting::uiVote()
{
    auto layout = _uiVote;
    Size size = layout->getContentSize();
    
    auto uiVoteCoin = ui::ImageView::create("Assets/icon/icon_minority_coin.png");
    uiVoteCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiVoteCoin->setScale9Enabled(true);
    uiVoteCoin->setPosition(Vec2(layout->getContentSize().width/2, 215));
    layout->addChild(uiVoteCoin);
    {
        auto uiCount = ui::CText::create(MafUtils::format("x%d", EventVoteManager::getInstance()->getVoteCoinCount()), GAME_FONT_AL, 28);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCount->setPosition(uiVoteCoin->getPosition());
        uiCount->setTextAreaSize(Size(size.width/2, uiCount->getContentSize().height));
        uiCount->setTextOverflow(Label::Overflow::SHRINK);
        uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiCount->setName("UI_COIN_COUNT");
        layout->addChild(uiCount);
    }

    
    //카운트
    auto uiBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBG->setPosition(Vec2(layout->getContentSize().width/2, 144));
    layout->addChild(uiBG);
    
    auto uiCount = ui::CText::create(std::to_string(_nCount), GAME_FONT, 24);
    uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCount->setPosition(Vec2(uiBG->getContentSize()/2));
    uiCount->setTextAreaSize(Size(uiBG->getContentSize().width - 10, uiBG->getContentSize().height - 10));
    uiCount->setTextOverflow(Label::Overflow::SHRINK);
    uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiCount->setName("UI_COUNT");
    uiBG->addChild(uiCount);
    
    //증감
    auto btnPlus = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
    btnPlus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPlus->setPosition(uiBG->getPosition() + Vec2::RIGHT * (uiBG->getContentSize().width/2 + 15));
    btnPlus->setZoomScale(0.05f);
    btnPlus->setName("PLUS");
    btnPlus->addClickEventListener(CC_CALLBACK_1(PopupEventVoting::onClickIncrease, this));
    layout->addChild(btnPlus);
    
    auto btnMinus = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_l.png");
    btnMinus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnMinus->setPosition(uiBG->getPosition() + Vec2::LEFT * (uiBG->getContentSize().width/2 + 15));
    btnMinus->setZoomScale(0.05f);
    btnMinus->setName("MINUS");
    btnMinus->addClickEventListener(CC_CALLBACK_1(PopupEventVoting::onClickIncrease, this));
    layout->addChild(btnMinus);
    
    //투표 버튼
    auto uiBtnVote = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnVote->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnVote->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnVote->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(layout->getContentSize(), size.width/2 - 10, 20));
    uiBtnVote->addClickEventListener(CC_CALLBACK_1(PopupEventVoting::onClickVote, this));
    layout->addChild(uiBtnVote);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_yes"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiText->setPosition(uiBtnVote->getContentSize()/2);
        uiText->setTextAreaSize(Size(uiBtnVote->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnVote->addChild(uiText);
    }
    
    //취소 버튼
    auto uiBtnClose = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnClose->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiBtnClose->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnClose->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(layout->getContentSize(), size.width/2 + 10, 20));
    uiBtnClose->addClickEventListener(CC_CALLBACK_1(PopupEventVoting::onClickClose, this));
    layout->addChild(uiBtnClose);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_no"), GAME_FONT, 20);
        uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiText->setPosition(uiBtnClose->getContentSize()/2);
        uiText->setTextAreaSize(Size(uiBtnClose->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtnClose->addChild(uiText);
    }
}
#pragma mark - draw
void PopupEventVoting::drawVote()
{
    auto layout = _uiVote;
    
    // floor
    if(auto uiCount = (ui::CText*)utils::findChild(_uiVote, "UI_COUNT"); uiCount != nullptr)
    {
        uiCount->setString(std::to_string(_nCount));
    }
}

#pragma mark - click
void PopupEventVoting::onClickClose(Ref* sender)
{
    hide();
}
void PopupEventVoting::onClickIncrease(Ref* sender)
{
    auto obj = (Node*) sender;
    
    if(obj->getName() == "PLUS")
    {
        _nCount++;
        if(_nCount > EventVoteManager::getInstance()->getVoteCoinCount())
            _nCount = 0;
    }
    else if(obj->getName() == "MINUS")
    {
        _nCount--;
        if(_nCount < 0)
            _nCount = EventVoteManager::getInstance()->getVoteCoinCount();
    }
    
    drawVote();
}
void PopupEventVoting::onClickVote(Ref* sender)
{
    if(_nCount <= 0) return;
    
    EventVoteManager::getInstance()->requestVoting(true, _nIdx, _nCount);
    hide();
}

#pragma mark - callback
