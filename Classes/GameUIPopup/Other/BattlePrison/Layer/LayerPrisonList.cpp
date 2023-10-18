//
//  LayerPrisonList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerPrisonList.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"

#include "ManagerGame/PrisonManager.h"

USING_NS_CC;

LayerPrisonList* LayerPrisonList::create(cocos2d::Size size)
{
    LayerPrisonList* pRet = new(std::nothrow) LayerPrisonList();
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

LayerPrisonList::LayerPrisonList(void) :
_bInitUI(false),

// widget
_uiList(nullptr)

{
    
}

LayerPrisonList::~LayerPrisonList(void)
{
    
}

bool LayerPrisonList::init(cocos2d::Size size)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    // init
    initVar();
    initUi();
    
    //
    _bInitUI = true;

    return true;
}

void LayerPrisonList::onEnter()
{
    ui::Layout::onEnter();
}

void LayerPrisonList::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawList();
    }
}

#pragma mark - init
void LayerPrisonList::initVar()
{
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE )
    {
        _listQuest.clear();
        _listQuest = PrisonManager::getInstance()->getListQuestHell();
    }
    else
    {
        _listQuest.clear();
        _listQuest = PrisonManager::getInstance()->getListQuestDefault();
    }
}

void LayerPrisonList::initUi()
{
    auto size = getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ZERO);
    _uiList->setPosition(Vec2(0, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    addChild(_uiList);
}

#pragma mark - override DelegateListView
void LayerPrisonList::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerPrisonList::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 102;

    return Size(width, height);
}

