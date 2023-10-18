//
//  PopupStone.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2022/11/23.
//

#include "PopupStone.h"

#include "Common/ConfigGameNormal.h"

#include "GameUI/UI/UIReddot.h"
#include "GameUI/UI/UICurrency.h"
#include "GameUIPopup/Common/PopupHelp.h"
#include "GameUIPopup/Other/Stone/PopupStoneBuy.h"
#include "GameUIPopup/Other/Stone/PopupStoneStorageBuy.h"

#include "ManagerGame/StoneManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupStone::PopupStone():
_bInitUI(false),

//

//
_uiContentsInfo(nullptr),
_uiContentsList(nullptr),

_uiList(nullptr)
{
    
}

PopupStone::~PopupStone(void)
{
    
}

bool PopupStone::init()
{
    if (!PopupBase::init())
    {
        return false;
    }
    
    return true;
}

void PopupStone::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        //
        initVar();
        initUI();
        
        RefreshManager::getInstance()->addUI(E_REFRESH::STONE_BUY, this);
        RefreshManager::getInstance()->addUI(E_REFRESH::STONE_STORAGE_BUY, this);
        
        //
        StoneManager::getInstance()->requestInfo(true, CC_CALLBACK_1(PopupStone::callbackInfo, this));
        
        //
        _bInitUI = true;
    }
    
    //
    drawInfo();
    drawList();
}

#pragma mark - refresh
void PopupStone::refresh(E_REFRESH eType, cocos2d::ValueMap listData)
{
    if ( eType == E_REFRESH::STONE_BUY || eType == E_REFRESH::STONE_STORAGE_BUY )
    {
        drawList();
        drawInfo();
    }
}

#pragma mark - override DelegateListView
void PopupStone::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size PopupStone::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 135;

    return Size(width, height);
}

void PopupStone::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 130);
    
    //
    E_TYPE eType = _listStone.at(idx);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiLayout->setPosition(Vec2(size.width / 2, size.height));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_1_s.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(10,60,10,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    if ( eType == E_TYPE::Storage )
    {
        listviewCellStorage(uiLayout);
    }
    else if ( eType == E_TYPE::Default_Attack || eType == E_TYPE::Default_Gold || eType == E_TYPE::Default_Key )
    {
        E_STONE eStone = E_STONE::NONE;
        switch (eType) {
            case E_TYPE::Default_Attack:        eStone = E_STONE::ATTACK; break;
            case E_TYPE::Default_Gold:          eStone = E_STONE::GOLD; break;
            case E_TYPE::Default_Key:           eStone = E_STONE::KEY; break;
                
            default:
                break;
        }
        
        if ( eStone == E_STONE::NONE)
            return;
        
        listviewCellStone(uiLayout, eStone);
    }
    else
    {
        E_STONE eStone = E_STONE::NONE;
        switch (eType) {
            case E_TYPE::Force_Attack:          eStone = E_STONE::FORCE_ATTACK; break;
            case E_TYPE::Force_Gold:            eStone = E_STONE::FORCE_GOLD; break;
            case E_TYPE::Force_Key:             eStone = E_STONE::FORCE_KEY; break;
            case E_TYPE::Force_2_Attack:        eStone = E_STONE::FORCE_2_ATTACK; break;
                
            default:
                break;
        }
        
        if ( eStone == E_STONE::NONE)
            return;
        
        listviewCellStoneForce(uiLayout, eStone);
    }
}

