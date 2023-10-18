//
//  LayerFrostList.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "LayerFrostList.h"

#include "Common/ConfigGameNormal.h"

#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/PrisonFrostManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

LayerFrostList* LayerFrostList::create(cocos2d::Size size)
{
    LayerFrostList* pRet = new(std::nothrow) LayerFrostList();
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

LayerFrostList::LayerFrostList(void) :
_bInitUI(false),

_levelUPType(1),

// widget
_uiContentsUIDefault(nullptr),
_uiContentsUIButtons(nullptr),
_uiList(nullptr)

{
    
}

LayerFrostList::~LayerFrostList(void)
{
    
}

bool LayerFrostList::init(cocos2d::Size size)
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

    return true;
}

void LayerFrostList::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiButton();
    }
    
    //
    drawButton();
    drawList();
}

void LayerFrostList::onSizeChanged()
{
    ui::Layout::onSizeChanged();
    
    if ( _bInitUI == true )
    {
        drawList();
    }
}

#pragma mark - init
void LayerFrostList::initVar()
{
    _listSkill = PrisonFrostManager::getInstance()->getListSkill();
}

void LayerFrostList::initUi()
{
    auto size = getContentSize();
    
    
}

#pragma mark - override DelegateListView
void LayerFrostList::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size LayerFrostList::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 106 + 5;

    return Size(width, height);
}

void LayerFrostList::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();

    // data
    auto obj = _listSkill.at(idx);
    
    int step = PrisonFrostManager::getInstance()->getSkillStep(obj->getIdx());
    int level = PrisonFrostManager::getInstance()->getSkillLevel(obj->getIdx());
    int levelMax = obj->getLevelMax();
    int levelPlus = level + _levelUPType;
    if ( levelPlus > levelMax )
    {
        levelPlus = levelMax;
    }
    
    int cost = 0;
    if ( level < levelMax )
    {
        cost = PrisonFrostManager::getInstance()->getSkillLevelCost(obj->getIdx(), level + 1, levelPlus);
    }
    
    double effect = PrisonFrostManager::getInstance()->getSkillEffect(obj->getIdx(), level);
    
    std::string textLevel = UtilsString::getInstance()->format("Lv. %d(Max %d)", level, obj->getLevelMax());
    if ( step != -1 )
    {
        auto objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
        if ( objPass != nullptr && objPass->isBuyPass() == true )
        {
            time_t timeDiff = objPass->getResetTime() - UtilsDate::getInstance()->getTime();
            if ( timeDiff > 0 )
            {
                textLevel.append(MafUtils::format("{#41FF32:(Pass+%d)}", PrisonFrostManager::getInstance()->getInfoSetting("skill_pass_bonus")->getValueToInt()));
            }
        }
    }
    
    std::string textContents = obj->getContents();
    if ( obj->getIdx() == 50002 )
    {
        textContents = UtilsString::getInstance()->format("%s\n%.1lf%%", obj->getContents().c_str(), effect);
    }
    else
    {
        std::string effectString = MafUtils::doubleToString(effect);
        effectString = MafUtils::convertNumberToShort(effectString);
        textContents = UtilsString::getInstance()->format("%s\n%s%%", obj->getContents().c_str(), effectString.c_str());
    }
    
    
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(size.width, size.height - 5));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    auto uiBG = ui::ImageView::create("Assets/ui_common/list_bg_s_2_1.png");
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(25,50,1,1));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    // icon
    auto uiIcon = ui::ImageView::create(obj->getIconPath());
    uiIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiIcon->setPosition(Vec2(13, uiLayout->getContentSize().height / 2));
    uiIcon->setScale(MAX(84 / uiIcon->getContentSize().width, 84 / uiIcon->getContentSize().height));
    uiLayout->addChild(uiIcon);
    
    // level (x:114, y:23)
    auto uiLevel = ui::CText::create(textLevel, GAME_FONT, 24);
    uiLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiLevel->setPosition(Vec2(114, uiLayout->getContentSize().height - 23));
    uiLevel->setTextColor(Color4B(115, 220, 245, 255));
    uiLayout->addChild(uiLevel);
    
    // contents (x:114)
    auto uiContents = ui::CText::create(textContents, GAME_FONT, 22);
    uiContents->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiContents->setPosition(Vec2(114, 30));
    uiContents->setTextColor(Color4B(185, 220, 250, 255));
    uiContents->setLineSpacing(-10);
    uiContents->setTextAreaSize(Size(400, uiContents->getContentSize().height));
    uiContents->setTextOverflow(Label::Overflow::SHRINK);
    uiContents->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiContents->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiLayout->addChild(uiContents);
    
    //
    auto uiBtnLevel = ui::Button::create("Assets/ui_common/btn_blue.png");
    uiBtnLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiBtnLevel->setPosition(Vec2(uiLayout->getContentSize().width - 10, uiLayout->getContentSize().height / 2));
    uiBtnLevel->addClickEventListener(CC_CALLBACK_1(LayerFrostList::onClickLevelUP, this));
    uiBtnLevel->setTag((int)idx);
    uiLayout->addChild(uiBtnLevel);
    {
        if ( step == -1 )
        {
            uiBtnLevel->loadTextureNormal("Assets/ui_common/btn_ad_gray.png");
        }
        
        auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::FROST_PRISON_POINT);
        if ( objItem != nullptr )
        {
            objItem->setCount(cost);

            auto uiLevelCost = UtilsWidget::getInstance()->createIconText(objItem->getPath(), objItem->getCount(), uiBtnLevel->getContentSize().width - 10, 24, 0);
            uiLevelCost->setPosition(Vec2(uiBtnLevel->getContentSize().width / 2, uiBtnLevel->getContentSize().height - 30));
            uiBtnLevel->addChild(uiLevelCost);
        }
        
        auto uiLevelText = ui::CText::create("Level UP", GAME_FONT, 24);
        uiLevelText->setPosition(Vec2(uiBtnLevel->getContentSize().width / 2, 30));
        uiBtnLevel->addChild(uiLevelText);
    }
    
    if ( level >= levelMax )
    {
        uiBtnLevel->setVisible(false);
    }
    
    
    // 미획득
    if ( step == -1 )
    {
        auto uiLayoutBlack = ui::Layout::create();
        uiLayoutBlack->setContentSize(uiLayout->getContentSize());
        uiLayoutBlack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        uiLayoutBlack->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
        uiLayoutBlack->setBackGroundColor(Color3B::BLACK);
        uiLayoutBlack->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        uiLayoutBlack->setOpacity(200);
        uiLayout->addChild(uiLayoutBlack);
        
        
        auto uiText = ui::CText::create(GAME_TEXT(MafUtils::format("t_ui_frost_prison_sikll_null_%d", obj->getIdx())), GAME_FONT, 30);
        uiText->setPosition(Vec2(uiLayoutBlack->getContentSize().width / 2, uiLayoutBlack->getContentSize().height / 2));
        uiText->setTextColor(Color4B(245, 130, 15, 255));
        uiText->setTextAreaSize(Size(uiText->getContentSize().width - 20, uiText->getContentSize().height - 10));
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiLayoutBlack->addChild(uiText);
        
    }
}

