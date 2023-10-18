//
//  PopupStoneBuy.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupStoneBuy.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"

#include "ManagerGame/StoneManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupStoneBuy::PopupStoneBuy():
_bInitUI(false),

//
_eStone(E_STONE::NONE),
_count(0),

//
_uiContentsInfo(nullptr),
_uiContentsCount(nullptr),
_uiContentsButton(nullptr),

_uiTitle(nullptr)
{
    
}

PopupStoneBuy::~PopupStoneBuy(void)
{
    
}

bool PopupStoneBuy::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    return true;
}

void PopupStoneBuy::onEnter()
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
void PopupStoneBuy::initVar()
{
    setCountChange(1);
}

void PopupStoneBuy::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(517, 714) );
    
    // label title
    _uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_3"), GAME_FONT, 32);
    _uiTitle->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5));
    _uiTitle->setTextAreaSize(Size(_spriteContainer->getContentSize().width - 200, _uiTitle->getContentSize().height));
    _uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    _uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _uiTitle->setTextColor(Color4B(COLOR_TITLE));
    _spriteContainer->addChild(_uiTitle);
    
    // item close
    auto uiClose = ui::Button::create("Assets/ui_common/btn_x.png");
    uiClose->setPosition(Vec2(_spriteContainer->getContentSize().width - uiClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    uiContainer();
    uiInfo();
    uiCount();
    uiButton();
}

void PopupStoneBuy::uiContainer()
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
    _uiContentsButton->setContentSize(Size(size.width, 78));
    _uiContentsButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsButton->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsButton);
    
    _uiContentsCount = ui::Layout::create();
    _uiContentsCount->setContentSize( Size(size.width, size.height - _uiContentsInfo->getContentSize().height - _uiContentsButton->getContentSize().height) );
    _uiContentsCount->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsCount->setPosition( Vec2(size.width / 2, _uiContentsButton->getContentSize().height) );
    layout->addChild(_uiContentsCount);
}
 
void PopupStoneBuy::uiInfo()
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
    
    auto uiItemIcon = ui::ImageView::create("");
    uiItemIcon->setPosition(Vec2(size.width / 2, size.height - 25 - 80));
    ((ui::Scale9Sprite*)uiItemIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
    uiItemIcon->setScale(2);
    uiItemIcon->setName("UI_ITEM_ICON");
    layout->addChild(uiItemIcon);
    
    auto uiInfoCountBG = ui::ImageView::create("Assets/ui_common/base_box_1_23.png");
    uiInfoCountBG->setScale9Enabled(true);
    uiInfoCountBG->setContentSize(Size(426, 54));
    uiInfoCountBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiInfoCountBG->setPosition(Vec2(size.width / 2, 80 + 5));
    layout->addChild(uiInfoCountBG);
    {
        auto uiArrow = ui::ImageView::create("Assets/ui_common/arrow_R04.png");
        uiArrow->setPosition(Vec2(uiInfoCountBG->getContentSize().width / 2, uiInfoCountBG->getContentSize().height / 2));
        uiInfoCountBG->addChild(uiArrow);
        
        auto uiCountPrev = ui::CText::create("0", GAME_FONT, 24);
        uiCountPrev->setPosition(Vec2(uiInfoCountBG->getContentSize().width * 0.25, uiInfoCountBG->getContentSize().height / 2));
        uiCountPrev->setTextAreaSize(Size(180, uiCountPrev->getContentSize().height));
        uiCountPrev->setTextOverflow(Label::Overflow::SHRINK);
        uiCountPrev->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCountPrev->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiCountPrev->setName("UI_COUNT_PREV");
        uiInfoCountBG->addChild(uiCountPrev);
        
        auto uiCountNext = ui::CText::create("0", GAME_FONT, 24);
        uiCountNext->setPosition(Vec2(uiInfoCountBG->getContentSize().width * 0.75, uiInfoCountBG->getContentSize().height / 2));
        uiCountNext->setTextAreaSize(Size(180, uiCountNext->getContentSize().height));
        uiCountNext->setTextOverflow(Label::Overflow::SHRINK);
        uiCountNext->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiCountNext->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiCountNext->setTextColor(Color4B(55, 253, 0, 255));
        uiCountNext->setName("UI_COUNT_NEXT");
        uiInfoCountBG->addChild(uiCountNext);
    }
    
    auto uiInfoEffectBG = ui::ImageView::create("Assets/ui_common/base_box_1_23.png");
    uiInfoEffectBG->setScale9Enabled(true);
    uiInfoEffectBG->setContentSize(Size(426, 54));
    uiInfoEffectBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoEffectBG->setPosition(Vec2(size.width / 2, 80 - 5));
    layout->addChild(uiInfoEffectBG);
    {
        auto uiArrow = ui::ImageView::create("Assets/ui_common/arrow_R04.png");
        uiArrow->setPosition(Vec2(uiInfoEffectBG->getContentSize().width / 2, uiInfoEffectBG->getContentSize().height / 2));
        uiInfoEffectBG->addChild(uiArrow);
        
        auto uiEffectPrev = ui::CText::create("0", GAME_FONT, 24);
        uiEffectPrev->setPosition(Vec2(uiInfoEffectBG->getContentSize().width * 0.25, uiInfoEffectBG->getContentSize().height / 2));
        uiEffectPrev->setTextAreaSize(Size(180, uiEffectPrev->getContentSize().height));
        uiEffectPrev->setTextOverflow(Label::Overflow::SHRINK);
        uiEffectPrev->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffectPrev->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiEffectPrev->setName("UI_EFFECT_PREV");
        uiInfoEffectBG->addChild(uiEffectPrev);
        
        auto uiEffectNext = ui::CText::create("0", GAME_FONT, 24);
        uiEffectNext->setPosition(Vec2(uiInfoEffectBG->getContentSize().width * 0.75, uiInfoEffectBG->getContentSize().height / 2));
        uiEffectNext->setTextAreaSize(Size(180, uiEffectNext->getContentSize().height));
        uiEffectNext->setTextOverflow(Label::Overflow::SHRINK);
        uiEffectNext->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiEffectNext->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiEffectNext->setTextColor(Color4B(55, 253, 0, 255));
        uiEffectNext->setName("UI_EFFECT_NEXT");
        uiInfoEffectBG->addChild(uiEffectNext);
    }
    
}

