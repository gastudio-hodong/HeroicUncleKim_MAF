//
//  PopupFrostInfo.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFrostInfo.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupFrostInfo* PopupFrostInfo::create()
{
    PopupFrostInfo* pRet = new(std::nothrow) PopupFrostInfo();
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

PopupFrostInfo::PopupFrostInfo() :
_bInitUI(false),

// widget
_uiContentsUIList(nullptr)
{
    
}

PopupFrostInfo::~PopupFrostInfo(void)
{
    
}

bool PopupFrostInfo::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFrostInfo");
    
    //
    initVar();
    initUI();
    
    // event
    
    return true;
}

void PopupFrostInfo::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
    }
    
    //
    drawList();
}

#pragma mark - init
void PopupFrostInfo::initVar()
{
    _listFloor = PrisonFrostManager::getInstance()->getListFloor();
    _listFloor.reverse();
}

void PopupFrostInfo::initUI()
{
    // size
    Size size = Size(656, 922);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_frost_prison_reward_info"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFrostInfo::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

#pragma mark - override DelegateListView
void PopupFrostInfo::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupFrostInfo::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 162 - 15;

    return Size(width, height);
}

void PopupFrostInfo::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listFloor.at(idx);
    
    int floorClear = PrisonFrostManager::getInstance()->getClearFloor();
    int floorNow = obj->getIdx();
    bool bBoss = obj->isBoss();
    bool bClear = floorClear >= obj->getIdx();
    
    int type = 0;
    if ( floorNow <= floorClear )
    {
        type = 2;
    }
    else if ( floorNow == floorClear + 1 )
    {
        type = 1;
    }
    
    auto listReward = ItemsManager::getInstance()->getConvertArray(obj->getReward());
    
    std::string path = "";
    if ( bBoss == true )
    {
        path = UtilsString::getInstance()->format("Assets/ui/frost_prison/tower_boss_%d.png", type);
    }
    else
    {
        path = UtilsString::getInstance()->format("Assets/ui/frost_prison/tower_normal_%d.png", type);
    }
    
    
    // ui
    childview->removeAllChildren();

    // 426x162
    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    // 142x54 3배
    auto uiBG = ui::ImageView::create(path);
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, 0));
    uiBG->setScale(3);
    uiLayout->addChild(uiBG);
    
    // 층수
    auto uiFloor = ui::CText::create( UtilsString::getInstance()->format("%dF", floorNow), GAME_FONT, 20);
    uiFloor->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height - 16));
    uiFloor->enableOutline(Color4B::BLACK, 2);
    uiLayout->addChild(uiFloor);
    
    //
    // reward
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayout->addChild(uiLayoutReward);
    {
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            
            std::string pathRewardBG = "Assets/ui/frost_prison/tower_normal_reward_box.png";
            if ( bBoss == true )
            {
                pathRewardBG = "Assets/ui/frost_prison/tower_boss_reward_box.png";
            }
            
            auto uiRewardBG = ui::ImageView::create(pathRewardBG);
            uiLayoutReward->addChild(uiRewardBG);
            {
                auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 5));
                uiRewardBG->addChild(uiImgItemIcon);
                
                std::string rewardCount = objItems->getCount();
                std::string rewardCountBonus = objItems->getCount();
                 
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                    MafUtils::convertNumberToShort(rewardCountBonus);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiRewardBG->addChild(uiTextReward);
            }
            
            if ( bClear == true )
            {
                auto uiRewardClearBG = ui::ImageView::create("Assets/ui/frost_prison/tower_reward_box_black.png");
                uiRewardClearBG->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2));
                uiRewardBG->addChild(uiRewardClearBG);
                
                auto uiRewardClearCheck = ui::ImageView::create("Assets/ui/return_event/return_reward_check.png");
                uiRewardClearCheck->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2));
                uiRewardBG->addChild(uiRewardClearCheck);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
        uiLayoutReward->setContentSize(sizeReward);
        uiLayoutReward->setPosition(Vec2(uiLayout->getContentSize().width / 2, 30));
        uiLayoutReward->requestDoLayout();
    }
    
    
}

int64_t PopupFrostInfo::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listFloor.size();
   
    return count;
}

#pragma mark - ui
void PopupFrostInfo::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 14;
    size.height = size.height - 75;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize( Size(648, 844) );
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _uiContentsUIList->setPosition( Vec2(size.width / 2, size.height / 2) );
    layout->addChild(_uiContentsUIList);
}

void PopupFrostInfo::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    // 162x211 4배
    auto uiBG = ui::ImageView::create("Assets/ui/frost_prison/tower_info_bg.png");
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBG->setScale(4);
    layout->addChild(uiBG);
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    _uiList->setContainerOffset(Vec2::ZERO);
    _uiList->setTouchEnabled(false);
    layout->addChild(_uiList);
}
          
#pragma mark - draw
void PopupFrostInfo::drawList()
{
    _uiList->reloadData();
    
    int floorClear = PrisonFrostManager::getInstance()->getClearFloor();
    int idx = -1;
    for ( int i = 0; i < _listFloor.size(); i++ )
    {
        auto obj = _listFloor.at(i);
        if ( floorClear == obj->getIdx() )
        {
            idx = i;
            break;
        }
    }
    
    if ( idx == -1 )
    {
        idx = (int)(_listFloor.size() - 1);
    }
    _uiList->jumpToItem(idx, Vec2::ANCHOR_MIDDLE_BOTTOM, Vec2::ANCHOR_MIDDLE_BOTTOM);
    
}
 
#pragma mark - ui : set, get

#pragma mark - callback

#pragma mark - click
void PopupFrostInfo::onClickClose(Ref* sender)
{
    hide();
}

#pragma mark - game
