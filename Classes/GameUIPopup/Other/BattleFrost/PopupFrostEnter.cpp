//
//  PopupFrostEnter.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/12/03.
//

#include "PopupFrostEnter.h"

#include "GameUI/UI/UICurrency.h"
#include "GameUI/UI/UIBuff.h"
#include "GameUI/UI/UIReddot.h"

#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupFrostEnter* PopupFrostEnter::create()
{
    PopupFrostEnter* pRet = new(std::nothrow) PopupFrostEnter();
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

PopupFrostEnter::PopupFrostEnter() :
_bInitUI(false),

_countNow(1),
_countMax(0),
_bToggleAnimation(true),

_callbackResult(nullptr),

// widget
_uiContentsUICount(nullptr),
_uiContentsUIButtons(nullptr)
{
    
}

PopupFrostEnter::~PopupFrostEnter(void)
{
    
}

bool PopupFrostEnter::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    setName("PopupFrostEnter");
    
    //
    initVar();
    initUI();
    
    // event
    
    return true;
}

void PopupFrostEnter::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiCount();
        uiButtons();
    }
    
    //
    drawCount();
    drawButtons();
}

#pragma mark - init
void PopupFrostEnter::initVar()
{
    int clearFloor = PrisonFrostManager::getInstance()->getClearFloor();
    int clearFloorMax = PrisonFrostManager::getInstance()->getClearFloorMax();
    
    _countMax = clearFloorMax - clearFloor;
    if ( _countMax > 100 )
    {
        _countMax = 100;
    }
    
    if ( clearFloor >= clearFloorMax )
    {
        _countNow = 0;
        _countMax = 0;
    }
}

void PopupFrostEnter::initUI()
{
    // size
    Size size = Size(516, 558);

    _spriteContainer->setContentSize(size);
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_prison_enter_frost_title"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupFrostEnter::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - 50, _spriteContainer->getContentSize().height - 37.5);
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupFrostEnter::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = 508;
    size.height = size.height - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    //
    _uiContentsUIInfo = ui::Layout::create();
    _uiContentsUIInfo->setContentSize( Size(size.width, 130) );
    _uiContentsUIInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUIInfo->setPosition( Vec2(size.width / 2, size.height) );
    _uiContentsUIInfo->setBackGroundColor(Color3B::GREEN);
    _uiContentsUIInfo->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    layout->addChild(_uiContentsUIInfo);
    
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width - 40, 164) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsUIButtons);
    
    _uiContentsUICount = ui::Layout::create();
    _uiContentsUICount->setContentSize( Size(size.width - 40, size.height - _uiContentsUIButtons->getContentSize().height - _uiContentsUIInfo->getContentSize().height ) );
    _uiContentsUICount->setIgnoreAnchorPointForPosition(false);
    _uiContentsUICount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _uiContentsUICount->setPosition( Vec2(size.width / 2, size.height - _uiContentsUIInfo->getContentSize().height) );
    layout->addChild(_uiContentsUICount);
}

void PopupFrostEnter::uiInfo()
{
    auto layout = _uiContentsUIInfo;
    Size size = layout->getContentSize();
    
    //
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_3.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(Size(size.width, size.height));
    uiBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBG->setPosition(Vec2(size.width / 2, size.height));
    layout->addChild(uiBG);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_enter_frost_mag"), GAME_FONT, 24);
    uiText->setPosition(Vec2(size.width / 2, size.height / 2));
    uiText->setTextAreaSize(Size(size.width, size.height));
    uiText->setTextOverflow(Label::Overflow::SHRINK);
    uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiBG->addChild(uiText);
    
}

