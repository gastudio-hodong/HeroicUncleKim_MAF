//
//  PopupFlashmanMission.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFlashmanMission.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoQuest.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/FlashmanManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/SaveManager.h"

USING_NS_CC;

PopupFlashmanMission* PopupFlashmanMission::create()
{
    PopupFlashmanMission* pRet = new(std::nothrow) PopupFlashmanMission();
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

PopupFlashmanMission::PopupFlashmanMission() :
_bInitUI(false),

//

// widget
_uiContentsUIDefault(nullptr),
_uiContentsUIList(nullptr),
_uiContentsUIButtons(nullptr),

_uiList(nullptr)
{
    
}

PopupFlashmanMission::~PopupFlashmanMission(void)
{
    
}

bool PopupFlashmanMission::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFlashmanMission");
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupFlashmanMission::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        uiContainer();
        uiDefault();
        uiList();
        uiButtons();
        
        //
        _bInitUI = true;
    }
    
    //
    drawDefault();
    drawList();
    drawButtons();
}

#pragma mark - override DelegateListView
void PopupFlashmanMission::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupFlashmanMission::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 105;

    return Size(width, height);
}

void PopupFlashmanMission::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, size.height - 2);

    // data
    auto obj = _listInfoMission.at(idx);
     
    int count = FlashmanManager::getInstance()->getMissionCount(obj->getIdx());
    int countMax = (int)obj->getCountMax();
    
    bool bTicket = FlashmanManager::getInstance()->isTicket();
    
    bool bReceive =  FlashmanManager::getInstance()->getMissionReceive(obj->getIdx());
    
    bool bComplete = false;
    if ( bReceive == false && count >= countMax )
        bComplete = true;
    
    std::string titleText = obj->getTitle();
    std::string countText = MafUtils::format("%d/%d", count, countMax);
    std::string pathCheckBox = "Assets/ui_common/checkbox_1_off.png";
    if ( bReceive == true )
        pathCheckBox = "Assets/ui_common/checkbox_1_on.png";
    
    //progress
    double percent = (double)count / countMax * 100;
    if ( percent > 100 )
        percent = 100;
    
    auto listReward = obj->getListReward();
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    uiLayout->setTouchEnabled(true);
    uiLayout->addClickEventListener(CC_CALLBACK_1(PopupFlashmanMission::onClickReward, this));
    uiLayout->setTag((int)idx);
    childview->addChild(uiLayout);
    
    
    // reward default
    auto uiRewardBG = ui::ImageView::create("Assets/ui_common/list_bg_s_2_1.png");
    uiRewardBG->setScale9Enabled(true);
    uiRewardBG->setCapInsets(Rect(25,50,1,1));
    uiRewardBG->setContentSize(Size(490, sizeLayout.height));
    uiRewardBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiRewardBG->setPosition(Vec2(0, sizeLayout.height / 2));
    uiLayout->addChild(uiRewardBG);
    {
        //322, 168
        //text
        auto uiName = ui::CText::create(titleText, GAME_FONT, 24);
        uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiName->setPosition(Vec2(15, uiRewardBG->getContentSize().height - 23));
        uiName->setTextAreaSize(Size(307, uiName->getContentSize().height));
        uiName->setTextOverflow(Label::Overflow::SHRINK);
        uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiName->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiRewardBG->addChild(uiName);
        
        //checkbox
        auto uiCheckBox = ui::ImageView::create(pathCheckBox);
        uiCheckBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCheckBox->setPosition(Vec2(15, 28.5));
        uiRewardBG->addChild(uiCheckBox);
        
        auto uiCountBarBG = ui::ImageView::create("Assets/ui_common/progressbar_01_1.png");
        uiCountBarBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCountBarBG->setPosition(Vec2(uiCheckBox->getPositionX() + uiCheckBox->getContentSize().width + 10, 28.5));
        uiRewardBG->addChild(uiCountBarBG);
    
        auto uiCountBar = ui::LoadingBar::create("Assets/ui_common/progressbar_01_2.png");
        uiCountBar->setPercent(percent);
        uiCountBar->setPositionType(ui::Widget::PositionType::PERCENT);
        uiCountBar->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiCountBarBG->addChild(uiCountBar);
          
        auto uiCount = ui::CText::create(countText, GAME_FONT, 16);
        uiCount->setPositionType(ui::Widget::PositionType::PERCENT);
        uiCount->setPositionPercent(Vec2::ANCHOR_MIDDLE);
        uiCountBarBG->addChild(uiCount);
        
        // reward
        auto uiLayoutReward = ui::Layout::create();
        uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiRewardBG->addChild(uiLayoutReward);
        {
            for ( int i = 0; i < listReward.size(); i++ )
            {
                auto objItems = listReward.at(i);
                
                auto uiRewardBG = ui::ImageView::create("Assets/ui_common/box_item.png");
                uiRewardBG->setScale9Enabled(true);
                uiRewardBG->setContentSize(Size(58, 58));
                uiLayoutReward->addChild(uiRewardBG);
                {
                    if ( i != 0 )
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setMargin(ui::Margin(10, 0, 0, 0));
                        uiRewardBG->setLayoutParameter(params);
                    }
                    
                    auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                    uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 5));
                    uiRewardBG->addChild(uiImgItemIcon);
                    
                    std::string rewardCount = objItems->getCount();
                    if ( objItems->isString() == true )
                    {
                        MafUtils::convertNumberToShort(rewardCount);
                    }
                    
                    auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                    uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 10));
                    uiTextReward->enableOutline(Color4B::BLACK, 2);
                    uiRewardBG->addChild(uiTextReward);
                }
            }
            
            auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
            uiLayoutReward->setContentSize(sizeReward);
            uiLayoutReward->setPosition(Vec2(uiRewardBG->getContentSize().width - 84, uiRewardBG->getContentSize().height / 2));
            uiLayoutReward->requestDoLayout();
        }
        
        if ( bReceive == true )
        {
            uiName->enableStrikethrough();
        }
        else if ( bComplete == true)
        {
            auto uiBoardEffect = ui::ImageView::create("Assets/ui_common/list_bg_s_2_0.png");
            uiBoardEffect->setScale9Enabled(true);
            uiBoardEffect->setContentSize(uiRewardBG->getContentSize());
            uiBoardEffect->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2));
            uiBoardEffect->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.f), FadeIn::create(1.f), NULL)));
            uiRewardBG->addChild(uiBoardEffect);
        }
    }
    
    // reward ticket
    auto uiRewardTicketBG = ui::ImageView::create("Assets/ui_common/list_bg_s_2_3.png");
    uiRewardTicketBG->setScale9Enabled(true);
    uiRewardTicketBG->setCapInsets(Rect(25,50,1,1));
    uiRewardTicketBG->setContentSize(Size(178, sizeLayout.height));
    uiRewardTicketBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiRewardTicketBG->setPosition(Vec2(sizeLayout.width, sizeLayout.height / 2));
    uiLayout->addChild(uiRewardTicketBG);
    {
        // reward
        auto uiLayoutReward = ui::Layout::create();
        uiLayoutReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayoutReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiRewardTicketBG->addChild(uiLayoutReward);
        {
            for ( int i = 0; i < listReward.size(); i++ )
            {
                auto objItems = listReward.at(i);
                
                auto uiRewardBG = ui::ImageView::create("Assets/ui_common/box_item.png");
                uiRewardBG->setScale9Enabled(true);
                uiRewardBG->setContentSize(Size(58, 58));
                uiLayoutReward->addChild(uiRewardBG);
                {
                    if ( i != 0 )
                    {
                        auto params = ui::LinearLayoutParameter::create();
                        params->setMargin(ui::Margin(10, 0, 0, 0));
                        uiRewardBG->setLayoutParameter(params);
                    }
                    
                    auto uiImgItemIcon = ui::ImageView::create(objItems->getPath());
                    uiImgItemIcon->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2 + 5));
                    uiRewardBG->addChild(uiImgItemIcon);
                    
                    std::string rewardCount = objItems->getCount();
                    if ( objItems->isString() == true )
                    {
                        MafUtils::convertNumberToShort(rewardCount);
                    }
                    
                    auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                    uiTextReward->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, 10));
                    uiTextReward->enableOutline(Color4B::BLACK, 2);
                    uiRewardBG->addChild(uiTextReward);
                }
            }
            
            auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiLayoutReward);
            uiLayoutReward->setContentSize(sizeReward);
            uiLayoutReward->setPosition(Vec2(uiRewardTicketBG->getContentSize().width / 2, uiRewardTicketBG->getContentSize().height / 2));
            uiLayoutReward->requestDoLayout();
        }
    }
    
    //
    auto uiPlus = ui::ImageView::create("Assets/ui/purchase/shop_item_plus.png");
    uiPlus->setPosition(Vec2(uiRewardBG->getPositionX() + uiRewardBG->getContentSize().width + 22, sizeLayout.height / 2));
    uiLayout->addChild(uiPlus);
    
    if ( bReceive == true )
    {
        auto uiBlack = ui::Layout::create();
        uiBlack->setContentSize(uiRewardBG->getContentSize());
        uiBlack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiBlack->setPosition(Vec2(uiRewardBG->getContentSize().width / 2, uiRewardBG->getContentSize().height / 2));
        uiBlack->setBackGroundColor(Color3B::BLACK);
        uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiBlack->setOpacity(200);
        uiRewardBG->addChild(uiBlack);
        
        if ( bTicket == true )
        {
            auto uiBlack = ui::Layout::create();
            uiBlack->setContentSize(uiRewardTicketBG->getContentSize());
            uiBlack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            uiBlack->setPosition(Vec2(uiRewardTicketBG->getContentSize().width / 2, uiRewardTicketBG->getContentSize().height / 2));
            uiBlack->setBackGroundColor(Color3B::BLACK);
            uiBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
            uiBlack->setOpacity(200);
            uiRewardTicketBG->addChild(uiBlack);
        }
    }
}

