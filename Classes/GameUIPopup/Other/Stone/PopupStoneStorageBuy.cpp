//
//  PopupStoneStorageBuy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupStoneStorageBuy.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "ManagerGame/StoneManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupStoneStorageBuy::PopupStoneStorageBuy():
_bInitUI(false),

//
_count(0),

//
_uiContentsInfo(nullptr),
_uiContentsCount(nullptr),
_uiContentsButton(nullptr)

{
    
}

PopupStoneStorageBuy::~PopupStoneStorageBuy(void)
{
    
}

bool PopupStoneStorageBuy::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    return true;
}

void PopupStoneStorageBuy::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        initVar();
        initUI();
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawCount();
    drawButton();
}

#pragma mark - ui
void PopupStoneStorageBuy::initVar()
{
    setCountChange(1);
}

void PopupStoneStorageBuy::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(517, 714) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_costume_msg_39"), GAME_FONT, 32);
    uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    uiContainer();
    uiInfo();
    uiCount();
    uiButton();
}

void PopupStoneStorageBuy::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    // 184 * 241
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, 345));
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(_uiContentsInfo);
    
    _uiContentsButton = ui::Layout::create();
    _uiContentsButton->setContentSize(Size(size.width, 88));
    _uiContentsButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsButton->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButton);
    
    _uiContentsCount = ui::Layout::create();
    _uiContentsCount->setContentSize( Size(size.width, size.height - _uiContentsInfo->getContentSize().height - _uiContentsButton->getContentSize().height) );
    _uiContentsCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsCount->setPosition( Vec2(size.width / 2, _uiContentsButton->getContentSize().height) );
    layout->addChild(_uiContentsCount);
}
 
void PopupStoneStorageBuy::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiItemBG = ui::ImageView::create("Assets/ui/return_event/return_reward_box.png");
    uiItemBG->setScale9Enabled(true);
    uiItemBG->setContentSize(Size(160, 160));
    uiItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiItemBG->setPosition(Vec2(size.width / 2, size.height - 25));
    layout->addChild(uiItemBG);
    
    auto uiItemIcon = ui::ImageView::create("Assets/icon/icon_stone.png");
    uiItemIcon->setPosition(Vec2(size.width / 2, size.height - 25 - 80));
    ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiItemIcon->setScale(2);
    uiItemIcon->setName("UI_ITEM_ICON");
    layout->addChild(uiItemIcon);
    
    
    auto uiInfoCountBG = ui::ImageView::create("Assets/ui_common/base_box_1_23.png");
    uiInfoCountBG->setScale9Enabled(true);
    uiInfoCountBG->setContentSize(Size(426, 54));
    uiInfoCountBG->setPosition(Vec2(size.width / 2, 80));
    layout->addChild(uiInfoCountBG);
    {
        auto uiArrow = ui::ImageView::create("Assets/ui_common/arrow_R04.png");
        uiArrow->setPosition(Vec2(uiInfoCountBG->getContentSize().width / 2, uiInfoCountBG->getContentSize().height / 2));
        uiInfoCountBG->addChild(uiArrow);
        
        auto uiCountPrev = ui::CText::create("0", GAME_FONT, 26);
        uiCountPrev->setPosition(Vec2(uiInfoCountBG->getContentSize().width * 0.25, uiInfoCountBG->getContentSize().height / 2));
        uiCountPrev->setName("UI_COUNT_PREV");
        uiInfoCountBG->addChild(uiCountPrev);
        
        auto uiCountNext = ui::CText::create("0", GAME_FONT, 26);
        uiCountNext->setPosition(Vec2(uiInfoCountBG->getContentSize().width * 0.75, uiInfoCountBG->getContentSize().height / 2));
        uiCountNext->setTextColor(Color4B(55, 253, 0, 255));
        uiCountNext->setName("UI_COUNT_NEXT");
        uiInfoCountBG->addChild(uiCountNext);
    }
    
}

void PopupStoneStorageBuy::uiCount()
{
    auto layout = _uiContentsCount;
    auto size = layout->getContentSize();
    
    auto uiCountBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
    uiCountBG->setPosition(Vec2(size.width / 2, size.height * 0.30));
    layout->addChild(uiCountBG);
    {
        auto uiCount = ui::CText::create("0", GAME_FONT, 24);
        uiCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
        uiCount->setName("UI_COUNT");
        uiCountBG->addChild(uiCount);
        
        auto uiArrowLeft = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_l.png");
        uiArrowLeft->setPosition(Vec2(uiCountBG->getPositionX() - 90, uiCountBG->getPositionY()));
        uiArrowLeft->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickCountArrow, this));
        uiArrowLeft->setTag(-1);
        layout->addChild(uiArrowLeft);
        
        auto uiArrowLeftD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_l.png");
        uiArrowLeftD->setPosition(Vec2(uiCountBG->getPositionX() - 150, uiCountBG->getPositionY()));
        uiArrowLeftD->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickCountArrow, this));
        uiArrowLeftD->setTag(-10);
        layout->addChild(uiArrowLeftD);
        
        auto uiArrowRight = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
        uiArrowRight->setPosition(Vec2(uiCountBG->getPositionX() + 90, uiCountBG->getPositionY()));
        uiArrowRight->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickCountArrow, this));
        uiArrowRight->setTag(+1);
        layout->addChild(uiArrowRight);
        
        auto uiArrowRightD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_r.png");
        uiArrowRightD->setPosition(Vec2(uiCountBG->getPositionX() + 150, uiCountBG->getPositionY()));
        uiArrowRightD->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickCountArrow, this));
        uiArrowRightD->setTag(+10);
        layout->addChild(uiArrowRightD);
    }
    
    auto uiCostBG = ui::Layout::create();
    uiCostBG->setContentSize(Size(size.width, 45));
    uiCostBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiCostBG->setPosition(Vec2(size.width / 2, size.height * 0.70));
    uiCostBG->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiCostBG->setName("UI_COST");
    layout->addChild(uiCostBG);
    
}

