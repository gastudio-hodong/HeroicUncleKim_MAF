//
//  PopupPurchaseFirst.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseFirst.h"

#include "Common/ConfigGameNormal.h"
#include "Common/ConfigKey.h"

#include "ManagerGame/SaveManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPurchaseFirst* PopupPurchaseFirst::create()
{
    PopupPurchaseFirst* pRet = new(std::nothrow) PopupPurchaseFirst();
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

PopupPurchaseFirst::PopupPurchaseFirst(void) :
_callbackResult(nullptr),

_layerContainerContents(nullptr),
_layerContainerContentsMiddle(nullptr)
{
    
}

PopupPurchaseFirst::~PopupPurchaseFirst(void)
{
    
}

bool PopupPurchaseFirst::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // init
    initVar();
    initUi();
    
    
    return true;
}

void PopupPurchaseFirst::onEnter()
{
    PopupBase::onEnter();
    
    PopupLoading::show();
    CashManager::getInstance()->requestFirstBonusInfo(CC_CALLBACK_1(PopupPurchaseFirst::callbackInfo, this));
}

#pragma mark - init
void PopupPurchaseFirst::initVar()
{
    UserDefault::getInstance()->setBoolForKey(KEY_REDDOT_PURCHASE_FIRST, true);
}

void PopupPurchaseFirst::initUi()
{
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( Size(750, 1176) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainerContents->setPosition( getContentSize().width / 2, getContentSize().height / 2);
    addChild(_layerContainerContents);
    
    //
    _layerContainerContentsMiddle = Layer::create();
    _layerContainerContentsMiddle->setContentSize( Size(746, 494) );
    _layerContainerContentsMiddle->setIgnoreAnchorPointForPosition(false);
    _layerContainerContentsMiddle->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainerContentsMiddle->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    addChild(_layerContainerContentsMiddle);
     
    
    //
    uiContents();
    uiContentsMiddle();
}

#pragma mark - ui
void PopupPurchaseFirst::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/purchase/shop_bonus1_1.png");
    spriteBG->setPosition(_layerContainerContents->getContentSize().width / 2, _layerContainerContents->getContentSize().height / 2);
    _layerContainerContents->addChild(spriteBG);
    
    auto spriteIcon = Sprite::create("Assets/ui/purchase/shop_bs1.png");
    spriteIcon->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    spriteIcon->setPosition(spriteBG->getContentSize().width - 20, spriteBG->getContentSize().height - 50);
    spriteBG->addChild(spriteIcon);
    
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x_1.png"), Sprite::create("Assets/ui_common/btn_x_1.png"), nullptr, CC_CALLBACK_1(PopupPurchaseFirst::onClickClose, this));
    itemClose->setPosition(spriteBG->getContentSize().width - 10 - itemClose->getContentSize().width / 2, spriteBG->getContentSize().height - 10 - itemClose->getContentSize().height / 2);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    auto labelText1 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_1"), GAME_FONT, 80);
    labelText1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelText1->setPosition(20, spriteBG->getContentSize().height - 90);
    labelText1->setDimensions(spriteBG->getContentSize().width * 0.9, labelText1->getContentSize().height);
    labelText1->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelText1->setOverflow(Label::Overflow::SHRINK);
    spriteBG->addChild(labelText1);
    
    auto labelText2 = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_2"), GAME_FONT, 40);
    labelText2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    labelText2->setPosition(20, spriteBG->getContentSize().height - 190);
    labelText2->setColor(Color3B(255, 212, 0));
    labelText2->enableShadow(Color4B(166, 55, 0, 255));
    labelText2->setDimensions(spriteBG->getContentSize().width * 0.9, labelText2->getContentSize().height);
    labelText2->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labelText2->setOverflow(Label::Overflow::SHRINK);
    spriteBG->addChild(labelText2);
    //(const Color4B& shadowColor = Color4B::BLACK,const Size &offset = Size(2,-2), int blurRadius = 0);
    
    auto spriteInfoBG = Sprite::create("Assets/ui_common/base_text_2_1.png");
    spriteInfoBG->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    spriteInfoBG->setPosition(0, spriteBG->getContentSize().height - 245);
    spriteBG->addChild(spriteInfoBG);
    {
        auto labelText = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_3"), GAME_FONT, 28);
        labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        labelText->setPosition(20, spriteInfoBG->getContentSize().height / 2);
        labelText->setDimensions(spriteInfoBG->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteInfoBG->addChild(labelText);
    }
}

void PopupPurchaseFirst::uiContentsMiddle()
{
    _layerContainerContentsMiddle->removeAllChildren();
    
    auto spriteBG = Sprite::create("Assets/ui/purchase/shop_bonus1_2.png");
    spriteBG->setPosition(_layerContainerContentsMiddle->getContentSize().width / 2, _layerContainerContentsMiddle->getContentSize().height / 2);
    _layerContainerContentsMiddle->addChild(spriteBG);
    
    
    bool bGet = false;
    int nResult = CashManager::getInstance()->getFirstBonusResult();
    if ( nResult == 0 )
    {
        bGet = true;
    }
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    
    // reward
    int nRewardCount = 3;
    std::string listRewardBG[] = {"Assets/ui/purchase/shop_bonus_box1_1.png", "Assets/ui/purchase/shop_bonus_box1_2.png", "Assets/ui/purchase/shop_bonus_box1_3.png"};
    std::string listRewardTextKey[] = {"t_ui_purchase_bonus_msg_7", "t_ui_purchase_bonus_msg_8", "t_ui_purchase_bonus_msg_9"};
    double nPosX = _layerContainerContentsMiddle->getContentSize().width / 2 - (nRewardCount / 2.0 * 201) - (nRewardCount / 2 * 10);
    double nPosY = _layerContainerContentsMiddle->getContentSize().height / 2 ;
    for ( int i = 0; i < nRewardCount; i++ )
    {
        std::string strBG = listRewardBG[i];
        std::string strText = GAME_TEXT(listRewardTextKey[i]);;
       
        auto spriteBG = Sprite::create(strBG);
        spriteBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        spriteBG->setPosition(nPosX, nPosY);
        _layerContainerContentsMiddle->addChild(spriteBG);
        
        auto labelText = Label::createWithTTF(strText, GAME_FONT, 24);
        labelText->setPosition(spriteBG->getContentSize().width / 2, 54);
        labelText->setDimensions(spriteBG->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        spriteBG->addChild(labelText);
        
        nPosX = nPosX + 201 + 10;
    }
    
    // item
    std::string strGetPath = "Assets/ui_common/btn_4_1.png";
    if ( bGet == true )
    {
        strGetPath = "Assets/ui_common/btn_4_2.png";
    }
    auto spriteGet = ui::Scale9Sprite::create(strGetPath, Rect::ZERO, Rect(40,48,10,2));
    spriteGet->setContentSize(Size(296, 100));
    auto spriteSelectGet = ui::Scale9Sprite::create(strGetPath, Rect::ZERO, Rect(40,48,10,2));
    spriteSelectGet->setContentSize(Size(296, 100));
    
    auto itemGet = MafNode::MafMenuItemSprite::create(spriteGet, spriteSelectGet, nullptr, CC_CALLBACK_1(PopupPurchaseFirst::onClickGet, this));
    itemGet->setPosition(spriteBG->getContentSize().width / 2, 0);
    menu->addChild(itemGet);
    {
        std::string strGet = GAME_TEXT("t_ui_purchase_bonus_msg_4");
        if ( bGet == true )
        {
            strGet = GAME_TEXT("t_ui_purchase_bonus_msg_5");
        }
        
        auto labelText = Label::createWithTTF(strGet, GAME_FONT, 32);
        labelText->setPosition(itemGet->getContentSize().width / 2, itemGet->getContentSize().height / 2);
        labelText->setDimensions(itemGet->getContentSize().width * 0.9, labelText->getContentSize().height);
        labelText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelText->setOverflow(Label::Overflow::SHRINK);
        itemGet->addChild(labelText);
    }
    
    
    auto labelInfo = Label::createWithTTF(GAME_TEXT("t_ui_purchase_bonus_msg_11"), GAME_FONT, 24);
    labelInfo->setPosition(_layerContainerContentsMiddle->getContentSize().width / 2, -100);
    labelInfo->setDimensions(spriteBG->getContentSize().width, labelInfo->getContentSize().height);
    labelInfo->setOverflow(Label::Overflow::SHRINK);
    labelInfo->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _layerContainerContentsMiddle->addChild(labelInfo);
}
 

#pragma mark - set, get, sort, add
void PopupPurchaseFirst::setCallbackResult(const std::function<void(int)>& callback)
{
    _callbackResult = callback;
}

#pragma mark - click
void PopupPurchaseFirst::onClickClose(Ref* sender)
{
    hide();
}
  
void PopupPurchaseFirst::onClickGet(cocos2d::Ref* sender)
{
    int nResult = CashManager::getInstance()->getFirstBonusResult();
    if ( nResult == 0 )
    {
        PopupLoading::show();
        CashManager::getInstance()->requestFirstBonusReceive(CC_CALLBACK_2(PopupPurchaseFirst::callbackReward, this));
    }
    else
    {
        if ( _callbackResult != nullptr )
        {
            _callbackResult(nResult);
            _callbackResult = nullptr;
        }
        hide();
    }
}

#pragma mark - callbakc
void PopupPurchaseFirst::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false || CashManager::getInstance()->getFirstBonusResult() == 5 )
    {
        hide();
        return;
    }
    
    //
    uiContentsMiddle();
}

void PopupPurchaseFirst::callbackReward(bool bResult, std::string strReward)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    // 상점 정보 다시 가져오기
    PopupLoading::show();
    CashManager::getInstance()->requestDailyInfo(CC_CALLBACK_1(PopupPurchaseFirst::callbackCashPension, this));
}

void PopupPurchaseFirst::callbackCashPension(bool bResult)
{
    //
    PopupLoading::hide();
    
    // 저장하기
    PopupLoading::show();
    SaveManager::getInstance()->requestSave(CC_CALLBACK_1(PopupPurchaseFirst::callbackSave, this));
}

void PopupPurchaseFirst::callbackSave(bool bResult)
{
    //
    PopupLoading::hide();
    
    if ( _callbackResult != nullptr )
    {
        _callbackResult(CashManager::getInstance()->getFirstBonusResult());
        _callbackResult = nullptr;
    }
    hide();
}