void PopupFrostEnter::uiCount()
{
    auto layout = _uiContentsUICount;
    Size size = layout->getContentSize();
    
    auto uiCostBG = ui::Layout::create();
    uiCostBG->setContentSize(Size(size.width, 44));
    uiCostBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCostBG->setPosition(Vec2(size.width / 2, size.height - 20));
    uiCostBG->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiCostBG->setName("UI_COST_BG");
    layout->addChild(uiCostBG);
    
    
    
    //
    auto uiCountBG = ui::ImageView::create("Assets/ui/prison/bafometz/baphomet_numbox.png");
    uiCountBG->setPosition(Vec2(layout->getContentSize().width / 2, 50));
    uiCountBG->setName("UI_COUNT_BG");
    layout->addChild(uiCountBG);
    {
        auto uiCount = ui::CText::create("0", GAME_FONT, 28);
        uiCount->setPosition(Vec2(uiCountBG->getContentSize().width / 2, uiCountBG->getContentSize().height / 2));
        uiCount->setName("UI_COUNT");
        uiCountBG->addChild(uiCount);
        
        auto uiArrowL = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_l.png");
        uiArrowL->setPosition(Vec2(uiCountBG->getContentSize().width / 2 - 90, uiCountBG->getContentSize().height / 2));
        uiArrowL->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickArrow, this));
        uiArrowL->setTag(-1);
        uiCountBG->addChild(uiArrowL);
        
        auto uiArrowLD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_l.png");
        uiArrowLD->setPosition(Vec2(uiCountBG->getContentSize().width / 2 - 150, uiCountBG->getContentSize().height / 2));
        uiArrowLD->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickArrow, this));
        uiArrowLD->setTag(-10);
        uiCountBG->addChild(uiArrowLD);
        
        auto uiArrowR = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_r.png");
        uiArrowR->setPosition(Vec2(uiCountBG->getContentSize().width / 2 + 90, uiCountBG->getContentSize().height / 2));
        uiArrowR->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickArrow, this));
        uiArrowR->setTag(1);
        uiCountBG->addChild(uiArrowR);
        
        auto uiArrowRD = ui::Button::create("Assets/ui/prison/bafometz/baphomet_arrow_big_r.png");
        uiArrowRD->setPosition(Vec2(uiCountBG->getContentSize().width / 2 + 150, uiCountBG->getContentSize().height / 2));
        uiArrowRD->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickArrow, this));
        uiArrowRD->setTag(10);
        uiCountBG->addChild(uiArrowRD);
    }
}

void PopupFrostEnter::uiButtons()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    auto uiBtnConfirm = ui::Button::create("Assets/ui_common/btn_ok.png");
    uiBtnConfirm->setScale9Enabled(true);
    uiBtnConfirm->setContentSize(Size(272, 88));
    uiBtnConfirm->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiBtnConfirm->setPosition(Vec2(size.width / 2, size.height));
    uiBtnConfirm->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickEnter, this));
    layout->addChild(uiBtnConfirm);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_enter"), GAME_FONT, 32);
        uiText->setPosition(Vec2(uiBtnConfirm->getContentSize().width / 2, uiBtnConfirm->getContentSize().height / 2));
        uiBtnConfirm->addChild(uiText);
    }
    
    auto uiBtnToggle = ui::Button::create("Assets/ui/mission/mission_on.png");
    uiBtnToggle->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    uiBtnToggle->setPosition(Vec2(size.width, 0));
    uiBtnToggle->setName("UI_TOGGLE");
    uiBtnToggle->addClickEventListener(CC_CALLBACK_1(PopupFrostEnter::onClickToggleAnimation, this));
    layout->addChild(uiBtnToggle);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_prison_enter_frost_skip_bt"), GAME_FONT, 24);
        uiText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        uiText->setPosition(Vec2(-10, 0));
        uiBtnToggle->addChild(uiText);
    }
}
          
