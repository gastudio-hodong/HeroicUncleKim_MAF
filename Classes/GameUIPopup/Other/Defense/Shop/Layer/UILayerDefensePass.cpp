//
//  UILayerDefensePass.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2023/08/25.
//

#include "UILayerDefensePass.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/Defense/InfoDefenseCharacter.h"
#include "GameObject/Defense/InfoDefenseWeapon.h"
#include "GameObject/InfoSetting.h"
#include "GameObject/InfoProduct.h"
#include "GameObject/InfoProductGrowth.h"
#include "GameObject/InfoItems.h"

#include "GameUI/UI/UICurrency.h"

#include "GameUIPopup/Other/CurrencyDetail/PopupCurrencyDetail.h"

#include "ManagerGame/DefenseManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

UILayerDefensePass* UILayerDefensePass::create(cocos2d::Size size)
{
    UILayerDefensePass* pRet = new(std::nothrow) UILayerDefensePass();
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

UILayerDefensePass::UILayerDefensePass(void):
_level(0),
_levelNow(0),
_levelMax(0),
_idxCash(0),
_type("DEFENSE_FLOOR"),
_objProduct(nullptr),
_lyMain(nullptr),
_lyBottom(nullptr)
{
    _listProductItmes.clear();
    _listGrowth.clear();
    _onRefresh.Clear();
}

UILayerDefensePass::~UILayerDefensePass(void)
{
    _listProductItmes.clear();
    _listGrowth.clear();
    DefenseManager::getInstance()->_onUpdateFloor.Clear(this);
    _onRefresh.Clear();
}

bool UILayerDefensePass::init(cocos2d::Size size)
{
    if ( !LayerColor::init())
    {
        return false;
    }
    
    setContentSize(size);
    setIgnoreAnchorPointForPosition(false);
    
    _level = CashManager::getInstance()->getGrowthNowLevel(_type);
    _levelNow = _level;
    _levelMax = CashManager::getInstance()->getGrowthMaxLevel(_type);
    _idxCash = CashManager::getInstance()->getGrowthToIdxCash(_type, _levelNow);
    //
    DefenseManager::getInstance()->_onUpdateFloor += new MafFunction<void(void)>(CC_CALLBACK_0(UILayerDefensePass::onFloorUpdate, this), this);
    // init
    initVar();
    initUi();
    
    return true;
}
void UILayerDefensePass::onEnter()
{
    LayerColor::onEnter();
    
    // network
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(UILayerDefensePass::callbackInfo, this));
}
#pragma mark - init
void UILayerDefensePass::initVar()
{
}

void UILayerDefensePass::initUi()
{
    // UI
    //
    
    auto box = ui::Scale9Sprite::create("Assets/ui_common/base_box_1_22.png");
    box->setCapInsets(Rect(21, 21, 20, 20));
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(getContentSize()/2);
    box->setContentSize(getContentSize());
    addChild(box);
    
    _lyBottom = LayerColor::create();
    _lyBottom->setContentSize(Size(getContentSize().width, 80));
    _lyBottom->setIgnoreAnchorPointForPosition(false);
    _lyBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyBottom->setPosition(Vec2(getContentSize().width / 2, 0));
    addChild(_lyBottom);
    
    _lyMain = LayerColor::create();
    _lyMain->setContentSize(Size(getContentSize().width, 330));
    _lyMain->setIgnoreAnchorPointForPosition(false);
    _lyMain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lyMain->setPosition(_lyBottom->getPosition() + Vec2::UP * _lyBottom->getContentSize().height);
    addChild(_lyMain);
    
    uiTop();
    uiMain();
    uiBottom();
}

#pragma mark - utils
bool UILayerDefensePass::isCondition(InfoProductGrowth* objGrowth)
{
    return DefenseManager::getInstance()->getHighFloor() >= objGrowth->getCondition();
}
#pragma mark - ui
void UILayerDefensePass::uiTop()
{
    auto box = Sprite::create("Assets/ui/defense/Defense_pass_title_box.png");
    box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    box->setPosition(getContentSize().width/2, getContentSize().height);
    addChild(box);
    
    auto title = Sprite::create("Assets/ui/defense/Defense_pass_title.png");
    title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    title->setPosition(box->getContentSize()/2);
    box->addChild(title);
}

