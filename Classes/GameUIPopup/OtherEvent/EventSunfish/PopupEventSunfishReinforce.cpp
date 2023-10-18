//
//  PopupEventSunfishReinforce.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupEventSunfishReinforce.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerEvent/EventSunfishManager.h"

USING_NS_CC;

PopupEventSunfishReinforce* PopupEventSunfishReinforce::create()
{
    PopupEventSunfishReinforce* pRet = new(std::nothrow) PopupEventSunfishReinforce();
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

PopupEventSunfishReinforce::PopupEventSunfishReinforce():
_bInitUI(false),

_eTab(E_TAB::FOOD),


//
_uiContentsInfo(nullptr),
_uiContentsTab(nullptr),
_uiContentsList(nullptr)
{
    
}

PopupEventSunfishReinforce::~PopupEventSunfishReinforce(void)
{
    
}

bool PopupEventSunfishReinforce::init()
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

void PopupEventSunfishReinforce::onEnter()
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
void PopupEventSunfishReinforce::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupEventSunfishReinforce::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 100;

    return Size(width, height);
}

void PopupEventSunfishReinforce::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 90);
    
    //
    auto obj = _listSunfish.at(idx);
    
    auto name = obj->getName();
    auto level = obj->getLevel();
    auto cost = obj->getCost();
    auto pathSkin = obj->getSkinPath();
    auto pathIcon = obj->getIconPath();
    
    auto bReinforce = EventSunfishManager::getInstance()->isReinforce(enumToInt(_eTab), level);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    childview->addChild(uiLayout);
    
    auto pathBG = "Assets/ui_common/base_box_1_4.png";
    auto pathIconBG = "Assets/ui_common/btn_2_9_on.png";
    if ( bReinforce == true )
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
    auto pathReinforce = "Assets/ui_common/box_05.png";
    auto textReinforce = GAME_TEXTFORMAT("t_ui_event_sunfish_point", cost);
    if ( bReinforce == true )
    {
        pathReinforce = "Assets/ui_common/box_05_1.png";
        textReinforce = GAME_TEXT("t_ui_event_sunfish_purchase_complete");
    }
    
    auto uiReinforce = ui::Button::create(pathReinforce);
    uiReinforce->setScale9Enabled(true);
    uiReinforce->setCapInsets(Rect(12, 12, 8, 8));
    uiReinforce->setContentSize(Size(186, 76));
    uiReinforce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiReinforce->setPosition(Vec2(sizeLayout.width - 15, sizeLayout.height/ 2));
    uiReinforce->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishReinforce::onClickReinforce, this));
    uiReinforce->setTag((int)idx);
    uiLayout->addChild(uiReinforce);
    {
        auto uiText = ui::CText::create(textReinforce, GAME_FONT, 24);
        uiText->setPosition(Vec2(uiReinforce->getContentSize().width / 2, uiReinforce->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiReinforce->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiReinforce->addChild(uiText);
    }
    
    //
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(120, sizeLayout.height / 2));
    uiName->setTextAreaSize(Size(sizeLayout.width - 120 - uiReinforce->getContentSize().width - 30, uiName->getContentSize().height));
    uiName->setTextOverflow(Label::Overflow::SHRINK);
    uiName->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiName->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiLayout->addChild(uiName);
}

int64_t PopupEventSunfishReinforce::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listSunfish.size();
    
    return count;
}

#pragma mark - ui
void PopupEventSunfishReinforce::initVar()
{
    //
    _listSunfish.clear();
    
    //
    auto listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish((EventSunfishManager::E_TYPE)_eTab);
    for ( auto& objSunfish : listSunfish )
    {
        if ( objSunfish->getCost() > 0 )
        {
            _listSunfish.pushBack(objSunfish);
        }
    }
}

void PopupEventSunfishReinforce::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(700, 818) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_event_sunfish_bt_2"), GAME_FONT, 32);
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
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishReinforce::onClickClose, this));
    _spriteContainer->addChild(uiClose);
}

void PopupEventSunfishReinforce::uiContainer()
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
 
void PopupEventSunfishReinforce::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/base_box_1_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    //
    _uiPoint = ui::CText::create(GAME_TEXTFORMAT("t_ui_event_sunfish_point", 0), GAME_FONT, 28);
    _uiPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _uiPoint->setPosition(Vec2(size.width - 15, size.height / 2));
    layout->addChild(_uiPoint);
}


void PopupEventSunfishReinforce::uiTab()
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
            uiButton->addClickEventListener(CC_CALLBACK_1(PopupEventSunfishReinforce::onClickTab, this));
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
        }
        
        auto sizeTabs = UtilsWidget::getInstance()->getLayoutAccumulatedSize(uiTabs);
        uiTabs->setContentSize(sizeTabs);
        uiTabs->requestDoLayout();
    }
}

void PopupEventSunfishReinforce::uiList()
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
void PopupEventSunfishReinforce::drawInfo()
{
    auto layout = _uiContentsInfo;
    
    
    auto point = EventSunfishManager::getInstance()->getGamePoint();
    _uiPoint->setString(GAME_TEXTFORMAT("t_ui_event_sunfish_point", point));
}

void PopupEventSunfishReinforce::drawTab()
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

void PopupEventSunfishReinforce::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - click
void PopupEventSunfishReinforce::onClickClose(Ref* sender)
{
    hide();
}

void PopupEventSunfishReinforce::onClickTab(Ref* sender)
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
    _listSunfish.clear();
    
    //
    auto listSunfish = EventSunfishManager::getInstance()->getListInfoSunfish((EventSunfishManager::E_TYPE)_eTab);
    for ( auto& objSunfish : listSunfish )
    {
        if ( objSunfish->getCost() > 0 )
        {
            _listSunfish.pushBack(objSunfish);
        }
    }
    
    // draw
    drawTab();
    drawList();
}

void PopupEventSunfishReinforce::onClickReinforce(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto idx = uiBtn->getTag();
    auto obj = _listSunfish.at(idx);
    
    auto type = enumToInt(_eTab);
    auto level = obj->getLevel();
    auto itemNeed = obj->getCost();
    auto itemNow = EventSunfishManager::getInstance()->getGamePoint();
    
    auto bReinforcePrev = EventSunfishManager::getInstance()->isReinforce(type, level - 1);
    if ( bReinforcePrev == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_event_sunfish_purchase_error"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    auto bReinforce = EventSunfishManager::getInstance()->isReinforce(type, level);
    if ( bReinforce == true )
    {
        return;
    }
    
    if ( itemNow < itemNeed )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_31"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    EventSunfishManager::getInstance()->requestReinforce(true, type, level, CC_CALLBACK_2(PopupEventSunfishReinforce::callbackReinforce, this));
}

#pragma mark - callback
void PopupEventSunfishReinforce::callbackReinforce(bool bResult, int nResult)
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
