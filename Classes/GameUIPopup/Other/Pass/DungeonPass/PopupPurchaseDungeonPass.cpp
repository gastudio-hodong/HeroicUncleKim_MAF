//
//  PopupPurchaseDungeonPass.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/09/17.
//

#include "PopupPurchaseDungeonPass.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/PassManager.h"

USING_NS_CC;

PopupPurchaseDungeonPass* PopupPurchaseDungeonPass::create()
{
    PopupPurchaseDungeonPass* pRet = new(std::nothrow) PopupPurchaseDungeonPass();
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

PopupPurchaseDungeonPass::PopupPurchaseDungeonPass():
_objPass(nullptr),

_callbackRedraw(nullptr),

_layerContainerReward(nullptr),
_layerContainerBuff(nullptr)
{
    
}

PopupPurchaseDungeonPass::~PopupPurchaseDungeonPass(void)
{
    if ( _objPass != nullptr )
    {
        _objPass->release();
        _objPass = nullptr;
    }
}

bool PopupPurchaseDungeonPass::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    initVar();
    initUI();
    
    return true;
}

#pragma mark - init
void PopupPurchaseDungeonPass::initVar()
{
    _objPass = PassManager::getInstance()->getPassInfo(E_PASS_TYPE::PASS_DUNGEON);
    if ( _objPass != nullptr )
    {
        _objPass->retain();
        
        //
        _listPassStep.clear();
        _listPassStep = _objPass->getListStep();
        
        //
        Vector<InfoItems*> listRewardAll;
        Vector<InfoItems*> listRewardNow;
        for ( auto& objStep : _listPassStep )
        {
            auto objItems = ItemsManager::getInstance()->getInfoItems(objStep->getRewardPurIdx());
            if ( objItems == nullptr )
            {
                continue;
            }
            
            objItems->setTypeParams(objStep->getRewardPurTypeParams());
            objItems->setCount(objStep->getRewardPurCount());
            listRewardAll.pushBack(objItems);
            
            
            if ( _objPass->getLevel() >= objStep->getIdx() )
            {
                auto objItems = ItemsManager::getInstance()->getInfoItems(objStep->getRewardPurIdx());
                objItems->setTypeParams(objStep->getRewardPurTypeParams());
                objItems->setCount(objStep->getRewardPurCount());
                listRewardNow.pushBack(objItems);
            }
        }
        
        //
        for ( auto obj : listRewardAll )
        {
            bool bApply = false;
            for ( int i = 0; i < _listRewardAll.size(); i++ )
            {
                auto objTemp = _listRewardAll.at(i);
                
                if ( obj->getIdx() == objTemp->getIdx() && obj->getTypeParams() == objTemp->getTypeParams() )
                {
                    bApply = true;
    
                    std::string strCountPrev = objTemp->getCount();
                    strCountPrev = MafUtils::bigAddNum(strCountPrev, obj->getCount());
                    objTemp->setCount(strCountPrev);
                    break;
                }
            }
    
            if ( bApply == false )
            {
                auto objTemp = ItemsManager::getInstance()->getInfoItems(obj->getIdx());
                if ( objTemp != nullptr )
                {
                    objTemp->setCount(obj->getCount());
                    objTemp->setTypeParams(obj->getTypeParams());
    
                    _listRewardAll.pushBack(objTemp);
                }
            }
        }
        
        //
        for ( auto obj : listRewardNow )
        {
            bool bApply = false;
            for ( int i = 0; i < _listRewardNow.size(); i++ )
            {
                auto objTemp = _listRewardNow.at(i);
                
                if ( obj->getIdx() == objTemp->getIdx() && obj->getTypeParams() == objTemp->getTypeParams() )
                {
                    bApply = true;
    
                    std::string strCountPrev = objTemp->getCount();
                    strCountPrev = MafUtils::bigAddNum(strCountPrev, obj->getCount());
                    objTemp->setCount(strCountPrev);
                    break;
                }
            }
    
            if ( bApply == false )
            {
                auto objTemp = ItemsManager::getInstance()->getInfoItems(obj->getIdx());
                if ( objTemp != nullptr )
                {
                    objTemp->setCount(obj->getCount());
                    objTemp->setTypeParams(obj->getTypeParams());
    
                    _listRewardNow.pushBack(objTemp);
                }
            }
        }
    }
    
    
}