int64_t PopupFlashmanMission::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listInfoMission.size();
    return count;
}

#pragma mark - init
void PopupFlashmanMission::initVar()
{
    _listInfoMission = FlashmanManager::getInstance()->getListInfoMission();
    
    _listMissionRewardAll.clear();
    for ( int i = 0; i < _listInfoMission.size(); i++ )
    {
        auto objMission = _listInfoMission.at(i);

        //
        auto listItems = objMission->getListReward();
        for ( int j = 0; j < listItems.size(); j++ )
        {
            auto objItem = listItems.at(j);

            bool bHave = false;
            for ( auto& obj : _listMissionRewardAll )
            {
                if ( obj->getIdx() == objItem->getIdx() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                _listMissionRewardAll.pushBack(objItemClone);
            }
        }
    }
}

void PopupFlashmanMission::initUI()
{
    // size
    Size size = Size(732, 1210);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_minigame_quest_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFlashmanMission::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupFlashmanMission::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width -= 10;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, 350) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height - 75) );
    _uiContentsUIDefault->setBackGroundColor(Color3B::BLUE);
    _uiContentsUIDefault->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    _uiContentsUIDefault->setClippingEnabled(true);
    layout->addChild(_uiContentsUIDefault);
    
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width, 105) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 25) );
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize( Size(size.width, size.height - 75 - _uiContentsUIDefault->getContentSize().height - 25 - _uiContentsUIButtons->getContentSize().height - 25) );
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIList->setPosition( Vec2(size.width / 2, size.height - 75 - _uiContentsUIDefault->getContentSize().height) );
    layout->addChild(_uiContentsUIList);
    
    
}

