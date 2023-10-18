//
//  PopupEventRaceRankReward.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/08/08.
//

#include "PopupFlashmanRankRewardInfo.h"

#include "GameObject/InfoRank.h"
#include "GameObject/InfoItems.h"

#include "ManagerGame/FlashmanManager.h"

USING_NS_CC;

PopupFlashmanRankRewardInfo* PopupFlashmanRankRewardInfo::create()
{
    PopupFlashmanRankRewardInfo* pRet = new(std::nothrow) PopupFlashmanRankRewardInfo();
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

PopupFlashmanRankRewardInfo::PopupFlashmanRankRewardInfo():
_bInitUI(false),

//
_uiContentsUIDefault(nullptr),
_uiContentsUIList(nullptr),
_uiContentsUIButtons(nullptr),

_uiList(nullptr)
{
    
}

PopupFlashmanRankRewardInfo::~PopupFlashmanRankRewardInfo(void)
{
    
}
bool PopupFlashmanRankRewardInfo::init()
{
    if (!PopupBase::init(PopupBaseType::TRANSPARENT))
    {
        return false;
    }
    
    //
    setName("PopupFlashmanRankRewardInfo");
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupFlashmanRankRewardInfo::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiDefault();
        uiList();
        uiButtons();
    }
    
    //
    drawList();
}
 

#pragma mark - override DelegateListView
void PopupFlashmanRankRewardInfo::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupFlashmanRankRewardInfo::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 75;

    return Size(width, height);
}

void PopupFlashmanRankRewardInfo::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, size.height - 11);

    // data
    auto obj = _listInfoRank.at(idx);
     
    int condition1 = (int)obj->getCondition1();
    int condition2 = (int)obj->getCondition2();
    auto pathIcon = MafUtils::format("Assets/ui/race_event/race_reward_rank_no%d.png", condition1);
    
    auto listReward = obj->getListReward();
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui/race_event/race_reward_rank_listbox.png");
    uiBG->setPosition(Vec2(sizeLayout.width / 2, sizeLayout.height / 2));
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiBG->setScale(4);
    uiLayout->addChild(uiBG);
   
    
    // 160 64
    if ( FileUtils::getInstance()->isFileExist(pathIcon) == true )
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(80, sizeLayout.height / 2));
        uiLayout->addChild(uiIcon);
    }
    else
    {
        std::string conditionText = MafUtils::format("%d ~ %d", condition1, condition2);
        if( obj->getIdx() >= _listInfoRank.size() )
        {
            conditionText = MafUtils::format("%d ~", condition1);
        }
        
        auto uiCondition = ui::CText::create(conditionText, GAME_FONT, 28);
        uiCondition->setPosition(Vec2(80, sizeLayout.height / 2));
        uiCondition->setTextAreaSize(Size(150, uiCondition->getContentSize().height));
        uiCondition->setTextOverflow(Label::Overflow::SHRINK);
        uiCondition->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCondition->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiCondition->setTextColor(Color4B(166, 193, 207, 255));
        uiLayout->addChild(uiCondition);
    }
    
    // reward
    auto uiLayoutReward = ui::Layout::create();
    uiLayoutReward->setContentSize(Size(sizeLayout.width - 160, sizeLayout.height));
    uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayoutReward->setPosition(Vec2(sizeLayout.width - (sizeLayout.width - 160) / 2, sizeLayout.height / 2));
    uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiLayout->addChild(uiLayoutReward);
    {
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItems = listReward.at(i);
            
            auto uiItemBG = ui::Layout::create();
            uiItemBG->setContentSize(Size(130, 64));
            uiLayoutReward->addChild(uiItemBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(20, 0, 0, 0));
                    uiItemBG->setLayoutParameter(params);
                }
                
                std::string rewardCount = objItems->getCount();
                if ( objItems->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                rewardCount.insert(0, "x");
                
                auto uiItem = UtilsWidget::getInstance()->createIconText(objItems->getPath(), rewardCount, 130, 30, 5);
                uiItem->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiItem);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
        uiLayoutReward->setContentSize(sizeReward);
        uiLayoutReward->requestDoLayout();
        
    }
        
}