void PopupPurchaseDungeonPass::initUI()
{
    Size size = Size(734, 900);
    
    //
    _spriteContainer = ui::Scale9Sprite::create("Assets/ui_common/popup_mini_scale2_1.png", Rect::ZERO, Rect(77,23,2,2));
    _spriteContainer->setContentSize(size);
    _spriteContainer->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 13.5);
    addChild(_spriteContainer);
    
    auto spriteEffect = Sprite::create("Assets/ui/purchase/fx_popup_bg1_1.png");
    spriteEffect->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(spriteEffect, -1);
    
    auto spriteTitleBG = Sprite::create("Assets/ui/purchase/base_title_1_1.png");
    spriteTitleBG->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height);
    _spriteContainer->addChild(spriteTitleBG);
    {
        auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_btn_purchase"), GAME_FONT, 40);
        lbTitle->setPosition(spriteTitleBG->getContentSize().width / 2, spriteTitleBG->getContentSize().height / 2 + 10);
        lbTitle->setDimensions(450, lbTitle->getContentSize().height);
        lbTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbTitle->setOverflow(Label::Overflow::SHRINK);
        lbTitle->setColor(COLOR_TITLE);
        spriteTitleBG->addChild(lbTitle);
    }
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"),nullptr, CC_CALLBACK_1(PopupPurchaseDungeonPass::onClickClose, this));
    itemClose->setPosition(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height + 175);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    // item purhcase
    auto itemPurchase = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui/purchase/pass_bar_btn2_1.png"),nullptr, CC_CALLBACK_1(PopupPurchaseDungeonPass::onClickPurchase, this));
    itemPurchase->setPosition(_spriteContainer->getContentSize().width / 2, 0);
    itemPurchase->setVisible(false);
    menu->addChild(itemPurchase);
    
    auto objProduct = CashManager::getInstance()->getProduct(E_CASH::CASH_DUNGEON_PASS_1);
    if ( objProduct != nullptr )
    {
        itemPurchase->setVisible(true);
        
        auto labelPrice = Label::createWithTTF(objProduct->getPrice(), GAME_FONT, 30);
        labelPrice->setPosition(itemPurchase->getContentSize()/2);
        itemPurchase->addChild(labelPrice);
    }
    
    _layerContainerReward = Layer::create();
    _layerContainerReward->setContentSize( Size(680, 420) );
    _layerContainerReward->setIgnoreAnchorPointForPosition(false);
    _layerContainerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerReward->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 120);
    _spriteContainer->addChild(_layerContainerReward);
    
    _layerContainerBuff = Layer::create();
    _layerContainerBuff->setContentSize( Size(680, 234) );
    _layerContainerBuff->setIgnoreAnchorPointForPosition(false);
    _layerContainerBuff->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBuff->setPosition(_spriteContainer->getContentSize().width / 2, 80);
    _spriteContainer->addChild(_layerContainerBuff);
    
    
    // text time
    {
        auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/base_text_1_2.png");
        spriteBG->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 80);
        _spriteContainer->addChild(spriteBG);
        
        _labelTime = Label::createWithTTF("", GAME_FONT, 20);
        _labelTime->setPosition(spriteBG->getContentSize()/2);
        spriteBG->addChild(_labelTime);
        
        
    }
    
    //
    uiReward();
    uiBuff();
    
    //
    drawTime();
}