void PopupFlashmanMission::uiDefault()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui/flashman/bsbs_ mission_bg_1.png");
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    uiBG->setScale(size.height / uiBG->getContentSize().height);
    ((ui::Scale9Sprite*)uiBG->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create("", GAME_FONT, 24);
    uiText->setPosition(Vec2(size.width / 2, 54));
    uiText->setTextAreaSize(Size(size.width - 10, 108 - 10));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    layout->addChild(uiText);
    {
        auto action1 = CallFuncN::create([=](Ref* sender){
            int time = (int)(FlashmanManager::getInstance()->getTimeReset() - UtilsDate::getInstance()->getTime());//서버에서 주는 시간으로 변경 필요
            
            int day = time / 86400;
            int hour = time % 86400 / 3600;
            int min = time % 3600 / 60;
             
            std::string text = GAME_TEXT("t_ui_minigame_quest_msg_1") + "\n";
            text += GAME_TEXTFORMAT("t_ui_raid_end_time", day, hour, min);
            
            ((ui::CText*)sender)->setString(text);
        });
        auto action2 = DelayTime::create(60.f);
        uiText->runAction(RepeatForever::create(Sequence::create(action1, action2, NULL)));
    }
    
    // ticket purchase
    auto uiTicketBG = ui::ImageView::create("Assets/ui/raid/raid_mission_box_1.png");
    uiTicketBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiTicketBG->setPosition(Vec2(size.width - 5, size.height - 5));
    layout->addChild(uiTicketBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_new_raid_ticket_msg_1"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTicketBG->getContentSize().width / 2, uiTicketBG->getContentSize().height - 4 -23.5));
        uiText->setTextAreaSize(Size(uiTicketBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextColor(Color4B(255, 216, 0, 255));
        uiTicketBG->addChild(uiText);
        
        auto uiBuy = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
        uiBuy->setScale9Enabled(true);
        uiBuy->setCapInsets(Rect(33,33,2,2));
        uiBuy->setContentSize(Size(240, 60));
        uiBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiBuy->setPosition(Vec2(uiTicketBG->getContentSize().width / 2, 4));
        uiBuy->addClickEventListener(CC_CALLBACK_1(PopupFlashmanMission::onClickTicketBuy, this));
        uiBuy->setName("UI_TICKET_BUY");
        uiTicketBG->addChild(uiBuy);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_go_buy_1"), GAME_FONT, 28);
            uiText->setPosition(Vec2(uiBuy->getContentSize().width / 2, uiBuy->getContentSize().height /2));
            uiText->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setName("UI_TICKET_BUY_TEXT");
            uiBuy->addChild(uiText);
        }
        
        //rewards
        auto uiReward = ui::Layout::create();
        uiReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        uiReward->setPosition(Vec2(uiTicketBG->getContentSize().width / 2, 64 + 40.5));
        uiReward->setLayoutType(ui::Layout::Type::HORIZONTAL);
        uiReward->setName("UI_TICKET_REWARD");
        uiTicketBG->addChild(uiReward);
        
        auto uiStemp = ui::ImageView::create("Assets/ui/raid/icon_activate_1.png");
        uiStemp->setPosition(Size(uiTicketBG->getContentSize().width / 2, uiTicketBG->getContentSize().height / 2));
        uiStemp->setVisible(false);
        uiStemp->setName("UI_TICKET_STEMP");
        uiTicketBG->addChild(uiStemp);
    }
}

void PopupFlashmanMission::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    
    auto uiTitleBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
    uiTitleBG->setScale9Enabled(true);
    uiTitleBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiTitleBG->setContentSize(Size(490, 40));
    uiTitleBG->setPosition(Vec2(5, size.height - 5));
    layout->addChild(uiTitleBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_mission_reward"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTitleBG->getContentSize().width / 2, uiTitleBG->getContentSize().height / 2));
        uiText->setColor(Color3B(128,128,128));
        uiTitleBG->addChild(uiText);
    }
    
    auto uiTitleTicketBG = ui::ImageView::create("Assets/ui_common/base_box_1_9.png");
    uiTitleTicketBG->setScale9Enabled(true);
    uiTitleTicketBG->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    uiTitleTicketBG->setContentSize(Size(178, 40));
    uiTitleTicketBG->setPosition(Vec2(size.width - 5, size.height - 5));
    layout->addChild(uiTitleTicketBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_minigame_ticket_msg"), GAME_FONT, 24);
        uiText->setPosition(Vec2(uiTitleTicketBG->getContentSize().width / 2, uiTitleTicketBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiTitleTicketBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setColor(Color3B(227,199,110));
        uiTitleTicketBG->addChild(uiText);
    }
    
    _uiList = ui::CListView::create(this, Size(size.width - 10, size.height - 50), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

void PopupFlashmanMission::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBtn = ui::Button::create("Assets/ui_common/btn_1_9_on.png");
    uiBtn->setScale9Enabled(true);
    uiBtn->setCapInsets(Rect(33,33,2,2));
    uiBtn->setContentSize(Size(672, size.height));
    uiBtn->setPosition(Vec2(size.width / 2, size.height / 2));
    uiBtn->addClickEventListener(CC_CALLBACK_1(PopupFlashmanMission::onClickRewardAll, this));
    uiBtn->setName("UI_BTN");
    layout->addChild(uiBtn);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_attend_receive_all"), GAME_FONT, 30);
        uiText->setPosition(Vec2(uiBtn->getContentSize().width / 2, uiBtn->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBtn->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBtn->addChild(uiText);
    }
}

#pragma mark - draw
void PopupFlashmanMission::drawDefault()
{
    auto layout = _uiContentsUIDefault;
    auto size = layout->getContentSize();
    
    bool bTicketBuy = FlashmanManager::getInstance()->isTicket();
    
    if ( auto uiText = utils::findChild<ui::CText*>(layout, "UI_TICKET_BUY_TEXT"); uiText != nullptr )
    {
        if ( bTicketBuy == true )
            uiText->setString(GAME_TEXT("t_ui_raid_tickt_noti"));
        else
            uiText->setString(GAME_TEXT("t_ui_go_buy_1"));
    }
    
    if ( auto uiReward = utils::findChild<ui::Layout*>(layout, "UI_TICKET_REWARD"); uiReward != nullptr )
    {
        uiReward->removeAllChildren();
        
        for ( int i = 0; i < _listMissionRewardAll.size(); i++ )
        {
            auto objItem = _listMissionRewardAll.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/box_item.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(58, 58));
            uiReward->addChild(uiItemBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(10, 0, 0, 0));
                    uiItemBG->setLayoutParameter(params);
                }
                
                auto uiImgItemIcon = ui::ImageView::create(objItem->getPath());
                uiImgItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiImgItemIcon);
                
                std::string rewardCount = objItem->getCount();
                if ( objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                
                auto uiTextReward = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiTextReward->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiTextReward->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiTextReward);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiReward);
        uiReward->setContentSize(sizeReward);
        uiReward->requestDoLayout();
    }
    
    if ( auto uiStemp = utils::findChild<ui::ImageView*>(layout, "UI_TICKET_STEMP"); uiStemp != nullptr )
    {
        uiStemp->setVisible(bTicketBuy);
    }
}

void PopupFlashmanMission::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

void PopupFlashmanMission::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
    if ( auto uiBtn = utils::findChild<ui::Button*>(layout, "UI_BTN"); uiBtn != nullptr )
    {
        bool bComplete = FlashmanManager::getInstance()->isMissionComplete();
        
        std::string pathBtn = "Assets/ui_common/btn_1_off.png";
        if ( bComplete == true )
        {
            pathBtn = "Assets/ui_common/btn_1_9_on.png";
        }
        uiBtn->loadTextureNormal(pathBtn);
        uiBtn->setEnabled(bComplete);
    }
    
}

 
#pragma mark - set, get