void UILayerDefensePass::uiMain()
{
    _lyMain->removeAllChildren();
    
    auto freeBox = Sprite::create("Assets/ui/defense/Defense_pass_bg_box1_blue_big.png");
    freeBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    freeBox->setPosition(25, _lyMain->getContentSize().height/2);
    _lyMain->addChild(freeBox);
    {
        auto icon = Sprite::create("Assets/ui/defense/Defense_pass_box1.png");
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        icon->setPosition(freeBox->getContentSize()/2);
        freeBox->addChild(icon);
    }
    
    auto cashBox = Sprite::create("Assets/ui/defense/Defense_pass_bg_box1_purple_big.png");
    cashBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    cashBox->setPosition(25, _lyMain->getContentSize().height/2);
    _lyMain->addChild(cashBox);
    {
        auto icon = Sprite::create("Assets/ui/defense/Defense_pass_box2.png");
        icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        icon->setPosition(cashBox->getContentSize()/2);
        cashBox->addChild(icon);
        
        std::string price = "";
        if(_objProduct != nullptr)
        {
            price = _objProduct->getPrice();
            if(!_bBuy)
            {
                price = _objProduct->getPrice();
                
                auto btnPurchase = ui::Button::create("Assets/ui/defense/Btn_defense_pass_buy.png");
                btnPurchase->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                btnPurchase->setPosition(Vec2(cashBox->getContentSize().width/2, 10));
                btnPurchase->setTitleText(price);
                btnPurchase->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
                btnPurchase->getTitleLabel()->setDimensions(btnPurchase->getContentSize().width - 10, btnPurchase->getContentSize().height - 15);
                btnPurchase->getTitleLabel()->setPosition(Vec2(btnPurchase->getContentSize()/2) + Vec2::DOWN * 4);
                btnPurchase->getTitleLabel()->enableOutline(Color4B::BLACK, 2);
                btnPurchase->setTitleFontSize(24);
                btnPurchase->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
                btnPurchase->addClickEventListener(CC_CALLBACK_1(UILayerDefensePass::onClickBuy, this));
                cashBox->addChild(btnPurchase);
            }
        }
    }
    
    Vec2 location = freeBox->getPosition() + Vec2::RIGHT * (freeBox->getContentSize().width + 3);
    for(int i = 0; i < _listGrowth.size(); ++i)
    {
        auto obj = _listGrowth.at(i);
        
        bool isCondition = false;
        if(DefenseManager::getInstance()->getHighFloor() >= obj->getCondition())
            isCondition = true;
        
        std::string freeRewardBoxPath = "";
        std::string cashRewardBoxPath = "";
        if(isCondition)
        {
            freeRewardBoxPath = "Assets/ui/defense/Defense_pass_bg_box1_blue_select.png";
            cashRewardBoxPath = "Assets/ui/defense/Defense_pass_bg_box1_purple_select.png";
        }
        else
        {
            freeRewardBoxPath = "Assets/ui/defense/Defense_pass_bg_box1_blue.png";
            cashRewardBoxPath = "Assets/ui/defense/Defense_pass_bg_box1_purple.png";
        }
        
        auto freeRewardBox = Sprite::create(freeRewardBoxPath);
        freeRewardBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        freeRewardBox->setPosition(location);
        _lyMain->addChild(freeRewardBox);
        {
            auto box = Sprite::create("Assets/ui/defense/Pass_compensation_box1.png");
            box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            box->setPosition(freeRewardBox->getContentSize()/2);
            freeRewardBox->addChild(box);
            
            if(auto item = obj->getItemsFree().at(0))
            {
                auto icon = Sprite::create(item->getPath());
                icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                icon->setPosition(Vec2(box->getContentSize()/2) + Vec2::UP * 10);
                box->addChild(icon);
                
                auto lbCount = Label::createWithTTF(MafUtils::format("X%s", item->getCount().c_str()), GAME_FONT, 20);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbCount->setPosition(box->getContentSize().width/2, 5);
                lbCount->setDimensions(box->getContentSize().width - 10, lbCount->getContentSize().height);
                lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                box->addChild(lbCount);
            }
            if(isCondition)
            {
                std::string path = "";
                if(obj->isRewardFree())
                    path = "Assets/ui/defense/Pass_compensation_box_bluecheck.png";
                else
                    path = "Assets/ui/defense/Pass_compensation_box_blueline.png";
                
                auto selectLine = Sprite::create(path);
                selectLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                selectLine->setPosition(box->getContentSize()/2);
                box->addChild(selectLine);
            }
            else
            {
                auto coverBox = Sprite::create("Assets/ui/defense/Pass_compensation_box_black.png");
                coverBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                coverBox->setPosition(freeRewardBox->getContentSize()/2);
                freeRewardBox->addChild(coverBox);
            }
        }
        
        auto cashRewardBox = Sprite::create(cashRewardBoxPath);
        cashRewardBox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        cashRewardBox->setPosition(location + Vec2::DOWN * 15);
        _lyMain->addChild(cashRewardBox);
        {
            auto box = Sprite::create("Assets/ui/defense/Pass_compensation_box2.png");
            box->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            box->setPosition(cashRewardBox->getContentSize()/2);
            cashRewardBox->addChild(box);
            
            if(auto item = obj->getItemsCash().at(0))
            {
                auto icon = Sprite::create(item->getPath());
                icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                icon->setPosition(Vec2(box->getContentSize()/2) + Vec2::UP * 10);
                box->addChild(icon);
                
                auto lbCount = Label::createWithTTF(MafUtils::format("X%s", item->getCount().c_str()), GAME_FONT, 20);
                lbCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                lbCount->setPosition(box->getContentSize().width/2, 5);
                lbCount->setDimensions(box->getContentSize().width - 10, lbCount->getContentSize().height);
                lbCount->setAlignment(TextHAlignment::CENTER, TextVAlignment::BOTTOM);
                lbCount->setOverflow(Label::Overflow::SHRINK);
                box->addChild(lbCount);
            }
            if(isCondition)
            {
                std::string path = "";
                if(obj->isRewardCash())
                    path = "Assets/ui/defense/Pass_compensation_box_purplecheck.png";
                else
                    path = "Assets/ui/defense/Pass_compensation_box_purpleline.png";
                
                auto selectLine = Sprite::create(path);
                selectLine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                selectLine->setPosition(box->getContentSize()/2);
                box->addChild(selectLine);
            }
            else
            {
                auto coverBox = Sprite::create("Assets/ui/defense/Pass_compensation_box_black.png");
                coverBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                coverBox->setPosition(box->getContentSize()/2);
                box->addChild(coverBox);
            }
            if(!_bBuy)
            {
                auto lock = Sprite::create("Assets/ui_common/Defense_lock.png");
                lock->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                lock->setPosition(Vec2(0, box->getContentSize().height));
                box->addChild(lock);
            }
        }
        
        std::string rewardButtonPath = "";
        if(_bBuy)
            rewardButtonPath = "Assets/ui/defense/Btn_defense_pass_yellow.png";
        else
            rewardButtonPath = "Assets/ui/defense/Btn_defense_pass_blue.png";
            
        //t_ui_shop_bonus_msg_4
        auto btnReward = ui::Button::create(rewardButtonPath);
        btnReward->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btnReward->setPosition(location + Vec2::RIGHT * freeRewardBox->getContentSize().width/2);
        btnReward->setTitleText(GAME_TEXT("t_ui_shop_bonus_msg_4"));
        btnReward->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
        btnReward->getTitleLabel()->setDimensions(btnReward->getContentSize().width - 10, btnReward->getContentSize().height - 5);
        btnReward->getTitleLabel()->enableOutline(Color4B::BLACK, 2);
        btnReward->setTitleFontSize(24);
        btnReward->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
        btnReward->setTag(i+1);
        btnReward->addClickEventListener(CC_CALLBACK_1(UILayerDefensePass::onClickReward, this));
        _lyMain->addChild(btnReward);
        
        if(isCondition)
        {
            if(_bBuy)
            {
                if(obj->isRewardCash())
                    btnReward->setEnabled(false);
            }
            else
            {
                if(obj->isRewardFree())
                    btnReward->setEnabled(false);
            }
        }
        else
            btnReward->setEnabled(false);
        
        std::string conditionBoxPath = "";
        if(isCondition)
            conditionBoxPath = "Assets/ui/defense/Pass_b_number_box_blue.png";
        else
            conditionBoxPath = "Assets/ui/defense/Pass_b_number_box.png";
            
        auto conditionBox = Sprite::create("Assets/ui/defense/Pass_b_number_box_blue.png");
        conditionBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        conditionBox->setPosition(freeRewardBox->getContentSize().width/2, freeRewardBox->getContentSize().height - 3);
        freeRewardBox->addChild(conditionBox);
        
        auto lbCondition = Label::createWithTTF(MafUtils::format("B%d", obj->getCondition()), GAME_FONT, 20);
        lbCondition->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbCondition->setPosition(Vec2(conditionBox->getContentSize()/2) + Vec2::UP * 3);
        lbCondition->setDimensions(conditionBox->getContentSize().width - 30, lbCondition->getContentSize().height);
        lbCondition->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbCondition->setOverflow(Label::Overflow::SHRINK);
        conditionBox->addChild(lbCondition);
        
        location += Vec2::RIGHT * (freeRewardBox->getContentSize().width + 3);
    }
    
}

