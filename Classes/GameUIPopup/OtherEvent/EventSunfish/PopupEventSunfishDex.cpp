//
//  PopupEventSunfishDex.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupEventSunfishDex.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"

#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishDex* PopupEventSunfishDex::create()
{
    PopupEventSunfishDex* pRet = new(std::nothrow) PopupEventSunfishDex();
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

PopupEventSunfishDex::PopupEventSunfishDex():
_bInitUI(false),

_eTab(E_TAB::FOOD),


//
_uiContentsInfo(nullptr),
_uiContentsTab(nullptr),
_uiContentsList(nullptr)
{
    
}

PopupEventSunfishDex::~PopupEventSunfishDex(void)
{
    
}

bool PopupEventSunfishDex::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    //
    initVar();
    initUI();
    
    return true;
}

void PopupEventSunfishDex::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        uiContainer();
        uiInfo();
        uiTab();
        uiList();
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawTab();
    drawList();
}

#pragma mark - override DelegateListView
void PopupEventSunfishDex::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupEventSunfishDex::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 100;

    return Size(width, height);
}

void PopupEventSunfishDex::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 90);
    
    //
    auto obj = _listSunfish.at(idx);
    
    auto name = obj->getIllustratedTitleText();
    auto level = obj->getLevel();
    auto listReward = obj->getRewardDie();
    auto pathSkin = obj->getSkinPath();
    auto pathIcon = obj->getIconPath();
    
    auto bDie = EventSunfishManager::getInstance()->isDie(enumToInt(_eTab), level);
    auto bDieReward = EventSunfishManager::getInstance()->isDieReward(enumToInt(_eTab), level);
    
    if ( bDie == false )
    {
        name = "???";
    }
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    uiLayout->setTouchEnabled(true);
    uiLayout->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishDex::onClickDesc, this));
    uiLayout->setTag((int)idx);
    childview->addChild(uiLayout);
    
    auto pathBG = "Assets/ui_common/base_box_1_4.png";
    auto pathIconBG = "Assets/ui_common/btn_2_9_on.png";
    if ( bDie == true )
    {
        pathBG = "Assets/ui_common/base_box_1_20.png";
        pathIconBG = "Assets/ui_common/btn_2_6_on.png";
    }
    
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    //
    auto uiIconBG = ui::ImageView::create(pathIconBG);
    uiIconBG->setScale9Enabled(true);
    uiIconBG->setContentSize(Size(76, 76));
    uiIconBG->setPosition(Vec2(60, sizeLayout.height / 2));
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIconBG->addChild(uiIcon);
    }
    
    //
    auto uiRewards = ui::Layout::create();
    uiRewards->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiRewards->setPosition(Vec2(sizeLayout.width - 15, sizeLayout.height / 2));
    uiRewards->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiRewards->setTouchEnabled(true);
    uiRewards->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishDex::onClickReward, this));
    uiRewards->setTag((int)idx);
    uiLayout->addChild(uiRewards);
    {
        for ( int i = 0; i < listReward.size(); i++ )
        {
            auto objItem = listReward.at(i);
            
            auto uiItemBG = ui::ImageView::create("Assets/ui_common/btn_2_6_on.png");
            uiItemBG->setScale9Enabled(true);
            uiItemBG->setContentSize(Size(72, 72));
            uiRewards->addChild(uiItemBG);
            {
                if ( i != 0 )
                {
                    auto params = ui::LinearLayoutParameter::create();
                    params->setMargin(ui::Margin(10, 0, 0, 0));
                    uiItemBG->setLayoutParameter(params);
                }
                
                auto uiItemIcon = ui::ImageView::create(objItem->getPath());
                uiItemIcon->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2 + 5));
                uiItemBG->addChild(uiItemIcon, 10);
                
                std::string rewardCount = objItem->getCount();
                if ( objItem->isString() == true )
                {
                    MafUtils::convertNumberToShort(rewardCount);
                }
                
                auto uiItemCount = ui::CText::create(rewardCount, GAME_FONT, 20);
                uiItemCount->setPosition(Vec2(uiItemBG->getContentSize().width / 2, 10));
                uiItemCount->enableOutline(Color4B::BLACK, 2);
                uiItemBG->addChild(uiItemCount, 10);
            }
            
            // disable
            if ( bDieReward == true )
            {
                auto uiDisable = ui::Layout::create();
                uiDisable->setContentSize(uiItemBG->getContentSize());
                uiDisable->setBackGroundColor(Color3B::BLACK);
                uiDisable->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
                uiDisable->setOpacity(100);
                uiItemBG->addChild(uiDisable, 10);
                
                auto uiCheck = ui::ImageView::create("Assets/icon/icon_check_01.png");
                uiCheck->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiCheck, 10);
            }
            else if ( bDie == true )
            {
                auto uiActive = ui::ImageView::create("Assets/ui_common/btn_2_7_l_on.png");
                uiActive->setScale9Enabled(true);
                uiActive->setContentSize(uiItemBG->getContentSize());
                uiActive->setPosition(Vec2(uiItemBG->getContentSize().width / 2, uiItemBG->getContentSize().height / 2));
                uiItemBG->addChild(uiActive);
            }
        }
        
        auto sizeReward = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiRewards);
        uiRewards->setContentSize(sizeReward);
        uiRewards->requestDoLayout();
    }
    
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(120, sizeLayout.height / 2));
    uiName->setTextAreaSize(Size(sizeLayout.width - 120 - uiRewards->getContentSize().width - 30, uiName->getContentSize().height));
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiLayout->addChild(uiName);
    
    
}

