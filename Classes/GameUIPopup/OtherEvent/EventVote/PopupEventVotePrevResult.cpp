//
//  PopupEventVotePrevResult.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/29.
//

#include "PopupEventVotePrevResult.h"

#include "GameObject/InfoItems.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerEvent/EventVoteManager.h"

USING_NS_CC;

bool PopupEventVotePrevResult::sortVoteMinority(InfoEventVoteCandidate *a, InfoEventVoteCandidate *b)
{
    if(a->getPersnalVoteAllCount() != b->getPersnalVoteAllCount())
        return a->getPersnalVoteAllCount() < b->getPersnalVoteAllCount();
    
    return a->getIdx() > b->getIdx();
}


PopupEventVotePrevResult* PopupEventVotePrevResult::create()
{
    PopupEventVotePrevResult* pRet = new(std::nothrow) PopupEventVotePrevResult();
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

PopupEventVotePrevResult::PopupEventVotePrevResult():
_bInitUI(false)
,_uiList(nullptr)
{
    _listInfoPrevVoteCandidate.clear();
}

PopupEventVotePrevResult::~PopupEventVotePrevResult(void)
{
    _listInfoPrevVoteCandidate.clear();
}
bool PopupEventVotePrevResult::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    _bHideBackKey = false;
    _listInfoPrevVoteCandidate = EventVoteManager::getInstance()->getListPrevVoteCandidate();
    std::sort(_listInfoPrevVoteCandidate.begin(), _listInfoPrevVoteCandidate.end(), PopupEventVotePrevResult::sortVoteMinority);
    initVar();
    initUI();
    return true;
}

void PopupEventVotePrevResult::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
    }
    
    drawList();
    
}
#pragma mark - override DelegateListView
void PopupEventVotePrevResult::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:
            break;
        default:
            break;
    }
}

Size PopupEventVotePrevResult::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 80;

    return Size(width, height);
}

void PopupEventVotePrevResult::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    
    InfoEventVoteCandidate* info = _listInfoPrevVoteCandidate.at(idx);
    if(info == nullptr)
        return;
    
    bool isFirst = idx == 0;
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(470, 64));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setIgnoreAnchorPointForPosition(false);
    uiLayout->setPosition(size/2);
    childview->addChild(uiLayout);
    
    std::string imagePath = "Assets/ui/event/minority_event_listbox.png";
    
    auto uiBox = ui::ImageView::create(imagePath);
    uiBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBox->setPosition(uiLayout->getContentSize()/2);
    uiBox->setScale(2);
    ((ui::Scale9Sprite*)uiBox->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiLayout->addChild(uiBox);
    
    std::string strTitle = "No." + std::to_string(info->getIdx());
    
    auto tIdx = ui::CText::create(strTitle, GAME_FONT_AL, 30);
    tIdx->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    tIdx->setPosition(Vec2(44, uiLayout->getContentSize().height/2));
    tIdx->setTextAreaSize(Size(88, 64));
    tIdx->setTextOverflow(Label::Overflow::SHRINK);
    tIdx->setTextVerticalAlignment(TextVAlignment::CENTER);
    tIdx->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiLayout->addChild(tIdx);
    
    if(isFirst)
    {
        uiLayout->setClippingEnabled(false);
        
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.1);
        for ( int i = 0; i <= 5; i++ )
        {
            auto spriteAni = Sprite::create(MafUtils::format("Assets/ui/event/minority_event_seal%02d.png",i).c_str());
            spriteAni->getTexture()->setTexParameters(texParams);
            animation->addSpriteFrame(spriteAni->getSpriteFrame());
        }
        auto uiBedge = ui::ImageView::create("Assets/ui/event/minority_event_seal00.png");
        uiBedge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiBedge->setPosition(Vec2(0, uiLayout->getContentSize().height));
        uiBedge->getVirtualRenderer()->stopAllActions();
        uiBedge->getVirtualRenderer()->runAction(RepeatForever::create(Animate::create(animation)));
        uiLayout->addChild(uiBedge);
        
        auto uiEdge = ui::ImageView::create("Assets/ui/event/minority_event_listbox_best.png");
        uiEdge->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiEdge->setPosition(uiBox->getContentSize()/2);
        ((ui::Scale9Sprite*)uiBox->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiBox->addChild(uiEdge);
        
        tIdx->setColor(Color3B(250,225,5));
    }
    
    //리워드
    auto uiRewardLayout = ui::Layout::create();
    uiRewardLayout->setContentSize(Size(245, uiLayout->getContentSize().height));
    uiRewardLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiRewardLayout->setIgnoreAnchorPointForPosition(false);
    uiRewardLayout->setPosition(Vec2(110, uiLayout->getContentSize().height/2));
    uiLayout->addChild(uiRewardLayout);
    if(auto objItem = info->getReward(); objItem != nullptr)
    {
        if (auto uiItemIcon = ui::ImageView::create(objItem->getPath()); uiItemIcon != nullptr )
        {
            uiItemIcon->setPosition(Vec2(30, uiRewardLayout->getContentSize().height / 2));
            uiItemIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiRewardLayout->addChild(uiItemIcon);
            
            std::string strItemsCount = "";
            
            if((E_ITEMS)objItem->getIdx() != E_ITEMS::COSTUME_SKIN)
            {
                strItemsCount = "x" + objItem->getCount();
                if (objItem->isString())
                    MafUtils::convertNumberToShort(strItemsCount);
            }
                
            auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 30);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiCount->setPosition(uiItemIcon->getPosition() + Vec2::RIGHT * uiItemIcon->getContentSize().width);
            uiCount->setTextAreaSize(Size(150, uiCount->getContentSize().height));
            uiCount->setTextOverflow(Label::Overflow::SHRINK);
            uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
            uiRewardLayout->addChild(uiCount);
        }
    }
    
    //득표율
    auto uiPersent = ui::CText::create(MafUtils::format("%.2f%%", getPersent(info)), GAME_FONT, 26);
    uiPersent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiPersent->setPosition(Vec2(uiLayout->getContentSize().width - 50, uiLayout->getContentSize().height/2));
    uiPersent->setTextAreaSize(Size(90, uiPersent->getContentSize().height));
    uiPersent->setTextOverflow(Label::Overflow::SHRINK);
    uiPersent->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiPersent->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiLayout->addChild(uiPersent);
    
}