void PopupStone::listviewCellStorage(cocos2d::ui::Layout *uiLayout)
{
    Size size = uiLayout->getContentSize();
    
    int countAll = StoneManager::getInstance()->getCountAll();
    int storageCountNow = StoneManager::getInstance()->getStorageCountNow();
    
    auto pathIcon = StoneManager::getInstance()->getStoragePath();
    
    auto name = StoneManager::getInstance()->getStorageName();
    auto desc = GAME_TEXTFORMAT("t_ui_costume_stone_pocket", countAll, storageCountNow);
    
    auto itemNeed = StoneManager::getInstance()->getStoragePrice();
    
    auto earnNext = std::string("+1");
    
    //
    auto uiIconBG = ui::ImageView::create("Assets/ui_common/box_quest.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, size.height / 2));
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create("Assets/icon/icon_stone.png");
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIconBG->addChild(uiIcon);
    }
    
    //
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(posTextX, size.height - 25.5));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(desc, GAME_FONT, 22);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc->setPosition(Vec2(posTextX, 39.5));
    uiLayout->addChild(uiDesc);
    if ( countAll > storageCountNow )
    {
        uiDesc->setTextColor(Color4B(COLOR_GEM));
    }

    auto uiBuy = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    uiBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBuy->setPosition(Vec2(size.width, size.height / 2));
    uiBuy->addClickEventListener(CC_CALLBACK_1(PopupStone::onClickBuyStorage, this));
    uiLayout->addChild(uiBuy);
    {
        auto uiItemIcon = ui::ImageView::create("Assets/icon/icon_jewel.png");
        uiItemIcon->setPosition(Vec2(uiBuy->getContentSize().width * 0.175, uiBuy->getContentSize().height * 0.3));
        uiBuy->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(MafUtils::toString(itemNeed), GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiBuy->getContentSize().width * 0.35, uiBuy->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(95, uiItemCount->getContentSize().height));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiItemCount);
        
        auto uiNext = ui::CText::create(earnNext, GAME_FONT_AL, 24);
        uiNext->setPosition(Vec2(uiBuy->getContentSize().width * 0.5, uiBuy->getContentSize().height * 0.75));
        uiNext->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiNext->getContentSize().height));
        uiNext->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiNext->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiNext->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiNext);
    }
}

void PopupStone::listviewCellStone(cocos2d::ui::Layout *uiLayout, E_STONE eStone)
{
    Size size = uiLayout->getContentSize();
    
    int countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStone);
    int countMax = StoneManager::getInstance()->getCountMax(eStone);
    
    int level = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStone);
    double effectPrev = StoneManager::getInstance()->getEffect(eStone, level) * 100;
    double effectNext = StoneManager::getInstance()->getEffect(eStone, level + 1) * 100;
    double effectDiff = effectNext - effectPrev;
    
    std::string effectPrevStr = MafUtils::doubleToString(effectPrev);
    std::string effectDiffStr = MafUtils::doubleToString(effectDiff);
    MafUtils::convertNumberToShort(effectPrevStr);
    MafUtils::convertNumberToShort(effectDiffStr);
    
    auto pathIcon = StoneManager::getInstance()->getPath(eStone);
    
    auto name = StoneManager::getInstance()->getName(eStone);
    name = MafUtils::format("%s x%d", name.c_str(), countNow);
    
    auto desc = StoneManager::getInstance()->getEffectName(eStone);
    desc = MafUtils::format(desc.c_str(), effectPrevStr.c_str());
    
    auto itemNeed = StoneManager::getInstance()->getPrice(eStone, level + 1);
    
    auto earnNext = MafUtils::format("+%s%%", effectDiffStr.c_str());
    
    //
    auto uiIconBG = ui::ImageView::create("Assets/ui_common/box_quest.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, size.height / 2));
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIconBG->addChild(uiIcon);
    }
    
    //
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(posTextX, size.height - 25.5));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(desc, GAME_FONT, 22);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc->setPosition(Vec2(posTextX, 39.5));
    uiLayout->addChild(uiDesc);
    
    auto uiBuy = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    uiBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBuy->setPosition(Vec2(size.width, size.height / 2));
    uiBuy->addClickEventListener(CC_CALLBACK_1(PopupStone::onClickBuyStone, this));
    uiBuy->setTag((int)eStone);
    uiLayout->addChild(uiBuy);
    if ( countNow >= countMax )
    {
        uiBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
        uiBuy->setEnabled(false);
        
        auto uiText = ui::CText::create("MAX", GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBuy->getContentSize().width / 2, uiBuy->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBuy->addChild(uiText);
    }
    else
    {
        auto uiItemIcon = ui::ImageView::create("Assets/icon/icon_coin_2.png");
        uiItemIcon->setPosition(Vec2(uiBuy->getContentSize().width * 0.175, uiBuy->getContentSize().height * 0.3));
        uiBuy->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(MafUtils::toString(itemNeed), GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiBuy->getContentSize().width * 0.35, uiBuy->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(95, uiItemCount->getContentSize().height));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiItemCount);
        
        auto uiNext = ui::CText::create(earnNext, GAME_FONT_AL, 24);
        uiNext->setPosition(Vec2(uiBuy->getContentSize().width * 0.5, uiBuy->getContentSize().height * 0.75));
        uiNext->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiNext->getContentSize().height));
        uiNext->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiNext->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiNext->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiNext);
    }
}

