//
//  UILayoutPetSkin.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 09/01/2020.
//

#include "UILayoutPetSkin.h"

#include "GameObject/InfoPetSkin.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/Equip/PopupWeaponPermanent.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

UILayoutPetSkin* UILayoutPetSkin::create(E_PET ePet)
{
    UILayoutPetSkin* pRet = new(std::nothrow) UILayoutPetSkin();
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

UILayoutPetSkin::UILayoutPetSkin(void) :
_bInitUI(false),

_ePlace(E_PLACE::EMPTY),
_ePet(E_PET::CHICK_1),

_uiContentsUIList(nullptr),

_uiList(nullptr),
_uiListDownArrow(nullptr)
{
    
}

UILayoutPetSkin::~UILayoutPetSkin(void)
{
    
}

bool UILayoutPetSkin::init(E_PET ePet)
{
    if ( !ui::Layout::init() )
    {
        return false;
    }
    
    //
    _ePet = ePet;
    
    // init
    initVar();
    initUi();
    
    return true;
}

void UILayoutPetSkin::onEnter()
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
void UILayoutPetSkin::initVar()
{
    _listPetSkin = PetNewManager::getInstance()->getListPetSkin(_ePet);
}

void UILayoutPetSkin::initUi()
{
    
}

#pragma mark - override DelegateListView
void UILayoutPetSkin::listviewScrollView(ui::CListView* listview, ui::ScrollView::EventType type)
{
    if ( type == ui::ScrollView::EventType::CONTAINER_MOVED )
    {
        int Opacity = listview->getContainerOffset().y * -2;
        if ( Opacity < 0 ) Opacity = 0;
        if ( Opacity > 255 ) Opacity = 255;
        _uiListDownArrow->setVisible(true);
        _uiListDownArrow->setOpacity(Opacity);
    }
    else if ( type == ui::ScrollView::EventType::SCROLLING_ENDED )
    {
        if ( listview->getContainerOffset().y >= 0 )
        {
            _uiListDownArrow->setVisible(false);
        }
        else
        {
            _uiListDownArrow->setVisible(true);
            _uiListDownArrow->setOpacity(255);
        }
    }
}

Size UILayoutPetSkin::listviewCellSize(ui::CListView* listview, ssize_t idx)
{
    double width = listview->getContentSize().width;
    double height = 122;

    return Size(width, height);
}

void UILayoutPetSkin::listviewCell(ui::CListView *listview, ui::Widget* childview, ssize_t idx)
{
    // size
    Size size = childview->getContentSize();
    Size sizeLayout = Size(size.width, 118);

    // data
    auto objPetSkin = _listPetSkin.at(idx);
    
    int skinEquip = PetNewManager::getInstance()->getPetSkin(_ePet);
    
    bool isSkinEqip = false;
    if ( objPetSkin->getIdx() == skinEquip )
    {
        isSkinEqip = true;
    }
    
    bool isSkinBuy = PetNewManager::getInstance()->isPetSkinBuy(_ePet, objPetSkin->getIdx());
    
    // ui
    childview->removeAllChildren();

    auto uiLayout = ui::Layout::create();
    uiLayout->setContentSize(Size(sizeLayout.width, sizeLayout.height));
    uiLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    uiLayout->setPosition(Vec2(size.width / 2, size.height / 2));
    childview->addChild(uiLayout);
    
    std::string pathBG = "Assets/ui/pet/pet_list_bg_1.png";
    if ( isSkinEqip )
    {
        pathBG = "Assets/ui/pet/pet_list_bg_2.png";
    }
    auto uiBG = ui::ImageView::create(pathBG);
    uiBG->setScale9Enabled(true);
    uiBG->setContentSize(uiLayout->getContentSize());
    uiBG->setPosition(Vec2(uiLayout->getContentSize().width / 2, uiLayout->getContentSize().height / 2));
    uiLayout->addChild(uiBG);
    
    //
    auto uiSkinBG = ui::Scale9Sprite::create("Assets/ui_common/box_item.png", Rect::ZERO, Rect(5,30,10,10));
    uiSkinBG->setContentSize(Size(82, 82));
    uiSkinBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiSkinBG->setPosition(102, uiLayout->getContentSize().height / 2);
    uiLayout->addChild(uiSkinBG);
    {
        auto uiSkinIcon = Sprite::create(MafUtils::format("Assets/icon_pet/skin_%d_%d.png", _ePet, objPetSkin->getIdx()));
        uiSkinIcon->setPosition(Vec2(uiSkinBG->getContentSize().width / 2, uiSkinBG->getContentSize().height / 2));
        uiSkinIcon->setScale((uiSkinBG->getContentSize().width - 3) / uiSkinIcon->getContentSize().width);
        uiSkinBG->addChild(uiSkinIcon);
        
        // event sale
        if ( isSkinBuy == false && objPetSkin->isEventSale() == true )
        {
            auto uiSaleSale = ui::ImageView::create("Assets/ui_common/icon_sale.png");
            uiSaleSale->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            uiSaleSale->setPosition(Vec2(10, uiSkinBG->getContentSize().height));
            ((ui::Scale9Sprite*)uiSaleSale->getVirtualRenderer())->getTexture()->setTexParameters(texParams);
            uiSaleSale->setScale(0.8);
            uiSkinBG->addChild(uiSaleSale);
        }
    }
    
    //
    std::string nameText = PetNewManager::getInstance()->getPetName(_ePet, true);
    if ( objPetSkin->getIdx() == 0 )
    {
        nameText = PetNewManager::getInstance()->getPetName(_ePet);
    }
    
    auto uiName = ui::CText::create(nameText, GAME_FONT, 22);
    uiName->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    uiName->setPosition(Vec2(120, uiLayout->getContentSize().height - 50));
    uiName->setTextColor(Color4B(COLOR_COIN));
    uiLayout->addChild(uiName);
    
    auto uiContents = ui::CText::create(objPetSkin->getName(), GAME_FONT, 24);
    uiContents->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    uiContents->setPosition(Vec2(120, uiLayout->getContentSize().height - 50));
    uiContents->setTextAreaSize(Size(350, 50));
    uiContents->setTextVerticalAlignment(TextVAlignment::TOP);
    uiContents->setTextHorizontalAlignment(TextHAlignment::LEFT);
    uiContents->setTextOverflow(Label::Overflow::SHRINK);
    uiLayout->addChild(uiContents);
  
    //
    std::string pathEquip = "Assets/ui_common/btn_1_1_on.png";
    if ( isSkinEqip )
    {
        pathEquip = "Assets/ui_common/btn_1_3_on.png";
    }
    auto uiEquip = ui::Button::create(pathEquip);
    uiEquip->setScale9Enabled(true);
    uiEquip->setCapInsets(Rect(33,33,2,2));
    uiEquip->setContentSize(Size(140, 82));
    uiEquip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    uiEquip->setPosition(Vec2(sizeLayout.width - 20, sizeLayout.height / 2));
    uiEquip->addClickEventListener(CC_CALLBACK_1(UILayoutPetSkin::onClickSkin, this));
    uiEquip->setTag(idx);
    uiLayout->addChild(uiEquip);
    {
        if ( isSkinEqip )
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_equip_now"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiEquip->getContentSize().width / 2, uiEquip->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiEquip->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiText->setTextColor(Color4B(COLOR_TITLE));
            uiEquip->addChild(uiText);
        }
        else if ( objPetSkin->getIdx() == 0 || isSkinBuy == true )
        {
            auto uiText = ui::CText::create(GAME_TEXT("t_ui_costume_equip"), GAME_FONT, 24);
            uiText->setPosition(Vec2(uiEquip->getContentSize().width / 2, uiEquip->getContentSize().height / 2));
            uiText->setTextAreaSize(Size(uiEquip->getContentSize().width - 10, uiText->getContentSize().height));
            uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
            uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
            uiText->setTextOverflow(Label::Overflow::SHRINK);
            uiEquip->addChild(uiText);
        }
        else
        {
            bool bAvailable = true;
            
            if ( objPetSkin->isEventSale() == false || objPetSkin->getItemIdx() == 0 )
            {
                bAvailable = false;
            }
            
            auto objItems = ItemsManager::getInstance()->getInfoItems(objPetSkin->getItemIdx());
            if ( objItems == nullptr )
            {
                bAvailable = false;
            }
            else
            {
                objItems->setTypeParams(objPetSkin->getItemTypeParams());
                objItems->setCount(objPetSkin->getItemCount());
            }
            
            if ( bAvailable == false )
            {
                uiEquip->setVisible(false);
            }
            else
            {
                auto spritePearl = Sprite::create(objItems->getPath());
                spritePearl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                spritePearl->setPosition(uiEquip->getContentSize().width * 0.45, uiEquip->getContentSize().height / 2);
                uiEquip->addChild(spritePearl);
                
                auto labelText = Label::createWithTTF(objItems->getCount(), GAME_FONT, 24);
                labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                labelText->setPosition(uiEquip->getContentSize().width * 0.40, uiEquip->getContentSize().height / 2);
                uiEquip->addChild(labelText);
            }
        }
    }
    

}