int64_t LayerFrostList::listviewNumberOfCells(ui::CListView *listview)
{
    int64_t count = _listSkill.size();
    return count;
}

#pragma mark - ui : defalut
void LayerFrostList::uiContainer()
{
    Size size = getContentSize();
    
    _uiContentsUIDefault = ui::Layout::create();
    _uiContentsUIDefault->setContentSize( Size(size.width, 80) );
    _uiContentsUIDefault->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIDefault->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIDefault->setPosition( Vec2(size.width / 2, size.height) );
    addChild(_uiContentsUIDefault);
    
    _uiContentsUIButtons = ui::Layout::create();
    _uiContentsUIButtons->setContentSize( Size(size.width, 60) );
    _uiContentsUIButtons->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIButtons->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContentsUIButtons->setPosition( Vec2(size.width / 2, size.height - _uiContentsUIDefault->getContentSize().height) );
    addChild(_uiContentsUIButtons);
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height - _uiContentsUIDefault->getContentSize().height - _uiContentsUIButtons->getContentSize().height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setBounceEnabled(true);
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    addChild(_uiList);
}

void LayerFrostList::uiInfo()
{
    auto layout = _uiContentsUIDefault;
    Size size = layout->getContentSize();
    
    auto uiIcon = ui::ImageView::create("Assets/ui/frost_prison/mamon_skill_icon.png");
    uiIcon->setPosition(Vec2(43, size.height / 2));
    layout->addChild(uiIcon);
    
    auto uiText = ui::CText::create(GAME_TEXT("t_ui_frost_prison_skill_title"), GAME_FONT, 32);
    uiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiText->setPosition(Vec2(85, size.height / 2));
    uiText->setTextColor(Color4B(215, 175, 95, 255));
    layout->addChild(uiText);
    
    auto uiButtonReset = ui::Button::create("Assets/ui/adventure/adventure_relic_info_btn_box.png");
    uiButtonReset->setScale9Enabled(true);
    uiButtonReset->setContentSize(Size(154, 52));
    uiButtonReset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiButtonReset->setPosition(Vec2(size.width - 10, size.height / 2));
    uiButtonReset->addClickEventListener(CC_CALLBACK_1(LayerFrostList::onClickReset, this));
    layout->addChild(uiButtonReset);
    {
        auto objItems = ItemsManager::getInstance()->getInfoItems(E_ITEMS::COSTUME_COIN);
        
        auto uiIcon = ui::ImageView::create(objItems->getPath());
        uiIcon->setPosition(Vec2(25, uiButtonReset->getContentSize().height / 2));
        uiButtonReset->addChild(uiIcon);
        
        auto uiCount = ui::CText::create(PrisonFrostManager::getInstance()->getInfoSetting("skill_reset_cost")->getValueToString(), GAME_FONT, 22);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        uiCount->setPosition(Vec2(50, uiButtonReset->getContentSize().height / 2));
        uiCount->setTextColor(Color4B(COLOR_COIN));
        uiButtonReset->addChild(uiCount);
        
        auto uiIconReset = ui::ImageView::create("Assets/ui/frost_prison/reset_icon.png");
        uiIconReset->setPosition(Vec2(uiButtonReset->getContentSize().width - 25, uiButtonReset->getContentSize().height / 2));
        uiButtonReset->addChild(uiIconReset);
    }
}