void UILayerDefensePass::uiBottom()
{
    _lyBottom->removeAllChildren();
    
    auto arrowBox = Sprite::create("Assets/ui_common/Defense_arrow_box.png");
    arrowBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    arrowBox->setPosition(_lyBottom->getContentSize().width - 30, _lyBottom->getContentSize().height/2);
    _lyBottom->addChild(arrowBox);
    
    auto lbText = Label::createWithTTF(GAME_TEXTFORMAT("t_ui_defense_shop_pass_msg1", _levelNow), GAME_FONT, 20);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setPosition(30, _lyBottom->getContentSize().height/2);
    lbText->setDimensions(_lyBottom->getContentSize().width - arrowBox->getContentSize().width - 80, lbText->getContentSize().height);
    lbText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    lbText->setOverflow(Label::Overflow::SHRINK);
    _lyBottom->addChild(lbText);
    
    
    auto btnPrivious = ui::Button::create("Assets/ui_common/arrow_L01.png");
    btnPrivious->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    btnPrivious->setPosition(Vec2(10, arrowBox->getContentSize().height/2));
    btnPrivious->setName("PREVIOUS");
    btnPrivious->addClickEventListener(CC_CALLBACK_1(UILayerDefensePass::onClickPage, this));
    arrowBox->addChild(btnPrivious);
    
    auto btnNext = ui::Button::create("Assets/ui_common/arrow_R01.png");
    btnNext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnNext->setPosition(Vec2(arrowBox->getContentSize().width - 10, arrowBox->getContentSize().height/2));
    btnNext->setName("NEXT");
    btnNext->addClickEventListener(CC_CALLBACK_1(UILayerDefensePass::onClickPage, this));
    arrowBox->addChild(btnNext);
    
    auto lbLv = Label::createWithTTF(MafUtils::format("LV.%d", _levelNow), GAME_FONT, 24);
    lbLv->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbLv->setPosition(arrowBox->getContentSize()/2);
    lbLv->setDimensions(150, lbLv->getContentSize().height);
    lbLv->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbLv->setOverflow(Label::Overflow::SHRINK);
    arrowBox->addChild(lbLv);
}
#pragma mark -utils
void UILayerDefensePass::onClickPage(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto node = (Node*) sender;
    
    int tmp = _levelNow;
    if(node->getName() == "NEXT")
        _levelNow++;
    else if(node->getName() == "PREVIOUS")
        _levelNow--;
    
    if(_levelNow <= 1)
        _levelNow = 1;
    if(_levelNow >= _levelMax)
        _levelNow = _levelMax;
    //
    
    _level = CashManager::getInstance()->getGrowthNowLevel(_type);
    _levelMax = CashManager::getInstance()->getGrowthMaxLevel(_type);
    _idxCash = CashManager::getInstance()->getGrowthToIdxCash(_type, _levelNow);
    
    if(tmp != _levelNow)
    {
        PopupLoading::show();
        CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(UILayerDefensePass::callbackInfo, this));
    }
}