void PopupStone::listviewCellStoneForce(cocos2d::ui::Layout *uiLayout, E_STONE eStone)
{
    Size size = uiLayout->getContentSize();
    
    int countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStone);
    int countMax = StoneManager::getInstance()->getCountMax(eStone);
    
    int level = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStone);
    double effectPrev = StoneManager::getInstance()->getEffect(eStone, level) * 100;
    double effectNext = StoneManager::getInstance()->getEffect(eStone, level + 1) * 100;
    double effectDiff = effectNext - effectPrev;
    
    std::string effectPrevStr = MafUtils::doubleToString(effectPrev);
    std::string effectDiffStr = MafUtils::doubleToString(effectDiff);
    MafUtils::convertNumberToShort(effectPrevStr);
    MafUtils::convertNumberToShort(effectDiffStr);
    
    auto pathIcon = StoneManager::getInstance()->getPath(eStone);
    
    auto name = StoneManager::getInstance()->getName(eStone);
    name = MafUtils::format("%s x%d", name.c_str(), countNow / 6);
    
    auto desc = StoneManager::getInstance()->getEffectName(eStone);
    desc = MafUtils::format(desc.c_str(), effectPrevStr.c_str());
    desc.append("\n");
    if ( countNow < countMax && (countNow + 1) % 6 == 0 )
    {
        desc.append("{#ffd800:");
        desc.append(GAME_TEXTFORMAT("t_ui_costume_stone_upgrade_1", countNow % 6));
        desc.append(" ").append(GAME_TEXT("t_ui_costume_stone_upgrade_2"));
        desc.append("}");
    }
    else
    {
        desc.append(GAME_TEXTFORMAT("t_ui_costume_stone_upgrade_1", countNow % 6));
    }
    
    auto itemNeed = StoneManager::getInstance()->getPrice(eStone, level + 1);
    
    auto earnNext = MafUtils::format("+%s%%", effectDiffStr.c_str());
    
    //
    auto uiIconBG = ui::ImageView::create("Assets/ui_common/box_quest.png");
    uiIconBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIconBG->setPosition(Vec2(3, size.height / 2));
    uiLayout->addChild(uiIconBG);
    {
        auto uiIcon = ui::ImageView::create(pathIcon);
        uiIcon->setPosition(Vec2(uiIconBG->getContentSize().width / 2, uiIconBG->getContentSize().height / 2));
        uiIcon->setScale(2);
        ((ui::Scale9Sprite*)uiIcon->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
        uiIconBG->addChild(uiIcon);
    }
    
    //
    double posTextX = uiIconBG->getPositionX() + uiIconBG->getContentSize().width + 10;
    
    auto uiName = ui::CText::create(name, GAME_FONT, 26);
    uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiName->setPosition(Vec2(posTextX, size.height - 25.5));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiDesc = ui::CText::create(desc, GAME_FONT, 22);
    uiDesc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiDesc->setPosition(Vec2(posTextX, 39.5));
    uiLayout->addChild(uiDesc);
    
    auto uiBuy = ui::Button::create("Assets/ui_common/btn_upgrade_bg_2.png");
    uiBuy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBuy->setPosition(Vec2(size.width, size.height / 2));
    uiBuy->addClickEventListener(CC_CALLBACK_1(PopupStone::onClickBuyStone, this));
    uiBuy->setTag((int)eStone);
    uiLayout->addChild(uiBuy);
    if ( countNow >= countMax )
    {
        uiBuy->loadTextureNormal("Assets/ui_common/btn_upgrade_bg_2_max.png");
        uiBuy->setEnabled(false);
        
        auto uiText = ui::CText::create("MAX", GAME_FONT, 24);
        uiText->setPosition(Vec2(uiBuy->getContentSize().width / 2, uiBuy->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiBuy->addChild(uiText);
    }
    else
    {
        auto pathItemIcon = std::string("Assets/icon/icon_coin_2.png");
        if ( (countNow + 1) % 6 == 0 )
        {
            pathItemIcon = "Assets/icon/evilcrystal.png";
        }
            
        auto uiItemIcon = ui::ImageView::create(pathItemIcon);
        uiItemIcon->setPosition(Vec2(uiBuy->getContentSize().width * 0.175, uiBuy->getContentSize().height * 0.3));
        uiBuy->addChild(uiItemIcon);

        auto uiItemCount = ui::CText::create(MafUtils::toString(itemNeed), GAME_FONT_AL, 30);
        uiItemCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiItemCount->setPosition(Vec2(uiBuy->getContentSize().width * 0.35, uiBuy->getContentSize().height * 0.3));
        uiItemCount->setTextColor(Color4B(COLOR_COIN));
        uiItemCount->setTextAreaSize(Size(95, uiItemCount->getContentSize().height));
        uiItemCount->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiItemCount->setTextHorizontalAlignment(TextHAlignment::LEFT);
        uiItemCount->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiItemCount);
        
        auto uiNext = ui::CText::create(earnNext, GAME_FONT_AL, 24);
        uiNext->setPosition(Vec2(uiBuy->getContentSize().width * 0.5, uiBuy->getContentSize().height * 0.75));
        uiNext->setTextAreaSize(Size(uiBuy->getContentSize().width - 10, uiNext->getContentSize().height));
        uiNext->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiNext->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiNext->setTextOverflow(Label::Overflow::SHRINK);
        uiBuy->addChild(uiNext);
    }
}

int64_t PopupStone::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listStone.size();
    
    return count;
}