#pragma mark - callback
void PopupFlashmanMission::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestInfo([=](bool){
            PopupLoading::hide();
            RefreshManager::getInstance()->refreshUI(E_REFRESH::CURRENCY);
        });
        
        PopupLoading::show();
        SaveManager::getInstance()->requestSave([=](bool bResult){
            PopupLoading::hide();
        });
        
        FlashmanManager::getInstance()->setTicket(true);
        
        // draw
        drawDefault();
        drawList();
        drawButtons();
        
        // alert
        std::string strText = GAME_TEXT("t_ui_cash_msg_1");
        if(nIdx == 81000000)
            strText = GAME_TEXT("t_ui_minigame_tickt_pop_2");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strText);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::FAIL )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_29"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else if ( eResult == MafGooglePlay::CANCEL && nResult != -1 )
    {
        /*
         0 # 더이상 구매 불가능
         1 # 성공
         2 #이벤트 중이 아닙니다. ( 복귀자 )
         3 # 판매 시작한 상품이 아닙니다.
         4 # 기간별 판매이면서 종료시간이 없음 # 주간판매 or 일간판매이면서 간격정보없음
         5 # 샵데이터가 없습니다.
         6 # 판매 종료된 상품입니다.
         7 # 조건이 맞지 않습니다. (뿌리링 선행 상품구매 x)
         8 # 서버 점검중..
         */
        std::string strMessageKey = "t_ui_error_30";
        switch (nResult) {
            case 0: strMessageKey = "t_ui_cash_msg_9_0";    break;
            case 2: strMessageKey = "t_ui_cash_msg_9_2";    break;
            case 3: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 6: strMessageKey = "t_ui_cash_msg_9_3";    break;
            case 8: strMessageKey = "t_ui_cash_msg_9_8";    break;
                
            default:
                break;
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
}

void PopupFlashmanMission::callbackInfo(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    drawDefault();
    drawList();
    drawButtons();
}

void PopupFlashmanMission::callbackReward(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    FlashmanManager::getInstance()->requestInfo(true, CC_CALLBACK_2(PopupFlashmanMission::callbackInfo, this));
}

#pragma mark - schedule

#pragma mark - click
void PopupFlashmanMission::onClickClose(Ref* sender)
{
    hide();
}

void PopupFlashmanMission::onClickReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiButton = dynamic_cast<ui::Layout*>(sender);
    
    auto obj = _listInfoMission.at(uiButton->getTag());
    if ( obj == nullptr )
    {
        return;
    }
    
    bool bReceive =  FlashmanManager::getInstance()->getMissionReceive(obj->getIdx());
    if ( bReceive == true )
    {
        return;
    }
    
    int count = FlashmanManager::getInstance()->getMissionCount(obj->getIdx());
    int countMax = (int)obj->getCountMax();
    if ( count < countMax )
    {
        return;
    }
        
    FlashmanManager::getInstance()->requestMissionReward(true, obj->getIdx(), CC_CALLBACK_2(PopupFlashmanMission::callbackReward, this));
}