void PopupStoneBuy::uiCount()
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
        uiArrowLeft->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickCountArrow, this));
        uiArrowLeft->setTag(-1);
        layout->addChild(uiArrowLeft);
        
        auto uiArrowLeftD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_l.png");
        uiArrowLeftD->setPosition(Vec2(uiCountBG->getPositionX() - 150, uiCountBG->getPositionY()));
        uiArrowLeftD->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickCountArrow, this));
        uiArrowLeftD->setTag(-10);
        layout->addChild(uiArrowLeftD);
        
        auto uiArrowRight = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
        uiArrowRight->setPosition(Vec2(uiCountBG->getPositionX() + 90, uiCountBG->getPositionY()));
        uiArrowRight->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickCountArrow, this));
        uiArrowRight->setTag(+1);
        layout->addChild(uiArrowRight);
        
        auto uiArrowRightD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_r.png");
        uiArrowRightD->setPosition(Vec2(uiCountBG->getPositionX() + 150, uiCountBG->getPositionY()));
        uiArrowRightD->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickCountArrow, this));
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

void PopupStoneBuy::uiButton()
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
        uiBuy->addClickEventListener(CC_CALLBACK_1(PopupStoneBuy::onClickBuy, this));
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
void PopupStoneBuy::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    auto name = StoneManager::getInstance()->getName(_eStone);
    auto pathIcon = StoneManager::getInstance()->getPath(_eStone);
    
    auto countPrev = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, _eStone);
    auto countNext = countPrev + _count;
    
    auto effectPrev = StoneManager::getInstance()->getEffect(_eStone, countPrev) * 100;
    auto effectNext = StoneManager::getInstance()->getEffect(_eStone, countNext) * 100;
    
    auto effectPrevStr = MafUtils::doubleToString(effectPrev);
    MafUtils::convertNumberToShort(effectPrevStr);
    
    auto effectNextStr = MafUtils::doubleToString(effectNext);
    MafUtils::convertNumberToShort(effectNextStr);
    
    
    //
    _uiTitle->setString(name);
    
    //
    if ( auto uiItemIcon = utils::findChild<ui::ImageView*>(layout, "UI_ITEM_ICON"); uiItemIcon != nullptr )
    {
        uiItemIcon->loadTexture(pathIcon);
    }

    //
    if ( auto uiCountPrev = utils::findChild<ui::CText*>(layout, "UI_COUNT_PREV"); uiCountPrev != nullptr )
    {
        if ( _eStone == E_STONE::ATTACK || _eStone == E_STONE::GOLD || _eStone == E_STONE::KEY )
        {
            uiCountPrev->setString(std::to_string(countPrev));
        }
        else
        {
            uiCountPrev->setString(MafUtils::format("%d (%d/5)", countPrev / 6, countPrev % 6));
        }
    }
    
    if ( auto uiCountNext = utils::findChild<ui::CText*>(layout, "UI_COUNT_NEXT"); uiCountNext != nullptr )
    {
        if ( _eStone == E_STONE::ATTACK || _eStone == E_STONE::GOLD || _eStone == E_STONE::KEY )
        {
            uiCountNext->setString(std::to_string(countNext));
        }
        else
        {
            uiCountNext->setString(MafUtils::format("%d (%d/5)", countNext / 6, countNext % 6));
        }
    }
    
    
    //
    if ( auto uiEffectPrev = utils::findChild<ui::CText*>(layout, "UI_EFFECT_PREV"); uiEffectPrev != nullptr )
    {
        std::string effectStr = StoneManager::getInstance()->getEffectName(_eStone);
        effectStr = MafUtils::format(effectStr.c_str(), effectPrevStr.c_str());
        
        uiEffectPrev->setString(effectStr);
    }
    
    if ( auto uiEffectNext = utils::findChild<ui::CText*>(layout, "UI_EFFECT_NEXT"); uiEffectNext != nullptr )
    {
        std::string effectStr = StoneManager::getInstance()->getEffectName(_eStone);
        effectStr = MafUtils::format(effectStr.c_str(), effectNextStr.c_str());
        
        uiEffectNext->setString(effectStr);
    }
    
}