void LayerFrostList::uiButton()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    std::vector<int> listLevelUPType = {100, 10, 1};
    
    double widthPadding = 9;
    double widthButtons = -widthPadding;
    
    double widthAverage = size.width / listLevelUPType.size();
    if ( widthAverage > 234 )
    {
        widthAverage = 234;
    }

    for ( int i = 0; i < listLevelUPType.size(); i++ )
    {
        int levelUPType = listLevelUPType.at(i);
        std::string text = MafUtils::format("+Lv.%d", levelUPType);
        
        auto uiButton = ui::Button::create("Assets/ui_common/btn_1_1_on.png");
        uiButton->setScale9Enabled(true);
        uiButton->setContentSize(Size(widthAverage, 60));
        uiButton->setCapInsets(Rect(33,33,2,2));
        uiButton->addClickEventListener(CC_CALLBACK_1(LayerFrostList::onClickLevelUPType, this));
        uiButton->setTag(levelUPType);
        layout->addChild(uiButton);
        
        auto uiText = ui::CText::create(text, GAME_FONT, 24);
        uiText->setPosition(Vec2(uiButton->getContentSize().width / 2, uiButton->getContentSize().height / 2));
        uiText->setName("UI_TEXT");
        uiButton->addChild(uiText);
        
        widthButtons += uiButton->getContentSize().width + widthPadding;
    }
    
    float posX = (size.width / 2.0f) - (widthButtons / 2.0f);
    float posY = size.height / 2.0f;
    for (const auto &child : layout->getChildren())
    {
        child->setPosition(posX + child->getContentSize().width * child->getScaleX() / 2.0f, posY);
        posX += child->getContentSize().width * child->getScaleX() + widthPadding;
    }
}

#pragma mark - ui : draw
void LayerFrostList::drawButton()
{
    auto layout = _uiContentsUIButtons;
    Size size = layout->getContentSize();
    
    for ( auto& child : layout->getChildren() )
    {
        auto uiButton = (ui::Button*)child;
        auto uiText = uiButton->getChildByName<ui::CText*>("UI_TEXT");
        
        if ( _levelUPType == uiButton->getTag() )
        {
            uiButton->loadTextureNormal("Assets/ui_common/btn_1_1_on.png");
            uiText->setTextColor(Color4B::WHITE);
        }
        else
        {
            uiButton->loadTextureNormal("Assets/ui_common/btn_1_off.png");
            uiText->setTextColor(Color4B(115, 115, 115, 255));
        }
    }
    
    for ( int i = 0; i < layout->getChildrenCount(); i++ )
    {
        
    }
}

void LayerFrostList::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}

#pragma mark - ui : set, get

#pragma mark - ui : click
void LayerFrostList::onClickLevelUPType(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int levelUPType = uiBtn->getTag();
    if ( levelUPType == _levelUPType )
    {
        return;
    }
    
    _levelUPType = levelUPType;
    
    drawButton();
    drawList();
}

void LayerFrostList::onClickLevelUP(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto obj = _listSkill.at(uiBtn->getTag());
    
    int idx = obj->getIdx();
    int step = PrisonFrostManager::getInstance()->getSkillStep(idx);
    if ( step < 0 )
    {
        return;
    }
    
    int level = PrisonFrostManager::getInstance()->getSkillLevel(idx);
    int levelMax = obj->getLevelMax();
    
    int levelCount = _levelUPType;
    if ( level + _levelUPType > levelMax )
    {
        levelCount = levelMax - level;
    }
    
    
    PrisonFrostManager::getInstance()->requestSkillLevel(true, idx, levelCount);
}

void LayerFrostList::onClickReset(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    bool bReset = false;
    
    // check
    std::map<int, int> listSkillLevel = PrisonFrostManager::getInstance()->getSkillLevelAll();
    for (auto iter = listSkillLevel.begin() ; iter !=  listSkillLevel.end(); iter++)
    {
        int idx = iter->first;
        int level = iter->second;
        if ( level > 0 )
        {
            bReset = true;
            break;
        }
    }
    
    if ( bReset == false )
    {
        return;
    }
    
    //
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_frost_prison_skill_reset"));
    popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
    popup->addButton(GAME_TEXT("t_ui_yes"), false, CC_CALLBACK_1(LayerFrostList::onClickResetOk, this));
    popup->show();
}

void LayerFrostList::onClickResetOk(cocos2d::Ref* sender)
{
    PrisonFrostManager::getInstance()->requestSkillReset(true);
}

#pragma mark - callback

#pragma mark - schedule

#pragma mark - game
void LayerFrostList::onDrawList()
{
    _listSkill = PrisonFrostManager::getInstance()->getListSkill();
    
    //
    drawList();
}

