//
//  PopupPetEquipBoxBuyPurchase.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/05/24.
//

#include "PopupPetEquipBoxBuyPurchase.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoProduct.h"

#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPetEquipBoxBuyPurchase* PopupPetEquipBoxBuyPurchase::create()
{
    PopupPetEquipBoxBuyPurchase* pRet = new(std::nothrow) PopupPetEquipBoxBuyPurchase();
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

PopupPetEquipBoxBuyPurchase::PopupPetEquipBoxBuyPurchase() :
_bInitUI(false)
{
    
}

PopupPetEquipBoxBuyPurchase::~PopupPetEquipBoxBuyPurchase(void)
{
}

bool PopupPetEquipBoxBuyPurchase::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    //
    initVar();
    initUi();
    
    return true;
}

void PopupPetEquipBoxBuyPurchase::onEnter()
{
    PopupBase::onEnter();
    
    if ( _bInitUI == false )
    {
        _bInitUI = true;
        
        //
        uiContainer();
        uiInfo();
        uiList();
    }
    
    //
}

#pragma mark - init
void PopupPetEquipBoxBuyPurchase::initVar()
{
    _listProduct = CashManager::getInstance()->getProductType(E_PRODUCT::PET_EQUIP);
    
    // 200, 260
    int maxCol = 2;
    int maxRow = (int)_listProduct.size() / maxCol;
    if ( _listProduct.size() % maxCol != 0 )
    {
        maxRow += 1;
    }
    int padding = 25;
    
    double widthList = maxCol * 200 + ((maxCol - 1) * padding);
    double heightList = maxRow * 260 + ((maxRow - 1) * padding);
    _sizeList = Size(widthList, heightList);
}
void PopupPetEquipBoxBuyPurchase::initUi()
{
    // size
    _spriteContainer->setContentSize( Size(45 + _sizeList.width + 45, 75 + 90 + _sizeList.height + 25) );
    
    // label title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_pet_equip_box_name_3"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5);
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPetEquipBoxBuyPurchase::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
}

#pragma mark - ui
void PopupPetEquipBoxBuyPurchase::uiContainer()
{
    Size size = _spriteContainer->getContentSize();
    size.width = _sizeList.width;
    size.height = _sizeList.height + 90;
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(size.width, size.height));
    layout->setIgnoreAnchorPointForPosition(false);
    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layout->setPosition(Vec2(_spriteContainer->getContentSize().width / 2,_spriteContainer->getContentSize().height - 75));
    _spriteContainer->addChild(layout);

    _uiContainerInfo = ui::Layout::create();
    _uiContainerInfo->setContentSize( Size(size.width, 90) );
    _uiContainerInfo->setIgnoreAnchorPointForPosition(false);
    _uiContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _uiContainerInfo->setPosition( Vec2(size.width / 2, size.height) );
    layout->addChild(_uiContainerInfo);
    
    _uiContainerList = ui::Layout::create();
    _uiContainerList->setContentSize(Size(size.width, size.height - _uiContainerInfo->getContentSize().height));
    _uiContainerList->setIgnoreAnchorPointForPosition(false);
    _uiContainerList->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _uiContainerList->setPosition( Vec2(size.width / 2, 0) );
    layout->addChild(_uiContainerList);
}
void PopupPetEquipBoxBuyPurchase::uiInfo()
{
    auto layout = _uiContainerInfo;
    Size size = layout->getContentSize();
    
    auto uiDesc = ui::CText::create(GAME_TEXT("t_pet_equip_box_purchase_content_1"), GAME_FONT, 24);
    uiDesc->setPosition(Vec2(size.width / 2, size.height / 2));
    uiDesc->setTextAreaSize(Size(size.width, size.height));
    uiDesc->setTextVerticalAlignment(TextVAlignment::CENTER);
    uiDesc->setTextHorizontalAlignment(TextHAlignment::CENTER);
    uiDesc->setTextOverflow(Label::Overflow::SHRINK);
    layout->addChild(uiDesc);
}

void PopupPetEquipBoxBuyPurchase::uiList()
{
    auto layout = _uiContainerList;
    Size size = layout->getContentSize();
    
    int maxCol = 2;
    int maxRow = (int)_listProduct.size() / maxCol;
    if ( _listProduct.size() % maxCol != 0 )
    {
        maxRow += 1;
    }
    int padding = 25;
    
    double widthCell = 200;
    double heightCell = 260;
    
    for ( int i = 0; i < _listProduct.size(); i++ )
    {
        auto objProduct = _listProduct.at(i);
        
        int x = i % maxCol;
        int y = i / maxCol;
        double posX = widthCell * x + padding * x;
        double posY = size.height - heightCell * y - padding * y;
        
        auto uiBG = ui::Button::create("Assets/ui/pet/pet_equipment_shop_2_1.png");
        uiBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        uiBG->setPosition(Vec2(posX, posY));
        uiBG->setTag(i);
        uiBG->addClickEventListener(CC_CALLBACK_1(PopupPetEquipBoxBuyPurchase::onClickPurchase, this));
        layout->addChild(uiBG);
        
        auto uiPrice = ui::CText::create(objProduct->getPrice(), GAME_FONT, 24);
        uiPrice->setPosition(Vec2(uiBG->getContentSize().width / 2, 32));
        uiPrice->setTextAreaSize(Size(uiBG->getContentSize().width - 10, uiPrice->getContentSize().height));
        uiPrice->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiPrice->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiPrice->setTextOverflow(Label::Overflow::SHRINK);
        uiBG->addChild(uiPrice);
        
        auto uiCount = ui::CText::create(objProduct->getSaleRatio(), GAME_FONT_AL, 20);
        uiCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        uiCount->setPosition(Vec2(uiBG->getContentSize().width / 2, 65));
        uiBG->addChild(uiCount);
        
        auto uiTitle = ui::CText::create(objProduct->getName(), GAME_FONT, 30);
        uiTitle->setPosition(Vec2(uiBG->getContentSize().width / 2, uiBG->getContentSize().height - 25));
        uiTitle->setTextAreaSize(Size(uiBG->getContentSize().width - 10, uiPrice->getContentSize().height));
        uiTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
        uiTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
        uiTitle->setTextOverflow(Label::Overflow::SHRINK);
        uiTitle->setTextColor(Color4B(COLOR_TITLE));
        uiBG->addChild(uiTitle);
    }
}

#pragma mark - draw

#pragma mark - callback
void PopupPetEquipBoxBuyPurchase::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::PET_EQUIP);
        RefreshReddotManager::getInstance()->refreshUI(E_REFRESH_REDDOT::PET_EQUIP_BOX);
        
        //
        hide();
        
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
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

#pragma mark - click
void PopupPetEquipBoxBuyPurchase::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupPetEquipBoxBuyPurchase::onClickPurchase(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiBtn = dynamic_cast<ui::Button*>(sender);

    
    auto objProduct = _listProduct.at(uiBtn->getTag());
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(objProduct->getIdx(), CC_CALLBACK_4(PopupPetEquipBoxBuyPurchase::callbackPurchase, this));
}
  