void PopupStoneBuy::drawCount()
{
    auto layout = _uiContentsCount;
    
    if ( auto uiCount = utils::findChild<ui::CText*>(layout, "UI_COUNT"); uiCount != nullptr )
    {
        uiCount->setString(std::to_string(_count));
    }
    
    if ( auto uiCostBG = utils::findChild<ui::Layout*>(layout, "UI_COST"); uiCostBG != nullptr )
    {
        uiCostBG->removeAllChildren();
        
        auto listItem = StoneManager::getInstance()->getCost(_eStone, _count);
        
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

void PopupStoneBuy::drawButton()
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
void PopupStoneBuy::onClickClose(Ref* sender)
{
    hide();
}

void PopupStoneBuy::onClickCountArrow(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;
    
    int countChange = uiButton->getTag();
    setCountChange(countChange);
}

void PopupStoneBuy::onClickBuy(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    if ( _count <= 0 )
    {
        return;
    }
    
    int countAll = StoneManager::getInstance()->getCountAll();
    int storageCount = StoneManager::getInstance()->getStorageCountNow();
    
    if ( countAll + _count > storageCount )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_16"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_msg_buy"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){
        
        int result = StoneManager::getInstance()->onBuy(_eStone, _count, CC_CALLBACK_2(PopupStoneBuy::callbackBuy, this));
        if ( result != 0 )
        {
            if ( result == 2 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            else if ( result == 3 )
            {
                auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_16"));
                popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
                popup->show();
            }
            return;
        }
        
        
    });
    popup->show();
}

#pragma mark - callback
void PopupStoneBuy::callbackBuy(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
        {
            strMessage = GAME_TEXT("t_ui_error_31");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    int countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, _eStone);
    int countMax = StoneManager::getInstance()->getCountMax(_eStone);
    
    if ( countNow >= countMax )
    {
        hide();
        return;
    }
    
    //
    setCountChange(0);
    
    //
    drawInfo();
    drawCount();
    drawButton();
}

#pragma mark - set, get
void PopupStoneBuy::setStone(E_STONE eStone)
{
    _eStone = eStone;
    
    if ( _bInitUI == true )
    {
        drawInfo();
        drawCount();
        drawButton();
    }
}

void PopupStoneBuy::setCountChange(int change)
{
    int countMax = StoneManager::getInstance()->getCountMax(_eStone) - StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, _eStone);
    countMax = MIN(countMax, 100);
    
    if ( _count > countMax )
    {
        _count = countMax;
    }
    
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
    _count = MIN(_count, countMax);
    
    //
    if ( _bInitUI == true )
    {
        drawInfo();
        drawCount();
        drawButton();
    }
}