void UILayerDefensePass::onClickBuy(cocos2d::Ref* sender)
{
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    
    std::string type = _type;
    int levelPrev = _levelNow - 1;
    if ( levelPrev > 0 )
    {
        int idxCash = CashManager::getInstance()->getGrowthToIdxCash(type, levelPrev);
        
        int buyCount = CashManager::getInstance()->getHistoryProduct(idxCash);
        if ( buyCount <= 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_growth_pack_purchase_msg"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    CashManager::getInstance()->purchaseHas(_idxCash, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(_idxCash, CC_CALLBACK_4(UILayerDefensePass::callbackPurchase, this));
            });
            popup->show();
        }
        else if ( nHas == 1 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_46"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), false, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            PopupLoading::show();
            CashManager::getInstance()->purchaseInitiate(_idxCash, CC_CALLBACK_4(UILayerDefensePass::callbackPurchase, this));
        }
    });
}

void UILayerDefensePass::onClickReward(cocos2d::Ref* sender)
{
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    auto uiButton = (ui::Button*)sender;

    int idx = uiButton->getTag();
    auto objGrowth = CashManager::getInstance()->getGrowth(idx, _idxCash);
    
    bool bCondition = isCondition(objGrowth);
    if ( bCondition == false )
    {
        return;
    }
    
    if ( objGrowth->isRewardFree() == true && objGrowth->isRewardCash() == true )
    {
        return;
    }
    
    if ( objGrowth->isRewardFree() == true && objGrowth->isRewardCash() == false )
    {
        if ( _bBuy == false )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_shop_bonus_msg_7"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            return;
        }
    }

    //
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthReceive(_idxCash, idx, CC_CALLBACK_1(UILayerDefensePass::callbackReceive, this));
}
#pragma mark -callback
void UILayerDefensePass::callbackInfo(bool bResult, int bBuy)
{
    //
    PopupLoading::hide();
    //
    if ( bResult == false )
    {
        std::string strMessageKey = "t_ui_error_30";
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT(strMessageKey));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    _bBuy = bBuy;
    
    //
    _listGrowth = CashManager::getInstance()->getGrowthGroup(_type, _levelNow);
    
    //
    if ( _objProduct != nullptr )
    {
        _objProduct->release();
        _objProduct = nullptr;
    }
    _objProduct = CashManager::getInstance()->getProduct(_idxCash);
    _objProduct->retain();
    
    _listProductItmes.clear();
    for ( int i = 0; i < _listGrowth.size(); i++ )
    {
        auto objGrowth = _listGrowth.at(i);

        //
        auto listItemsFree = objGrowth->getItemsFree();
        for ( int j = 0; j < listItemsFree.size(); j++ )
        {
            auto objItem = listItemsFree.at(j);

            bool bHave = false;
            for ( auto& obj : _listProductItmes )
            {
                if ( obj->getIdx() == objItem->getIdx() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                _listProductItmes.pushBack(objItemClone);
            }
        }
        
        //
        auto listItemsCash = objGrowth->getItemsCash();
        for ( int j = 0; j < listItemsCash.size(); j++ )
        {
            auto objItem = listItemsCash.at(j);

            bool bHave = false;
            for ( auto& obj : _listProductItmes )
            {
                if ( obj->getIdx() == objItem->getIdx() )
                {
                    std::string count = obj->getCount();
                    std::string countPlus = objItem->getCount();
                    
                    count = MafUtils::bigAddNum(count, countPlus);
                    obj->setCount(count);
                    bHave = true;
                    break;
                }
            }
            if ( bHave == false )
            {
                auto objItemClone = objItem->clone();
                objItemClone->setCount(objItem->getCount());
                _listProductItmes.pushBack(objItemClone);
            }
        }
    }
    
    //
    CashManager::getInstance()->addHistoryAds(_idxCash);
    
    
    uiMain();
    uiBottom();
}

void UILayerDefensePass::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // 재화 다시 가져오기.
        PopupLoading::show();
        CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(UILayerDefensePass::callbackInfo, this));
        
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

void UILayerDefensePass::callbackReceive(bool isResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( isResult == false )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            // 정보 다시 가져오기
            PopupLoading::show();
            CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(UILayerDefensePass::callbackInfo, this));
        });
        popup->show();
        return;
    }
    
    // 정보 다시 가져오기
    PopupLoading::show();
    CashManager::getInstance()->requestGrowthInfo(_idxCash, CC_CALLBACK_2(UILayerDefensePass::callbackInfo, this));
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_shop_bonus_msg_3"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
    _onRefresh.Invoke();
}
void UILayerDefensePass::onFloorUpdate()
{
    if(DefenseManager::getInstance()->getHighFloor() >= 100 && DefenseManager::getInstance()->getHighFloor() % 100 == 0)
        uiMain();
}