void PopupFlashmanMission::onClickRewardAll(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
        
    FlashmanManager::getInstance()->requestMissionRewardAll(true, CC_CALLBACK_2(PopupFlashmanMission::callbackReward, this));
}

void PopupFlashmanMission::onClickTicketBuy(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE, E_SOUND_TYPE::CONTENTS);
    
    auto uiBUtton = dynamic_cast<ui::Button*>(sender);
    
    bool bTicket = FlashmanManager::getInstance()->isTicket();
    if ( bTicket == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_minigame_tickt_pop_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto objProduct = CashManager::getInstance()->getProduct(81000000);
    if ( objProduct == nullptr )
    {
        return;
    }
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    
    CashManager::getInstance()->purchaseHas(81000000, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(81000000, CC_CALLBACK_4(PopupFlashmanMission::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            int time = (int)(FlashmanManager::getInstance()->getTimeReset() - UtilsDate::getInstance()->getTime());//서버에서 주는 시간으로 변경 필요
            
            int day = time / 86400;
            int hour = time % 86400 / 3600;
            int min = time % 3600 / 60;
            
            
            std::string strMsg = GAME_TEXT("t_ui_minigame_tickt_pop_1") + "\n" + GAME_TEXTFORMAT("t_ui_minigame_end_time", day, hour, min);
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMsg);
            popup->addButton(GAME_TEXT("t_ui_costume_skin_msg_2"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(81000000, CC_CALLBACK_4(PopupFlashmanMission::callbackPurchase, this));
            });
            popup->addButton(GAME_TEXT("t_ui_close"), true, nullptr);
            popup->show();
        }
    });
}