#pragma mark - ui
void PopupStone::initVar()
{
    _listStone.push_back(E_TYPE::Storage);
    _listStone.push_back(E_TYPE::Default_Attack);
    _listStone.push_back(E_TYPE::Default_Gold);
    _listStone.push_back(E_TYPE::Default_Key);
    _listStone.push_back(E_TYPE::Force_Attack);
    _listStone.push_back(E_TYPE::Force_Gold);
    _listStone.push_back(E_TYPE::Force_Key);
    _listStone.push_back(E_TYPE::Force_2_Attack);
}

void PopupStone::initUI()
{
    // size
    _spriteContainer->setContentSize( Size(738, 1316) );
    
    // label title
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_stone_special_info_title_1"), GAME_FONT, 32);
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
    uiClose->addClickEventListener(CC_CALLBACK_1(PopupStone::onClickClose, this));
    _spriteContainer->addChild(uiClose);
    
    //
    auto uiHelp = ui::Button::create("Assets/ui_common/icon_info_1_3.png");
    uiHelp->setPosition(Vec2(55, _spriteContainer->getContentSize().height - 37.5));
    uiHelp->addClickEventListener(CC_CALLBACK_1(PopupStone::onClickHelp, this));
    _spriteContainer->addChild(uiHelp);
    
    
    //
    std::vector<E_ITEMS> list;
    list.push_back(E_ITEMS::COSTUME_COIN);
    list.push_back(E_ITEMS::GEM);
    list.push_back(E_ITEMS::MATERIAL);
    
    auto uiCurrency = UICurrency::create(list, Size(_spriteContainer->getContentSize().width - 10, 58));
    uiCurrency->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiCurrency->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(uiCurrency);
    
    //
    uiContainer();
    uiInfo();
    uiList();
}

void PopupStone::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = size.width - 10;
    size.height = size.height - 75 - 58;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2, 0));
    _spriteContainer->addChild(layout);
    
    // 184 * 241
    _uiContentsInfo = ui::Layout::create();
    _uiContentsInfo->setContentSize(Size(size.width, 168));
    _uiContentsInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContentsInfo->setPosition(Vec2(size.width / 2, 0));
    layout->addChild(_uiContentsInfo);
    
    _uiContentsList = ui::Layout::create();
    _uiContentsList->setContentSize( Size(size.width - 10, size.height - _uiContentsInfo->getContentSize().height) );
    _uiContentsList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsList->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContentsList);
}
 