int64_t PopupEventSunfishDex::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listSunfish.size();
    
    return count;
}

#pragma mark - ui
void PopupEventSunfishDex::initVar()
{
    _listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish((EventSunfishManager::E_TYPE)_eTab);
}

void PopupEventSunfishDex::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(700, 818) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_3"), GAME_FONT, 32);
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
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishDex::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

void PopupEventSunfishDex::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height  - 75 - 25;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 25));
    _spriteContainer->addChild(layout);
    
    // 184 * 241
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, 64));
    _uiContentsInfo->setIgnoreAnchorPointForPosition(false);
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsInfo);
    
    _uiContentsTab = ui::Layout::create();
    _uiContentsTab->setContentSize( Size(size.width - 10, 75) );
    _uiContentsTab->setIgnoreAnchorPointForPosition(false);
    _uiContentsTab->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsTab->setPosition( Vec2(size.width / 2, size.height - _uiContentsInfo->getContentSize().height) );
    layout->addChild(_uiContentsTab);
    
    _uiContentsList = ui::Layout::create();
    _uiContentsList->setContentSize( Size(size.width - 10, size.height - _uiContentsInfo->getContentSize().height - _uiContentsTab->getContentSize().height) );
    _uiContentsList->setIgnoreAnchorPointForPosition(false);
    _uiContentsList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsList->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContentsList);
}
 
void PopupEventSunfishDex::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
}