int64_t PopupFlashmanRankRewardInfo::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoRank.size();
    return count;
}

#pragma mark - PopupEventRaceRankReward
void PopupFlashmanRankRewardInfo::initVar()
{
    _listInfoRank = FlashmanManager::getInstance()->getListInfoRank();
}

void PopupFlashmanRankRewardInfo::initUI()
{
    // size
    Size size = Size(630, 1030);

    _spriteContainer = ui::Scale9Sprite::create(PIXEL, Rect::ZERO);
    _spriteContainer->setContentSize(size);
    _spriteContainer->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
    _spriteContainer->setOpacity(0);
    addChild(_spriteContainer);
    {
        auto uiBG = ui::ImageView::create("Assets/ui/race_event/race_reward_rank_popupbox.png");
        uiBG->setScale9Enabled(true);
        uiBG->setCapInsets(Rect(9, 9, 15, 15));
        uiBG->setContentSize(size);
        uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
        _spriteContainer->addChild(uiBG);
    }
    
    auto uiTitleBG = ui::ImageView::create("Assets/ui/race_event/race_reward_title.png");
    uiTitleBG->setScale9Enabled(true);
    uiTitleBG->setCapInsets(Rect(67, 0, 22, 40));
    uiTitleBG->setContentSize(Size(size.width - 10, uiTitleBG->getContentSize().height));
    uiTitleBG->setPosition(Size(size.width / 2, size.height - 4 - 37.5));
    _spriteContainer->addChild(uiTitleBG);
    
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_raidreward_icon"), GAME_FONT, 30);
    uiTitle->setPosition(Size(size.width / 2, size.height - 4 - 37.5));
    uiTitle->setTextAreaSize(Size(450, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(255, 210, 28, 255));
    _spriteContainer->addChild(uiTitle);
}

#pragma mark - ui
void PopupFlashmanRankRewardInfo::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width -= 38;
    size.height -= 79;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, 100) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height - 10));
    layout->addChild(_uiContentsUIDefault);
    
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width, 33 + 33 + 84) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize( Size(size.width, size.height - 10 - _uiContentsUIDefault->getContentSize().height - 10 - _uiContentsUIButtons->getContentSize().height ) );
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIList->setPosition( Vec2(size.width / 2, size.height - 120) );
    layout->addChild(_uiContentsUIList);
    
    
}

void PopupFlashmanRankRewardInfo::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    auto size = layout->getContentSize();
    
    auto uiBanner = ui::ImageView::create("Assets/ui/flashman/bsbs_reward_rank_top.png");
    uiBanner->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBanner);
}

void PopupFlashmanRankRewardInfo::uiList()
{
    auto layout = _uiContentsUIList;
    auto size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void PopupFlashmanRankRewardInfo::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    auto size = layout->getContentSize();
    
    auto uiBtnBuyItem = ui::Button::create("Assets/ui/race_event/race_btn_big_blue.png");
    uiBtnBuyItem->setScale9Enabled(true);
    uiBtnBuyItem->setCapInsets(Rect(11, 10, 65, 65));
    uiBtnBuyItem->setContentSize(Size(200, 84));
    uiBtnBuyItem->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBtnBuyItem->addClickEventListener(CC_CALLBACK_1(PopupFlashmanRankRewardInfo::onClickClose, this));
    layout->addChild(uiBtnBuyItem);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_confirm"), GAME_FONT, 30);
        uiText->setPosition(Vec2(uiBtnBuyItem->getContentSize().width / 2, uiBtnBuyItem->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBtnBuyItem->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->enableOutline(Color4B::BLACK, 2);
        uiBtnBuyItem->addChild(uiText);
    }
    
}

#pragma mark - ui : draw
void PopupFlashmanRankRewardInfo::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - Utils
void PopupFlashmanRankRewardInfo::onClickClose(Ref* sender)
{
    hide();
}

 