void PopupStone::uiInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/popup_mini_scale2_5.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(35,60,10,10));
    uiBG->setContentSize(size);
    uiBG->setPosition(Vec2(size.width / 2, size.height / 2));
    layout->addChild(uiBG);
    
    auto uiTitle = ui::CText::create(GAME_TEXT("t_ui_total_stat_1"), GAME_FONT, 26);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiTitle->setPosition(Vec2(35, layout->getContentSize().height - 28.5));
    uiTitle->setTextAreaSize(Size(layout->getContentSize().width - 10, uiTitle->getContentSize().height));
    uiTitle->setTextOverflow(Label::Overflow::SHRINK);
    uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiTitle->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiTitle->setTextColor(Color4B(COLOR_TITLE));
    layout->addChild(uiTitle);
    
    auto uiInfoBG = ui::Layout::create();
    uiInfoBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiInfoBG->setContentSize(Size(size.width - 70, size.height - 54));
    uiInfoBG->setPosition(Vec2(size.width / 2, size.height - 54));
    uiInfoBG->setLayoutType(ui::Layout::Type::VERTICAL);
    uiInfoBG->setName("UI_INFO_BG");
    layout->addChild(uiInfoBG);
}

void PopupStone::uiList()
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
void PopupStone::drawInfo()
{
    auto layout = _uiContentsInfo;
    auto size = layout->getContentSize();
    
    if ( auto uiInfoBG = utils::findChild<ui::Layout*>(layout, "UI_INFO_BG"); uiInfoBG != nullptr )
    {
        uiInfoBG->removeAllChildren();
       
        //
        auto uiInfoAttackBG = ui::Layout::create();
        uiInfoAttackBG->setContentSize(Size(uiInfoBG->getContentSize().width, 30));
        uiInfoBG->addChild(uiInfoAttackBG);
        {
            std::string value = "0";
            double stoneBuff = 0.0;
            
            stoneBuff = StoneManager::getInstance()->getEffect(E_STONE::ATTACK) * 100;
            value = MafUtils::doubleToString(stoneBuff);
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_ATTACK);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_ATTACK);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
                
            value = MafUtils::convertNumberToShort(value);
            
            auto uiName = ui::CText::create(GAME_TEXT("t_ui_costume_stone_total_stat_1"), GAME_FONT, 20);
            uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiName->setPosition(Vec2(0, uiInfoAttackBG->getContentSize().height / 2));
            uiInfoAttackBG->addChild(uiName);
            
            auto uiValue = ui::CText::create(MafUtils::format("%s%%", value.c_str()), GAME_FONT, 20);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiInfoAttackBG->getContentSize().width, uiInfoAttackBG->getContentSize().height / 2));
            uiInfoAttackBG->addChild(uiValue);
        }
        
        //
        auto uiInfoGoldBG = ui::Layout::create();
        uiInfoGoldBG->setContentSize(Size(uiInfoBG->getContentSize().width, 30));
        uiInfoBG->addChild(uiInfoGoldBG);
        {
            std::string value = "0";
            double stoneBuff = 0.0;
            
            stoneBuff = StoneManager::getInstance()->getEffect(E_STONE::GOLD) * 100;
            value = MafUtils::doubleToString(stoneBuff);
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_GOLD);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_GOLD);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
                
            value = MafUtils::convertNumberToShort(value);
            
            auto uiName = ui::CText::create(GAME_TEXT("t_ui_costume_stone_total_stat_2"), GAME_FONT, 20);
            uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiName->setPosition(Vec2(0, uiInfoGoldBG->getContentSize().height / 2));
            uiInfoGoldBG->addChild(uiName);
            
            auto uiValue = ui::CText::create(MafUtils::format("%s%%", value.c_str()), GAME_FONT, 20);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiInfoGoldBG->getContentSize().width, uiInfoGoldBG->getContentSize().height / 2));
            uiInfoGoldBG->addChild(uiValue);
        }
        
        //
        auto uiInfoKeyBG = ui::Layout::create();
        uiInfoKeyBG->setContentSize(Size(uiInfoBG->getContentSize().width, 30));
        uiInfoBG->addChild(uiInfoKeyBG);
        {
            std::string value = "0";
            double stoneBuff = 0.0;
            
            stoneBuff = StoneManager::getInstance()->getEffect(E_STONE::KEY) * 100;
            value = MafUtils::doubleToString(stoneBuff);
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_KEY);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
            
            stoneBuff = 1.0f;
            stoneBuff += StoneManager::getInstance()->getEffect(E_STONE::FORCE_2_KEY);
            stoneBuff *= 100;
            if ( stoneBuff > 100 )
            {
                value = MafUtils::bigMulNum(value, MafUtils::doubleToString(stoneBuff));
                value.pop_back();
                value.pop_back();
            }
                
            value = MafUtils::convertNumberToShort(value);
            
            auto uiName = ui::CText::create(GAME_TEXT("t_ui_costume_stone_total_stat_3"), GAME_FONT, 20);
            uiName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            uiName->setPosition(Vec2(0, uiInfoKeyBG->getContentSize().height / 2));
            uiInfoKeyBG->addChild(uiName);
            
            auto uiValue = ui::CText::create(MafUtils::format("%s%%", value.c_str()), GAME_FONT, 20);
            uiValue->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            uiValue->setPosition(Vec2(uiInfoKeyBG->getContentSize().width, uiInfoKeyBG->getContentSize().height / 2));
            uiInfoKeyBG->addChild(uiValue);
        }
    }
}