int64_t PopupEventVotePrevResult::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoPrevVoteCandidate.size();
    return count;
}
#pragma mark - ui
void PopupEventVotePrevResult::initVar()
{
    double nWidth = 520;
    double nHeight = 590;
    
    // size
    _spriteContainer->setContentSize( Size(nWidth, nHeight) );
}

void PopupEventVotePrevResult::initUI()
{
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_event_vote_title_4"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
}

void PopupEventVotePrevResult::uiContainer()
{
    auto layout = ui::Layout::create();
    layout->setContentSize(_spriteContainer->getContentSize() - Size(10, 75));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setClippingEnabled(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, layout->getContentSize() - Size(0, 110), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiList->setClippingEnabled(false);
    _uiList->setPosition(Vec2(layout->getContentSize().width / 2, layout->getContentSize().height - 10));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
    
    auto uiBtnClose = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBtnClose->setScale9Enabled(true);
    uiBtnClose->setContentSize(Size(164, 64));
    uiBtnClose->setPositionType(ui::Widget::PositionType::PERCENT);
    uiBtnClose->setPositionPercent(UtilsWidget::getInstance()->getSizeToPercent(size, size.width/2, 25));
    uiBtnClose->addClickEventListener(CC_CALLBACK_1(PopupEventVotePrevResult::onClickClose, this));
    uiBtnClose->setTitleText(GAME_TEXT("t_ui_confirm"));
    uiBtnClose->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    uiBtnClose->getTitleLabel()->setDimensions(uiBtnClose->getContentSize().width, uiBtnClose->getContentSize().height);
    uiBtnClose->setTitleFontSize(25);
    layout->addChild(uiBtnClose);
}

#pragma mark - draw
void PopupEventVotePrevResult::drawList()
{
    _listInfoPrevVoteCandidate = EventVoteManager::getInstance()->getListPrevVoteCandidate();
    std::sort(_listInfoPrevVoteCandidate.begin(), _listInfoPrevVoteCandidate.end(), PopupEventVotePrevResult::sortVoteMinority);
    
    _uiList->reloadData();
}

#pragma mark - click
void PopupEventVotePrevResult::onClickClose(Ref* sender)
{
    hide();
}
#pragma mark - Utils
const float PopupEventVotePrevResult::getPersent(InfoEventVoteCandidate* candidate)
{
    return EventVoteManager::getInstance()->getPercentageOfVotes(candidate, getPrevVoteAllCount());
}
const int PopupEventVotePrevResult::getPrevVoteAllCount()
{
    int result = 0;
    
    for(auto obj : _listInfoPrevVoteCandidate)
        result += obj->getPersnalVoteAllCount();
    
    return result;
}