#pragma mark - ui
void PopupPurchaseDungeonPass::uiReward()
{
    _layerContainerReward->removeAllChildren();
    
    if ( _objPass == nullptr )
    {
        return;
    }

    //
    auto sprTopBox = Sprite::create("Assets/ui/purchase/pass_payment_box1_1.png");
    sprTopBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprTopBox->setPosition(_layerContainerReward->getContentSize().width / 2, _layerContainerReward->getContentSize().height);
    _layerContainerReward->addChild(sprTopBox);
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_totalreward"), GAME_FONT, 30);
        labelTitle->setPosition(sprTopBox->getContentSize().width / 2, sprTopBox->getContentSize().height - 37.5);
        labelTitle->setDimensions(sprTopBox->getContentSize().width - 10, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        sprTopBox->addChild(labelTitle);
        
        //
        const int paddingX = 5;
        const int paddingY = 5;
        int drawRow = 1;
        int drawCol = (int)_listRewardAll.size();
        
        int width = 75 * drawCol + paddingX * (drawCol - 1);
        int height = 75 * drawRow + paddingY * (drawRow - 1);
        
        auto layerReward = Layer::create();
        layerReward->setContentSize( Size(width, height) );
        layerReward->setIgnoreAnchorPointForPosition(false);
        layerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        layerReward->setPosition( sprTopBox->getContentSize().width / 2, 69.5);
        sprTopBox->addChild(layerReward);
           
        for ( int i = 0; i < _listRewardAll.size(); i++ )
        {
            int posX = i * 75 + i * paddingX;
            int posY = layerReward->getContentSize().height / 2;
            
            auto spriteRewardBg = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteRewardBg->setContentSize(Size(75, 75));
            spriteRewardBg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            spriteRewardBg->setPosition(posX, posY);
            layerReward->addChild(spriteRewardBg);
            
            auto obj = _listRewardAll.at(i);
            std::string strPathIcon = obj->getPath();
            std::string strCount = obj->getCount();
            if ( obj->isString() == true )
            {
                MafUtils::convertNumberToShort(strCount);
            }
            
            if ( strPathIcon.length() == 0 || strCount.length() == 0 )
            {
                continue;
            }
            
            auto spriteIcon = Sprite::create(strPathIcon);
            spriteIcon->setPosition(spriteRewardBg->getContentSize().width / 2, spriteRewardBg->getContentSize().height / 2);
            spriteIcon->setScale(1.2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteRewardBg->addChild(spriteIcon);
            
            auto label = MafNode::createBmFont(strCount, GAME_BM_FONT_AL, 20);
            label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
            label->setPosition(spriteRewardBg->getContentSize().width / 2, -5);
            spriteRewardBg->addChild(label);
        }
    }
    
    //
    auto sprBottomBox = Sprite::create("Assets/ui/purchase/pass_payment_box1_2.png");
    sprBottomBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    sprBottomBox->setPosition(_layerContainerReward->getContentSize().width / 2, 0);
    _layerContainerReward->addChild(sprBottomBox);
    {
        auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_currentreward"), GAME_FONT, 30);
        labelTitle->setPosition(sprTopBox->getContentSize().width / 2, sprTopBox->getContentSize().height - 37.5);
        labelTitle->setDimensions(sprBottomBox->getContentSize().width - 10, labelTitle->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        sprBottomBox->addChild(labelTitle);
        
        //
        const int paddingX = 5;
        const int paddingY = 5;
        int drawRow = 1;
        int drawCol = (int)_listRewardNow.size();
        
        int width = 75 * drawCol + paddingX * (drawCol - 1);
        int height = 75 * drawRow + paddingY * (drawRow - 1);
        
        auto layerReward = Layer::create();
        layerReward->setContentSize( Size(width, height) );
        layerReward->setIgnoreAnchorPointForPosition(false);
        layerReward->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
        layerReward->setPosition( sprBottomBox->getContentSize().width / 2, 69.5);
        sprBottomBox->addChild(layerReward);
           
        for ( int i = 0; i < _listRewardNow.size(); i++ )
        {
            int posX = i * 75 + i * paddingX;
            int posY = layerReward->getContentSize().height / 2;
            
            auto spriteRewardBg = ui::Scale9Sprite::create("Assets/ui_common/box_item.png");
            spriteRewardBg->setContentSize(Size(75, 75));
            spriteRewardBg->setAnchorPoint( Vec2::ANCHOR_MIDDLE_LEFT );
            spriteRewardBg->setPosition(posX, posY);
            layerReward->addChild(spriteRewardBg);
            
            auto obj = _listRewardNow.at(i);
            std::string strPathIcon = obj->getPath();
            std::string strCount = obj->getCount();
            if ( obj->isString() == true )
            {
                MafUtils::convertNumberToShort(strCount);
            }
            
            if ( strPathIcon.length() == 0 || strCount.length() == 0 )
            {
                continue;
            }
            
            auto spriteIcon = Sprite::create(strPathIcon);
            spriteIcon->setPosition(spriteRewardBg->getContentSize().width / 2, spriteRewardBg->getContentSize().height / 2);
            spriteIcon->setScale(1.2);
            spriteIcon->getTexture()->setTexParameters(texParams);
            spriteRewardBg->addChild(spriteIcon);
            
            auto label = MafNode::createBmFont(strCount, GAME_BM_FONT_AL, 20);
            label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
            label->setPosition(spriteRewardBg->getContentSize().width / 2, -5);
            spriteRewardBg->addChild(label);
        }
    }
}

void PopupPurchaseDungeonPass::uiBuff()
{
    _layerContainerBuff->removeAllChildren();
    
    //
    auto spriteBG = ui::Scale9Sprite::create("Assets/ui_common/box_02.png", Rect::ZERO, Rect(10,10,10,10));
    spriteBG->setContentSize(_layerContainerBuff->getContentSize());
    spriteBG->setPosition(_layerContainerBuff->getContentSize() / 2);
    _layerContainerBuff->addChild(spriteBG);
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_dungeonpass_v2_13"), GAME_FONT, 24);
    labelTitle->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 30);
    labelTitle->setColor(COLOR_TITLE);
    spriteBG->addChild(labelTitle);
    
    //
    std::string listBuffTextKey[] = {"t_ui_dungeonpass_v2_2", "t_ui_dungeonpass_v2_3", "t_ui_dungeonpass_v2_4", "t_ui_dungeonpass_v2_5"};
    for ( int i = 0; i < sizeof(listBuffTextKey)/sizeof(std::string); i++ )
    {
        double posX = 30;
        double posY = 160 - 25 * i - 10 * i;
        
        auto labelBuff = Label::createWithTTF(GAME_TEXT(listBuffTextKey[i]), GAME_FONT, 22);
        labelBuff->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        labelBuff->setPosition(posX, posY);
        spriteBG->addChild(labelBuff);
    }
}

#pragma mark - draw
void PopupPurchaseDungeonPass::drawTime()
{
    auto action1 = CallFuncN::create([=](Node *sender){
        
        auto labelTime = (Label*)sender;
        
        if ( _objPass == nullptr )
        {
            return;
        }
        
        time_t nDiffPurchase = _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime();
        time_t nDiffReset = _objPass->getResetTime() - UtilsDate::getInstance()->getTime();
        if ( nDiffPurchase < 0 ) nDiffPurchase = 0;
        if ( nDiffReset < 0 ) nDiffReset = 0;
        
        std::string strTime = "";
        if ( _objPass->isBuyPass() == true )
        {
            int nDay = (int)(nDiffReset / 86400);
            int nHour = nDiffReset % 86400 / 3600;
            int nMin = nDiffReset % 3600 / 60;
            strTime = GAME_TEXTFORMAT("t_ui_dungeonpass_timeremain", nDay, nHour, nMin);
        }
        else
        {
            int nDay = (int)(nDiffPurchase / 86400);
            int nHour = nDiffPurchase % 86400 / 3600;
            int nMin = nDiffPurchase % 3600 / 60;
            strTime = GAME_TEXTFORMAT("t_server_dailymission_weekleft", nDay, nHour, nMin);
        }
        
        labelTime->setString(strTime);
        labelTime->setDimensions(250, labelTime->getContentSize().height);
        labelTime->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        labelTime->setOverflow(Label::Overflow::SHRINK);
        labelTime->setColor(Color3B::WHITE);
        if ( _objPass->isBuyPass() == false && nDiffPurchase <= 0 )
            labelTime->setColor(Color3B::RED);
    });
    auto action2 = DelayTime::create(10);
    auto seq = Sequence::create(action1, action2, NULL);
    _labelTime->stopAllActions();
    _labelTime->runAction(RepeatForever::create(seq));
}

#pragma mark - set, get
void PopupPurchaseDungeonPass::setCallback(std::function<void(void)> pCallback)
{
    _callbackRedraw = pCallback;
}

#pragma mark - click
void PopupPurchaseDungeonPass::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupPurchaseDungeonPass::onClickPurchase(cocos2d::Ref* sender)
{
    if ( _objPass == nullptr )
    {
        return;
    }
    
    if ( _objPass->isBuyPass() || _objPass->getPurchaseTime() - UtilsDate::getInstance()->getTime() <= 600 )
    {
        return;
    }
    
    PopupLoading::show();
    CashManager::getInstance()->purchaseInitiate(E_CASH::CASH_DUNGEON_PASS_1, CC_CALLBACK_4(PopupPurchaseDungeonPass::callbackInAppPurchasePass, this));
}

#pragma mark - callback
void PopupPurchaseDungeonPass::callbackInAppPurchasePass(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    PopupLoading::hide();
    
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        // alert
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        // redraw
        PopupLoading::show();
        PassManager::getInstance()->requestInfo(CC_CALLBACK_1(PopupPurchaseDungeonPass::callbackInfo, this));
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::MINE);
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

void PopupPurchaseDungeonPass::callbackInfo(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        //
        hide();
        
        return;
    }
    
    if ( _callbackRedraw != nullptr )
    {
        _callbackRedraw();
    }
    
    //
    hide();
}