void PopupStone::drawList()
{
    auto layout = _uiContentsList;
    
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - click
void PopupStone::onClickClose(Ref* sender)
{
    hide();
}

void PopupStone::onClickHelp(Ref* sender)
{
    auto popup = PopupHelp::create(1, "t_ui_stone_special_info_title_%d", "t_ui_stone_special_info_msg_%d");
    popup->show();
}

void PopupStone::onClickBuyStorage(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto popup = PopupStoneStorageBuy::create();
    popup->show();
}

void PopupStone::onClickBuyStone(Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;
    
    E_STONE eStone = (E_STONE)uiButton->getTag();
    
    
    if ( eStone == E_STONE::FORCE_ATTACK )
    {
        E_STONE eStonePrev = E_STONE::ATTACK;
        auto name = StoneManager::getInstance()->getName(eStonePrev);
        auto countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStonePrev);
        auto countMax = StoneManager::getInstance()->getCountMax(eStonePrev);
        
        if ( countNow < countMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_costume_stone_qualification_stap_1", name.c_str(), countMax));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    else if ( eStone == E_STONE::FORCE_GOLD )
    {
        E_STONE eStonePrev = E_STONE::GOLD;
        auto name = StoneManager::getInstance()->getName(eStonePrev);
        auto countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStonePrev);
        auto countMax = StoneManager::getInstance()->getCountMax(eStonePrev);
        
        if ( countNow < countMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_costume_stone_qualification_stap_1", name.c_str(), countMax));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    else if ( eStone == E_STONE::FORCE_KEY )
    {
        E_STONE eStonePrev = E_STONE::KEY;
        auto name = StoneManager::getInstance()->getName(eStonePrev);
        auto countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStonePrev);
        auto countMax = StoneManager::getInstance()->getCountMax(eStonePrev);
        
        if ( countNow < countMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_costume_stone_qualification_stap_1", name.c_str(), countMax));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    else if ( eStone == E_STONE::FORCE_2_ATTACK )
    {
        E_STONE eStonePrev = E_STONE::FORCE_ATTACK;
        auto name = StoneManager::getInstance()->getName(eStonePrev);
        auto countNow = StoneManager::getInstance()->getCountNow(E_STONE_TYPE::ALL, eStonePrev);
        auto countMax = StoneManager::getInstance()->getCountMax(eStonePrev);
        
        if ( countNow < countMax )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_costume_stone_qualification_stap_2", name.c_str()));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    
    auto popup = PopupStoneBuy::create();
    popup->setStone(eStone);
    popup->show();
}

#pragma mark - callback
void PopupStone::callbackInfo(bool bResult)
{
    if ( bResult == false )
    {
        std::string strMessage = GAME_TEXT("t_ui_error_30");

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    
    drawInfo();
    drawList();
}
