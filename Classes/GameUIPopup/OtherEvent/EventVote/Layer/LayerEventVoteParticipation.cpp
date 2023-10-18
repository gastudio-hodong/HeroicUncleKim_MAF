//
//  LayerEventVoteParticipation.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "LayerEventVoteParticipation.h"

#include "GameObject/InfoItems.h"

#include "ManagerEvent/EventVoteManager.h"

USING_NS_CC;

LayerEventVoteParticipation* LayerEventVoteParticipation::create(cocos2d::Size size)
{
    LayerEventVoteParticipation* pRet = new(std::nothrow) LayerEventVoteParticipation();
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

LayerEventVoteParticipation::LayerEventVoteParticipation(void) :
_bInitUI(false),

_bResult(false),

_uiContentsLobby(nullptr)
{
    _listInfoVoteAll.clear();
}

LayerEventVoteParticipation::~LayerEventVoteParticipation(void)
{
    
}

bool LayerEventVoteParticipation::init(cocos2d::Size size)
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
    
    // init
    initVar();
    initUi();
    
    return true;
}

void LayerEventVoteParticipation::onEnter()
{
    ui::Layout::onEnter();
    
    _listInfoVoteAll = EventVoteManager::getInstance()->getListVoteAll();
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        uiContainer();
        uiParticipation();
    }
    
    //
    drawList();
}

void LayerEventVoteParticipation::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawList();
    }
}

#pragma mark - init
void LayerEventVoteParticipation::initVar()
{
    
}

void LayerEventVoteParticipation::initUi()
{
    
}

#pragma mark - override DelegateListView
void LayerEventVoteParticipation::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerEventVoteParticipation::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = 66;
    double height = 280;

    return Size(width, height);
}

void LayerEventVoteParticipation::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    
    InfoEventVoteAll* info = _listInfoVoteAll.at(idx);
    if(info == nullptr)
        return;
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    std::string imagePath = "Assets/ui/event/participate_num_gauge1_bg.png";
    Vec2 anchor = Vec2::ZERO;
    Vec2 pos = Vec2::ZERO;
    bool isEvenNumber = false;
    bool isCompleteCount = EventVoteManager::getInstance()->getAllVoteCount() >= info->getCount();
    if((idx + 1) % 2 == 0)
        isEvenNumber = true;
    
    if(isCompleteCount)
        imagePath = "Assets/ui/event/participate_num_gauge1_green.png";
    
    //프로그레스
    if(isEvenNumber)
    {
        if(isCompleteCount)
            imagePath = "Assets/ui/event/participate_num_gauge2_green.png";
        else
            imagePath = "Assets/ui/event/participate_num_gauge2_bg.png";
    }
    auto uiProgress = ui::ImageView::create(imagePath);
    uiProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiProgress->setPosition(Vec2(0, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiProgress);
    
    //카운트
    imagePath = "Assets/ui/event/participate_num_box1_gray.png";
    if(isCompleteCount)
        imagePath = "Assets/ui/event/participate_num_box1.png";
    anchor = Vec2::ANCHOR_MIDDLE_BOTTOM;
    pos = Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height / 2 + 10);
    if(isEvenNumber)
    {
        imagePath = "Assets/ui/event/participate_num_box2_gray.png";
        if(isCompleteCount)
            imagePath = "Assets/ui/event/participate_num_box2.png";
        anchor = Vec2::ANCHOR_MIDDLE_TOP;
        pos = Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height / 2 - 10);
    }
    auto uiCountBox = ui::ImageView::create(imagePath);
    uiCountBox->setAnchorPoint(anchor);
    uiCountBox->setPosition(pos);
    uiLayout->addChild(uiCountBox);
    {
        pos = Vec2(uiCountBox->getContentSize().width/2, 22);
        if(isEvenNumber)
            pos = Vec2(uiCountBox->getContentSize().width/2, 15);
            
        auto uiCount = ui::CText::create(std::to_string(info->getCount()), GAME_FONT_AL, 22);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiCount->setPosition(pos);
        uiCount->setTextAreaSize(Size(uiCountBox->getContentSize().width - 15, 20));
        uiCount->setTextOverflow(Label::Overflow::SHRINK);
        uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiCount->setColor(Color3B(95,105,105));
        if(isCompleteCount)
            uiCount->setColor(Color3B(200,255,250));
        uiCountBox->addChild(uiCount);
    }
    
    //리워드
    imagePath = "Assets/ui/return_event/return_reward_box.png";
    anchor = Vec2::ANCHOR_MIDDLE_BOTTOM;
    pos = Vec2(uiLayout->getContentSize().width, 9);
    if(isEvenNumber)
    {
        anchor = Vec2::ANCHOR_MIDDLE_TOP;
        pos = Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height - 9);
    }
    auto uiRewardBox = ui::ImageView::create(imagePath);
    uiRewardBox->setAnchorPoint(anchor);
    uiRewardBox->setPosition(pos);
    uiLayout->addChild(uiRewardBox);
    {
        if(isCompleteCount)
        {
            auto uiEdge = ui::ImageView::create("Assets/ui/event/minority_reward_box_edge.png");
            uiEdge->setAnchorPoint(Vec2::ZERO);
            uiEdge->setPosition(Vec2::ZERO);
            uiRewardBox->addChild(uiEdge);
        }
        auto objItem = info->getReward().at(0);
        if(objItem == nullptr)
            return;
        
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

            auto uiCount = ui::CText::create(strItemsCount, GAME_FONT, 16);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            uiCount->setPosition(Vec2(uiRewardBox->getContentSize().width / 2, 0));
            uiCount->setTextAreaSize(Size(uiRewardBox->getContentSize().width - 10, uiCount->getContentSize().height));
            uiCount->setTextOverflow(Label::Overflow::SHRINK);
            uiCount->setTextVerticalAlignment(TextVAlignment::BOTTOM);
            uiCount->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiRewardBox->addChild(uiCount);
        }
        
    }
}

int64_t LayerEventVoteParticipation::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoVoteAll.size();
    return count;
}

#pragma mark - ui
void LayerEventVoteParticipation::uiContainer()
{
    auto size = getContentSize();
    
    // ui default
    _uiContentsLobby = ui::Layout::create();
    _uiContentsLobby->setContentSize(size);
    _uiContentsLobby->setAnchorPoint(Vec2::ZERO);
    _uiContentsLobby->setPosition(Vec2::ZERO);
    addChild(_uiContentsLobby);
    
    _uiList = ui::CListView::create(this, Size(size.width - 40, 280), ui::ScrollView::Direction::HORIZONTAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 25));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    addChild(_uiList);
}

void LayerEventVoteParticipation::uiParticipation()
{
    auto layout = _uiContentsLobby;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/box_04.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiBG->setPosition(size/2);
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size - Size(20, 0));
    layout->addChild(uiBG);
    
    //타이틀
    auto uiTitleBox = ui::ImageView::create("Assets/ui/costume/costume_name_box.png");
    uiTitleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiTitleBox->setPosition(Vec2(size.width/2, size.height));
    layout->addChild(uiTitleBox);
    {
        auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_vote_title_2"), GAME_FONT, 24);
        uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiTitle->setPosition(Vec2(uiTitleBox->getContentSize()/2));
        uiTitle->setTextAreaSize(Size(uiTitleBox->getContentSize().width - 40, uiTitle->getContentSize().height));
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitleBox->addChild(uiTitle);
    }
    
}

#pragma mark - ui : draw
void LayerEventVoteParticipation::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - ui : set, get

#pragma mark - ui : click


#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerEventVoteParticipation::onDrawInfo()
{
    drawList();
}
