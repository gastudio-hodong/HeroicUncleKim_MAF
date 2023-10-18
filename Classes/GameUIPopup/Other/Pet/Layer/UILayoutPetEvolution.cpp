//
//  UILayoutPetEvolution.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetEvolution.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"

#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayoutPetEvolution* UILayoutPetEvolution::create(E_PET ePet)
{
    UILayoutPetEvolution* pRet = new(std::nothrow) UILayoutPetEvolution();
    if ( pRet && pRet->init(ePet) )
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

UILayoutPetEvolution::UILayoutPetEvolution(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_ePet(E_PET::CHICK_1),

_uiContentsUIList(nullptr),
_uiList(nullptr)
{
    
}

UILayoutPetEvolution::~UILayoutPetEvolution(void)
{
    
}

bool UILayoutPetEvolution::init(E_PET ePet)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePlace = E_PLACE::DG_NORMAL;
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayoutPetEvolution::onEnter()
{
    ui::Layout::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiList();
    }
    
    // draw
    drawList();
}

#pragma mark - init
void UILayoutPetEvolution::initVar()
{
    _listEvolution.push_back(1);
    _listEvolution.push_back(2);
    _listEvolution.push_back(3);
    _listEvolution.push_back(4);
    _listEvolution.push_back(5);
    _listEvolution.push_back(6);
    _listEvolution.push_back(7);
    _listEvolution.push_back(8);
    _listEvolution.push_back(9);
    _listEvolution.push_back(10);
}

void UILayoutPetEvolution::initUi()
{
    
}

#pragma mark - override DelegateListView
void UILayoutPetEvolution::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
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

Size UILayoutPetEvolution::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 78 + 5;

    return Size(width, height);
}

void UILayoutPetEvolution::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 78);

    // data
    int reward = _listEvolution.at(idx);
    int evolutionLevel = PetNewManager::getInstance()->getPetEvolution(_ePet);
    int evolutionReward = PetNewManager::getInstance()->getPetEvolutionReward(_ePet);
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    std::string pathBG = "Assets/ui_common/list_bg_s_1_1.png";
    if ( reward == evolutionLevel )
    {
        pathBG = "Assets/ui_common/list_bg_s_1_2.png";
    }
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setScale9Enabled(true);
    uiBG->setCapInsets(Rect(10,40,10,10));
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
     
    //
    std::string strTitle = GAME_TEXTFORMAT("t_ui_pet_msg_5", reward);
    if ( reward == 1 )          strTitle = GAME_TEXTFORMAT("t_ui_pet_msg_5_1", reward);
    else if ( reward == 2 )     strTitle = GAME_TEXTFORMAT("t_ui_pet_msg_5_2", reward);
    else if ( reward == 3 )     strTitle = GAME_TEXTFORMAT("t_ui_pet_msg_5_3", reward);

    auto uiTitle = ui::CText::create(strTitle, GAME_FONT, 22);
    uiTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiTitle->setPosition(Vec2(20, sizeLayout.height - 17));
    uiLayout->addChild(uiTitle);
    if ( reward > evolutionLevel )           uiTitle->setColor(Color3B(128,128,128));


    //
    std::string strEffect = PetNewManager::getInstance()->getPetEvolutionExplain(_ePet, reward).c_str();

    auto uiEffect = ui::CText::create(strEffect, GAME_FONT, 24);
    uiEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    uiEffect->setPosition(Vec2(20, 26));
    uiEffect->setTextAreaSize(Size(500, uiEffect->getContentSize().height));
    uiEffect->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiEffect->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiEffect->setTextOverflow(Label::Overflow::SHRINK);
    uiEffect->setTextColor(Color4B(COLOR_TITLE));
    uiLayout->addChild(uiEffect);
    if ( reward > evolutionLevel )           uiEffect->setColor(Color3B(128,128,128));


    //
    std::string strFloor = GAME_TEXTFORMAT("t_ui_pet_msg_6_1", PetNewManager::getInstance()->getPetEvolutionFloor(_ePet, reward - 1) - 1);

    auto uiFloor = ui::CText::create(strFloor, GAME_FONT, 20);
    uiFloor->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiFloor->setPosition(Vec2(sizeLayout.width - 10, sizeLayout.height - 15));
    uiFloor->setTextColor(Color4B(89,166,195, 255));
    uiLayout->addChild(uiFloor);
    if ( reward == evolutionLevel )          uiFloor->setColor(Color3B(181,153,92));
    else if ( reward > evolutionLevel )      uiFloor->setColor(Color3B(128,128,128));

    if ( reward <= evolutionLevel )
    {
        if ( reward == evolutionReward + 1 )
        {
            auto objItem = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
            objItem->setCount(100);

            auto uiReward = ui::Button::create("Assets/ui/pet/btn_pet_reward_1_1.png");
            uiReward->setPosition(Vec2(sizeLayout.width - 71 - 10, 26));
            uiReward->addClickEventListener(CC_CALLBACK_1(UILayoutPetEvolution::onClickReward, this));
            uiReward->setTag((int)idx);
            uiLayout->addChild(uiReward);
            {
                auto uiItem = UtilsWidget::getInstance()->createIconText(objItem->getPath(), objItem->getCount(), uiReward->getContentSize().width - 10, 22, 5);
                uiItem->setPosition(Vec2(uiReward->getContentSize().width / 2 - 5, uiReward->getContentSize().height / 2));
                uiReward->addChild(uiItem);
            }
        }
        else if ( reward <= evolutionReward )
        {
            auto uiCheck = ui::ImageView::create("Assets/ui_common/btn_check_01.png");
            uiCheck->setPosition(Vec2(sizeLayout.width - 71 - 10, 26));
            uiLayout->addChild(uiCheck);
        }
    }
}

int64_t UILayoutPetEvolution::listviewNumberOfCells(ui::CListView *listview)
{
    return _listEvolution.size();
}


#pragma mark - ui
void UILayoutPetEvolution::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutPetEvolution::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    _uiList = ui::CListView::create(this, Size(size.width, size.height), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 0));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
}

#pragma mark - draw
void UILayoutPetEvolution::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}
 
#pragma mark - set, get, add


#pragma mark - callback
 

#pragma mark - click
void UILayoutPetEvolution::onClickReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    int reward = _listEvolution.at(uiBtn->getTag());
    
    int result = PetNewManager::getInstance()->onPetEvolutionReward(_ePet);
    if ( result != 0 )
    {
        return;
    }
    
    //
    drawList();
}