void PopupEventSunfishDex::uiTab()
{
    auto layout = _uiContentsTab;
    auto size = layout->getContentSize();
    
    //
    auto uiTabs = ui::Layout::create();
    uiTabs->setContentSize(Size(size.width, 72));
    uiTabs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    uiTabs->setPosition(Vec2(size.width / 2, 0));
    uiTabs->setLayoutType(ui::Layout::Type::HORIZONTAL);
    uiTabs->setName("UI_TAB");
    layout->addChild(uiTabs);
    {
        std::vector<E_TAB> listTab = {E_TAB::FOOD, E_TAB::ADVENTURE};
        std::vector<std::string> listTextKey = {"t_ui_event_sunfish_teb_bt_1", "t_ui_event_sunfish_teb_bt_2"};
        
        double widthButton = (size.width - ((listTab.size() - 1) * 10)) / listTab.size();
        double heightButton = 72;
        for ( int i = 0; i < listTab.size(); i++ )
        {
            E_TAB eType = listTab.at(i);
            
            auto uiButton = ui::Button::create("Assets/ui_common/btn_5_1.png");
            uiButton->setScale9Enabled(true);
            uiButton->setCapInsets(Rect(9, 9, 32, 32));
            uiButton->setContentSize(Size(widthButton, heightButton));
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishDex::onClickTab, this));
            uiButton->setTag((int)eType);
            uiTabs->addChild(uiButton);
            
            auto uiText = ui::CText::create(GAME_TEXT(listTextKey.at(i)), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
            uiButton->addChild(uiText);
            
            if ( i != 0 )
            {
                auto params = ui::LinearLayoutParameter::create();
                params->setMargin(ui::Margin(10, 0, 0, 0));
                uiButton->setLayoutParameter(params);
            }
            
            if ( eType == E_TAB::FOOD )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                uiReddot->setPosition(uiButton->getContentSize().width - 10, uiButton->getContentSize().height - 10);
                uiReddot->setVisible(false);
                uiButton->addChild(uiReddot);
                {
                    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_FOOD, uiReddot);
                }
            }
            else if ( eType == E_TAB::ADVENTURE )
            {
                auto uiReddot = UIReddot::create(E_REDDOT_TYPE::NOMAL, Vec2::ANCHOR_MIDDLE);
                uiReddot->setPosition(uiButton->getContentSize().width - 10, uiButton->getContentSize().height - 10);
                uiReddot->setVisible(false);
                uiButton->addChild(uiReddot);
                {
                    RefreshReddotManager::getInstance()->addUI(E_REFRESH_REDDOT::EVENT_SUNFISH_DEX_ADVENTURE, uiReddot);
                }
            }
        }
        
        auto sizeTabs = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiTabs);
        uiTabs->setContentSize(sizeTabs);
        uiTabs->requestDoLayout();
    }
}

void PopupEventSunfishDex::uiList()
{
    auto layout = _uiContentsList;
    auto size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(false);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void PopupEventSunfishDex::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    
    
    
}

void PopupEventSunfishDex::drawTab()
{
    auto layout = _uiContentsTab;
    
    //
    if ( auto uiTabs = layout->getChildByName<ui::Layout*>("UI_TAB"); uiTabs != nullptr )
    {
        auto listChild = uiTabs->getChildren();
        for ( auto& child : listChild )
        {
            auto uiButton = (ui::Button*)child;
            
            if ( (E_TAB)uiButton->getTag() == _eTab )
            {
                uiButton->loadTextureNormal("Assets/ui_common/btn_5_1.png");
            }
            else
            {
                uiButton->loadTextureNormal("Assets/ui_common/btn_5.png");
            }
        }
    }
}

void PopupEventSunfishDex::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - click
void PopupEventSunfishDex::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventSunfishDex::onClickTab(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    E_TAB eTab = enumFromInt<E_TAB>(uiBtn->getTag());
    if ( eTab == _eTab )
    {
        return;
    }
    
    //
    _eTab = eTab;
    
    //
    _listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish((EventSunfishManager::E_TYPE)_eTab);
    
    // draw
    drawTab();
    drawList();
}

void PopupEventSunfishDex::onClickReward(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    auto idx = uiLayout->getTag();
    auto obj = _listSunfish.at(idx);

    auto type = enumToInt(_eTab);
    auto level = obj->getLevel();

    auto bDie = EventSunfishManager::getInstance()->isDie(type, level);
    auto bDieReward = EventSunfishManager::getInstance()->isDieReward(type, level);
    if ( bDie == false || bDieReward == true )
    {
        return;
    }
    
    EventSunfishManager::getInstance()->requestReward(true, type, level, CC_CALLBACK_2(PopupEventSunfishDex::callbackReward, this));
}

void PopupEventSunfishDex::onClickDesc(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiLayout = dynamic_cast<ui::Layout*>(sender);
    
    auto idx = uiLayout->getTag();
    auto obj = _listSunfish.at(idx);
    
    auto type = enumToInt(_eTab);
    auto level = obj->getLevel();

    auto bDie = EventSunfishManager::getInstance()->isDie(type, level);
    if ( bDie == false )
    {
        return;
    }
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_event_sunfish_bt_3"), obj->getIllustratedText());
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}

#pragma mark - callback
void PopupEventSunfishDex::callbackReward(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 2 )
            strMessage = GAME_TEXT("t_ui_draw_msg_1");
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    drawInfo();
    drawList();
}