int64_t UILayoutPetSkin::listviewNumberOfCells(ui::CListView *listview)
{
    return _listPetSkin.size();
}

#pragma mark - ui
void UILayoutPetSkin::uiContainer()
{
    Size size = this->getContentSize();
    
    _uiContentsUIList = ui::Layout::create();
    _uiContentsUIList->setContentSize(Size(size.width, size.height));
    _uiContentsUIList->setIgnoreAnchorPointForPosition(false);
    _uiContentsUIList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiContentsUIList->setPosition(Vec2(size.width / 2, 0));
    addChild(_uiContentsUIList);
}

void UILayoutPetSkin::uiList()
{
    auto layout = _uiContentsUIList;
    Size size = layout->getContentSize();
    
    auto uiDetailBG = ui::ImageView::create("Assets/ui_common/base_text_1_3.png");
    uiDetailBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    uiDetailBG->setPosition(Vec2(size.width / 2, size.height - 10));
    layout->addChild(uiDetailBG);
    {
        auto uiText = ui::CText::create(GAME_TEXT("t_ui_pet_msg_12"), GAME_FONT, 22);
        uiText->setPosition(Vec2(uiDetailBG->getContentSize().width / 2, uiDetailBG->getContentSize().height / 2));
        uiText->setTextAreaSize(Size(uiDetailBG->getContentSize().width - 10, uiText->getContentSize().height));
        uiText->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiText->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiText->setTextOverflow(Label::Overflow::SHRINK);
        uiDetailBG->addChild(uiText);
    }
    
    //
    _uiListDownArrow = ui::ImageView::create("Assets/ui_common/btn_down_s.png");
    _uiListDownArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiListDownArrow->setPosition(Vec2(layout->getContentSize().width / 2, 10));
    layout->addChild(_uiListDownArrow);
    
    //
    _uiList = ui::CListView::create(this, Size(size.width, size.height - 55 - 41), ui::ScrollView::Direction::VERTICAL);
    _uiList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _uiList->setPosition(Vec2(size.width / 2, 41));
    _uiList->setScrollBarPositionFromCorner(Vec2(7, 7));
    _uiList->reloadData();
    layout->addChild(_uiList);
    
    
}