void PopupStoneStorageBuy::uiButton()
{
    auto layout = _uiContentsButton;
    auto size = layout->getContentSize();
    
    auto uiButton = ui::Layout::create();
    uiButton->setIgnoreAnchorPointForPosition(false);
    uiButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiButton->setPosition(Vec2(size.width / 2, size.height / 2));
    uiButton->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiButton->setName("UI_BUTTONS");
    layout->addChild(uiButton);
    {
        // ad
        auto uiBuy = ui::Button::create("Assets/ui_common/btn_ok.png");
        uiBuy->setScale9Enabled(true);
        uiBuy->setContentSize(Size(260, 88));
        uiBuy->setName("UI_BUY");
        uiBuy->addClickEventListener(CC_CALLBACK_1(PopupStoneStorageBuy::onClickBuy, this));
        uiButton->addChild(uiBuy);
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_event_fish_buybait_pop_btn"), GAME_FONT, 24);
            uiText->setPosition(uiBuy->getContentSize()/2);
            uiText->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiBuy->addChild(uiText);
        }
    }
}

#pragma mark - draw
void PopupStoneStorageBuy::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto countPrev = StoneManager::getInstance()->getStorageCountNow();
    auto countNext = countPrev + _count;
    
    if ( auto uiCountPrev = utils::findChild<ui::CText*>(layout, "UI_COUNT_PREV"); uiCountPrev != nullptr )
    {
        uiCountPrev->setString(std::to_string(countPrev));
    }
    
    if ( auto uiCountNext = utils::findChild<ui::CText*>(layout, "UI_COUNT_NEXT"); uiCountNext != nullptr )
    {
        uiCountNext->setString(std::to_string(countNext));
    }
}

void PopupStoneStorageBuy::drawCount()
{
    auto layout = _uiContentsCount;
    
    if ( auto uiCount = utils::findChild<ui::CText*>(layout, "UI_COUNT"); uiCount != nullptr )
    {
        uiCount->setString(std::to_string(_count));
    }
    
    if ( auto uiCostBG = utils::findChild<ui::Layout*>(layout, "UI_COST"); uiCostBG != nullptr )
    {
        uiCostBG->removeAllChildren();
        
        auto listItem = StoneManager::getInstance()->getStorageCost(_count);
        
        for ( int i = 0; i < listItem.size(); i++ )
        {
            auto objItem = listItem.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(180, 45));
            uiCostBG->addChild(uiItemBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(10, 0, 0, 0));
                    uiItemBG->setLayoutParameter(params);
                }
                
                auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                uiItemIcon->setPosition(Vec2(30, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiItemIcon, 10);
                
                std::string itemNeed = objItem->getCount();
                std::string itemNow = ItemsManager::getInstance()->getItems((E_ITEMS)objItem->getIdx());
                if ( objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(itemNeed);
                }
                
                auto uiItemCount = ui::CText::create(itemNeed, GAME_FONT, 20);
                uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width - 10, uiItemBG->getContentSize().height / 2));
                uiItemCount->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiItemCount, 10);
                
                if ( MafUtils::compareStr(itemNow, itemNeed) == false )
                {
                    uiItemCount->setTextColor(Color4B::RED);
                }
            }
        }
        
        auto sizeCost = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiCostBG);
        uiCostBG->setContentSize(sizeCost);
        uiCostBG->requestDoLayout();
    }
 
}

void PopupStoneStorageBuy::drawButton()
{
    auto layout = _uiContentsButton;
    auto uiButton = layout->getChildByName<ui::Layout*>("UI_BUTTONS");

    //
    if ( auto uiBuy = uiButton->getChildByName<ui::Button*>("UI_BUY"); uiBuy != nullptr )
    {
        
    }
     
    //
    auto sizeButtons = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiButton);
    uiButton->setContentSize(sizeButtons);
    uiButton->requestDoLayout();
    
}

#pragma mark - click
void PopupStoneStorageBuy::onClickClose(Ref* sender)
{
    hide();
}

void PopupStoneStorageBuy::onClickCountArrow(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;
    
    int countChange = uiButton->getTag();
    setCountChange(countChange);
}

void PopupStoneStorageBuy::onClickBuy(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( _count <= 0 )
    {
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        int result = StoneManager::getInstance()->onStorageBuy(_count);
        if ( result != 0 )
        {
            if ( result == 2 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            
            return;
        }
        
        drawInfo();
        drawCount();
        drawButton();
    });
    popup->show();
}

#pragma mark - callback

#pragma mark - set, get
void PopupStoneStorageBuy::setCountChange(int change)
{
    int countMax = 100;
    
    int countPrev = _count;
    int countNext = _count;
    
    countNext += change;
    if ( countNext <= 0 )
    {
        if ( countPrev == 1 )
            countNext = countMax;
        else
            countNext = 1;
    }
    
    if ( countNext > countMax )
    {
        if ( countPrev >= countMax )
            countNext = 1;
        else
            countNext = countMax;
        
    }
    
    //
    _count = countNext;
    
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawCount();
        drawButton();
    }
}