#pragma mark - draw
void PopupFrostEnter::drawCount()
{
    auto layout = _uiContentsUICount;
    Size size = layout->getContentSize();
    
    //
    auto uiCountBG = layout->getChildByName<ui::ImageView*>("UI_COUNT_BG");
    {
        auto uiCount = uiCountBG->getChildByName<ui::CText*>("UI_COUNT");
        uiCount->setString(MafUtils::toString(_countNow));
    }
    
    //
    std::map<int, std::string> listCost;
    for (int i = 1; i <= _countNow; i++ )
    {
        int clearFloor = PrisonFrostManager::getInstance()->getClearFloor() + i;
        
        auto objInfoFloor = PrisonFrostManager::getInstance()->getInfoFloor(clearFloor);
        if ( objInfoFloor != nullptr )
        {
            auto objItem = objInfoFloor->getCost();
            if ( objItem != nullptr )
            {
                auto iter = listCost.find(objItem->getIdx());
                if ( iter != listCost.end() )
                {
                    std::string count = objItem->getCount();
                    count = MafUtils::bigAddNum(count, iter->second);
                    iter->second = count;
                }
                else
                {
                    listCost[objItem->getIdx()] = objItem->getCount();
                }
            }
        }
    }
    
    auto uiCostBG = layout->getChildByName<ui::Layout*>("UI_COST_BG");
    if ( uiCostBG != nullptr )
    {
        uiCostBG->removeAllChildren();
        
        size_t count = listCost.size();
        
        double padding = 10;
        
        double height = 44;
        double width = size.width / count;
        if ( count > 1 )
            width -= padding;
        width = MIN(width, 180);
        
        bool bFirst = true;
        for (auto iter = listCost.begin() ; iter !=  listCost.end(); iter++)
        {
            auto uiBG = ui::ImageView::create("Assets/ui/costume/costume_amount_box.png");
            uiBG->setScale9Enabled(true);
            uiBG->setContentSize(Size(width, height));
            uiCostBG->addChild(uiBG);
            if ( bFirst == false )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(padding, 0, 0, 0));
                uiBG->setLayoutParameter(params);
            }
            bFirst = false;
            
            //
            auto objItem = ItemsManager::getInstance()->getInfoItems(iter->first);
            auto uiIcon = ui::ImageView::create(objItem->getPath());
            uiIcon->setPosition(Vec2(25, height / 2));
            uiBG->addChild(uiIcon);
            
            auto uiCount = ui::CText::create(MafUtils::format("x%s", iter->second.c_str()), GAME_FONT, 24);
            uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiCount->setPosition(Vec2(width - 10, height / 2));
            uiCount->setTextAreaSize(Size(width * 0.55, uiCount->getContentSize().height));
            uiCount->setTextOverflow(Label::Overflow::SHRINK);
            uiCount->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiCount->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            uiBG->addChild(uiCount);
        }
        
        auto sizeCostBG = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiCostBG);
        uiCostBG->setContentSize(sizeCostBG);
        uiCostBG->requestDoLayout();
    }
}

void PopupFrostEnter::drawButtons()
{
    auto layout = _uiContentsUIButtons;
    
    //
    auto uiBtnToggle =  layout->getChildByName<ui::Button*>("UI_TOGGLE");
    if ( uiBtnToggle != nullptr )
    {
        if ( _bToggleAnimation )
        {
            uiBtnToggle->loadTextureNormal("Assets/ui/mission/mission_on.png");
        }
        else
        {
            uiBtnToggle->loadTextureNormal("Assets/ui/mission/mission_off.png");
        }
    }
}
 
#pragma mark - ui : set, get
void PopupFrostEnter::setCallback(const std::function<void(int, bool)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - callback

#pragma mark - click
void PopupFrostEnter::onClickClose(Ref* sender)
{
    hide();
}

void PopupFrostEnter::onClickArrow(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int count = uiBtn->getTag();;
    if ( count <= 0 )
    {
        _countNow += count;
        if ( _countNow <= 0 )
        {
            _countNow = _countMax;
        }
    }
    else
    {
        _countNow += count;
        if ( _countNow > _countMax )
        {
            _countNow = 1;
        }
    }
    
    if ( _countNow > _countMax )
    {
        _countNow = _countMax;
    }
    
    //
    drawCount();
}

void PopupFrostEnter::onClickEnter(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    if ( _callbackResult != nullptr )
    {
        _callbackResult(_countNow, _bToggleAnimation);
    }
    
    hide();
}

void PopupFrostEnter::onClickToggleAnimation(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    _bToggleAnimation = !_bToggleAnimation;
    if ( _bToggleAnimation )
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_on.png");
    }
    else
    {
        uiBtn->loadTextureNormal("Assets/ui/mission/mission_off.png");
    }
    
    //
    drawButtons();
}

#pragma mark - game