void LayerPrisonList::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listQuest.at(idx);
    
    std::string title = obj->getTitle();
    bool bReceive= obj->isReceive();
    
    int countNow = (int)obj->getCountNow();
    int countMax = (int)obj->getCountMax();
    
    double percent = (double)obj->getCountNow() / obj->getCountMax() * 100;
    if ( percent > 100 )
        percent = 100;
    
    if ( bReceive == true )
    {
        percent = 100;
    }
    
    bool bPTicket = false;
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::DEFAULT && PrisonManager::getInstance()->getTicketUseRatio() > 1 )
    {
        bPTicket = true;
    }
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);

    // bg
    std::string pathBG = "Assets/ui_common/list_bg_s_2_1.png";
    if ( bReceive == true )
    {
        pathBG = "Assets/ui_common/list_bg_s_2_off.png";
    }
    else if ( bPTicket == true )
    {
        pathBG = "Assets/ui_common/list_bg_s_2_4.png";
    }
    auto uiImgContentBG = ui::ImageView::create(pathBG);
    uiImgContentBG->setContentSize(Size(uiLayout->getContentSize().width - 270, uiLayout->getContentSize().height));
    uiImgContentBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiImgContentBG->setPosition(Vec2(0, uiLayout->getContentSize().height / 2));
    uiImgContentBG->setScale9Enabled(true);
    uiImgContentBG->setCapInsets(Rect(20,60,10,10));
    uiLayout->addChild(uiImgContentBG);
    {
        auto uiTextTitle = ui::CText::create(title, GAME_FONT, 24);
        uiTextTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiTextTitle->setPosition(Vec2(25, uiImgContentBG->getContentSize().height - 23.5));
        uiTextTitle->setTextAreaSize(Size(400, uiTextTitle->getContentSize().height));
        uiTextTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiImgContentBG->addChild(uiTextTitle);
        
        std::string pathReceive = "Assets/ui/costume/checkbox_off.png";
        if ( bReceive == true )
        {
            pathReceive = "Assets/ui/costume/checkbox_on.png";
        }
        auto uiImgReceive = ui::ImageView::create(pathReceive);
        uiImgReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        uiImgReceive->setPosition(Vec2(50, 25));
        uiImgContentBG->addChild(uiImgReceive);

        auto uiImgCountBG = ui::ImageView::create("Assets/ui/prison/quest_gauge_bg_2.png");
        uiImgCountBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiImgCountBG->setPosition(Vec2(50 + 10, 25));
        uiImgContentBG->addChild(uiImgCountBG);
        {
            std::string pathBar = "Assets/ui/prison/quest_gauge_d.png";
            if ( bReceive == true )
            {
                pathBar = "Assets/ui/prison/quest_gauge_e.png";
            }
            auto uiBar = ui::LoadingBar::create(pathBar);
            uiBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            uiBar->setPosition(Vec2(0, 0));
            uiBar->setPercent(percent);
            uiImgCountBG->addChild(uiBar);
            
            std::string count = MafUtils::format("%d/%d", countNow, countMax);
            if ( bReceive == true )
            {
                count = GAME_TEXT("t_ui_complete");
            }
            auto uiTextCount = ui::CText::create(count, GAME_FONT, 20);
            uiTextCount->setPosition(Vec2(uiImgCountBG->getContentSize().width / 2, uiImgCountBG->getContentSize().height / 2));
            uiImgCountBG->addChild(uiTextCount);
        }
    }
    
    
    auto uiImgRewardBG = ui::ImageView::create(pathBG);
    uiImgRewardBG->setContentSize(Size(260, uiLayout->getContentSize().height));
    uiImgRewardBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiImgRewardBG->setPosition(Vec2(uiLayout->getContentSize().width, uiLayout->getContentSize().height / 2));
    uiImgRewardBG->setScale9Enabled(true);
    uiImgRewardBG->setCapInsets(Rect(20,60,10,10));
    uiLayout->addChild(uiImgRewardBG);
    {
        // reward
        auto uiLayoutReward = ui::Layout::create();
        uiLayoutReward->setContentSize(uiImgRewardBG->getContentSize());
        uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayoutReward->setPosition(Vec2(uiImgRewardBG->getContentSize().width / 2, uiImgRewardBG->getContentSize().height / 2));
        uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiImgRewardBG->addChild(uiLayoutReward);
        
        auto listReward = obj->getListReward();
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiLayout = ui::Layout::create();
            uiLayout->setContentSize(Size(100, uiLayoutReward->getContentSize().height));
            uiLayoutReward->addChild(uiLayout);
            {
                auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                uiImgItemIcon->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height - 23.5));
                uiLayout->addChild(uiImgItemIcon);
                
                std::string rewardCount = objItems->getCount();
                std::string rewardCountBonus = objItems->getCount();
                if ( bPTicket == true )
                {
                    double nEvent = (PrisonManager::getInstance()->getTicketUseRatio() - 1.0) * 100;
                    rewardCountBonus = MafUtils::bigMulNum(rewardCountBonus, MafUtils::doubleToString(nEvent));
                    if ( rewardCountBonus.length() > 2 )
                    {
                        rewardCountBonus.pop_back();
                        rewardCountBonus.pop_back();
                    }
                    else
                    {
                        rewardCountBonus = "0";
                    }
                }
                
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                    MafUtils::convertNumberToShort(rewardCountBonus);
                }
                
                if ( bPTicket == true )
                {
                    rewardCountBonus = MafUtils::format("{#green:(+%s)}", rewardCountBonus.c_str());
                    rewardCount.append("\n");
                    rewardCount.append(rewardCountBonus);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiLayout->getContentSize().width / 2, 25));
                uiTextReward->setTextHorizontalAlignment(TextHAlignment::CENTER);
                uiTextReward->setLineSpacing(-10);
                uiLayout->addChild(uiTextReward);
            }
        }
        
        auto size = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
        uiLayoutReward->setContentSize(size);
        uiLayoutReward->setPositionX(uiImgRewardBG->getContentSize().width / 2);
        uiLayoutReward->requestDoLayout();
        
        //
        if ( bPTicket == true )
        {
            auto uiImgUP = ui::ImageView::create("Assets/icon/icon_up.png");
            uiImgUP->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            uiImgUP->setPosition(Vec2(uiImgRewardBG->getContentSize().width + 5, uiImgRewardBG->getContentSize().height + 15));
            uiImgRewardBG->addChild(uiImgUP);
        }
    }
    
     
}

int64_t LayerPrisonList::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listQuest.size();
   
    return count;
}

#pragma mark - ui : defalut

#pragma mark - ui : draw
void LayerPrisonList::drawList()
{
    auto size = getContentSize();
    
    _uiList->setContentSize(size);
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - ui : set, get

#pragma mark - ui : click

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerPrisonList::onDrawList()
{
    //
    if ( PrisonManager::getInstance()->getPrisonType() == PrisonManager::PrisonType::FIRE )
    {
        _listQuest.clear();
        _listQuest = PrisonManager::getInstance()->getListQuestHell();
    }
    else
    {
        _listQuest.clear();
        _listQuest = PrisonManager::getInstance()->getListQuestDefault();
    }
    drawList();
}