#pragma mark - draw
void UILayoutPetSkin::drawList()
{
    Vec2 pos = _uiList->getContainerOffset();
    _uiList->reloadData();
    _uiList->setContainerOffset(pos);
}
 
#pragma mark - set, get, add


#pragma mark - callback
void UILayoutPetSkin::callbackSkinBuy(bool bResult, int nResult)
{
    if ( bResult == false )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        
        if ( nResult == 0 )
        {
            strError = GAME_TEXT("t_ui_error_31");
        }
        else if ( nResult == 2 )
        {
            strError = GAME_TEXT("t_ui_error_6");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    ValueMap listData;
    listData["type"] = Value((int)_ePet);
    RefreshManager::getInstance()->refreshUI(E_REFRESH::PET, listData);
    
    //
    drawList();
}

#pragma mark - click
void UILayoutPetSkin::onClickSkin(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);
    
    auto objPetSkin = _listPetSkin.at(uiBtn->getTag());
    
    int skinIdx = objPetSkin->getIdx();
    int skinEquip = PetNewManager::getInstance()->getPetSkin(_ePet);
    
    bool isSkinEqip = false;
    if ( skinIdx == skinEquip )
    {
        isSkinEqip = true;
    }
    
    bool isSkinBuy = PetNewManager::getInstance()->isPetSkinBuy(_ePet, skinIdx);
    
    
    //
    if ( objPetSkin == nullptr || isSkinEqip == true )
    {// 오류, 장착중
        return;
    }
    else if ( isSkinBuy == true )
    {// 장착
        int result = PetNewManager::getInstance()->onPetSkinEquip(_ePet, objPetSkin->getIdx());
        if ( result != 0 )
        {
            return;
        }
        
        //
        drawList();
    }
    else
    {// 구입
        if ( AccountManager::getInstance()->getUserIdx() == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }

        if ( objPetSkin->isEventSale() == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_draw_msg_1"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }

        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_costume_msg_30"));
        popup->addButton(GAME_TEXT("t_ui_no"), true, nullptr);
        popup->addButton(GAME_TEXT("t_ui_yes"), false, [=](Ref* sender){

            
            //
            int result = PetNewManager::getInstance()->onPetSkinBuy(_ePet, skinIdx, true, CC_CALLBACK_2(UILayoutPetSkin::callbackSkinBuy, this));
            if ( result != 0 )
            {
                return;
            }
            
        });
        popup->show();
    }
}
