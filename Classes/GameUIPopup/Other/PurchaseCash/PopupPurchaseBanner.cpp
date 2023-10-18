//
//  PopupPurchaseBanner.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupPurchaseBanner.h"

#include "ManagerGame/UserInfoManager.h"

#include "ManagerEvent/EventManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

PopupPurchaseBanner* PopupPurchaseBanner::create()
{
    PopupPurchaseBanner* pRet = new(std::nothrow) PopupPurchaseBanner();
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

PopupPurchaseBanner::PopupPurchaseBanner(void) :
_nBannerIdx(0),

_nBannerSelectIdx(0),
_nBannerTotal(0),
_nBannerCount(0),
_nBannerBonusCount(0),
_nDownloadSuccess(0),
_nDownloadError(0),

_layerContainerTitle(nullptr),
_layerContainerContents(nullptr),
_layerContainerButton(nullptr),

_table(nullptr)
{
    
}

PopupPurchaseBanner::~PopupPurchaseBanner(void)
{
    
}

bool PopupPurchaseBanner::init()
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

void PopupPurchaseBanner::onEnter()
{
    PopupBase::onEnter();
    
    // network
    PopupLoading::show();
    CashManager::getInstance()->requestBannerInfo(CC_CALLBACK_1(PopupPurchaseBanner::callbackInfo, this));
    
}

void PopupPurchaseBanner::show()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP, TAG_SCENE_POPUP);
    }
    
    runAction( FadeTo::create(0.4, 250) );
}

#pragma mark - table
void PopupPurchaseBanner::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupPurchaseBanner::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    auto obj = _listBanner.at(idx);
    auto spriteTemp = Sprite::create(obj->getBGPath());
    spriteTemp->setScale(0.9f);
    
    return Size(table->getContentSize().width, spriteTemp->getBoundingBox().size.height);
}

extension::TableViewCell* PopupPurchaseBanner::tableCellAtIndex(extension::TableView *table, ssize_t idx)
{
    auto cell = table->dequeueCell();
    if ( cell == nullptr )
    {
        cell = new extension::TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    
    // size
    Size size = tableCellSizeForIndex(table, idx);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
 
    // obj
    auto obj = _listBanner.at(idx);
    auto spriteBG = Sprite::create(obj->getBGPath());
    
    auto itemBG = MafNode::MafMenuItemSprite::create(spriteBG, nullptr, nullptr);
    itemBG->setPosition(size.width / 2, size.height / 2);
    itemBG->setTag((int)idx);
    itemBG->setScale(0.9);
    menu->addChild(itemBG);
    
    if ( obj->getItemIdx() == 0 )
    {
        itemBG->setCallback(CC_CALLBACK_1(PopupPurchaseBanner::onClickBonus, this));
        
        //
        if ( obj->isReceive() == true )
        {
            auto spriteBuy = Sprite::create("Assets/ui/shop_package/btn_special_shop_bonus_1.png");
            spriteBuy->setPosition(itemBG->getContentSize().width / 2, 40);
            itemBG->addChild(spriteBuy);
        }
        else
        {
            auto spriteProgressBG = Sprite::create("Assets/ui/shop_package/special_shop_bonus_2_1.png");
            spriteProgressBG->setPosition(itemBG->getContentSize().width / 2, 40);
            itemBG->addChild(spriteProgressBG);
            {
                int nPercent = (double)_nBannerCount / _nBannerTotal * 100;
                if ( nPercent > 100 )
                    nPercent = 100;
                
                auto progressPurchase = ProgressTimer::create(Sprite::create("Assets/ui/shop_package/special_shop_bonus_2_2.png"));
                progressPurchase->setPosition(spriteProgressBG->getContentSize()/2);
                progressPurchase->setType(ProgressTimer::Type::BAR);
                progressPurchase->setBarChangeRate(Vec2(1,0));
                progressPurchase->setMidpoint(Vec2(0, 0.5));
                progressPurchase->setPercentage(nPercent);
                spriteProgressBG->addChild(progressPurchase);
                
                std::string strProgressCount = MafUtils::format("%d/%d", _nBannerCount, _nBannerTotal);
                auto labelProgressCount = Label::createWithTTF(strProgressCount, GAME_FONT, 24);
                labelProgressCount->setPosition(spriteProgressBG->getContentSize().width / 2, spriteProgressBG->getContentSize().height / 2);
                labelProgressCount->enableBold();
                spriteProgressBG->addChild(labelProgressCount);
            }
        }
    }
    else
    {
        itemBG->setCallback(CC_CALLBACK_1(PopupPurchaseBanner::onClickBuy, this));
        
        //
        if ( obj->isReceive() == true )
        {
            auto labelBuy = Label::createWithTTF(GAME_TEXT("t_ui_popshop_stepdone"), GAME_FONT, 30);
            labelBuy->setPosition(itemBG->getContentSize().width / 2, 40);
            labelBuy->setTextColor(Color4B(255, 216, 0, 255));
            labelBuy->enableBold();
            itemBG->addChild(labelBuy);
            
            //
            itemBG->setOpacity(150);
        }
        else
        {
            auto labelBuy = Label::createWithSystemFont(GAME_TEXT("t_ui_shop_price_loading"), SYSTEM_FONT, 36);
            labelBuy->setPosition(itemBG->getContentSize().width / 2, 40);
            labelBuy->setTextColor(Color4B(255, 216, 0, 255));
            labelBuy->enableBold();
            itemBG->addChild(labelBuy);
            
            auto objProduct = CashManager::getInstance()->getProduct(obj->getItemIdx());
            if ( objProduct != nullptr )
            {
                std::string strPrice = objProduct->getPrice();
                if ( strPrice.length() != 0 )
                {
                    labelBuy->setString(strPrice);
                }
                else
                {
                    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
                        
                        std::string strPrice = objProduct->getPrice();
                        if ( strPrice.length() != 0 )
                        {
                            ((Label*)sender)->setString(strPrice);
                            ((Label*)sender)->stopAllActions();
                            return;
                        }
                    });
                    auto action2 = DelayTime::create(5.0);
                    auto seq = Sequence::create(action1, action2, nullptr);
                    labelBuy->runAction( RepeatForever::create(seq) );
                }
            }
        }
    }
    
    return cell;
}

ssize_t PopupPurchaseBanner::numberOfCellsInTableView(extension::TableView *table)
{
    size_t size = _listBanner.size();
    return size;
}

#pragma mark - init
void PopupPurchaseBanner::initVar()
{
    _downloader.reset(new network::Downloader());
    _downloader->onTaskProgress = std::bind(&PopupPurchaseBanner::downloadTaskProgress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    _downloader->onFileTaskSuccess = std::bind(&PopupPurchaseBanner::downloadFileSuccess, this, std::placeholders::_1);
    _downloader->onTaskError = std::bind(&PopupPurchaseBanner::downloadTaskError, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    
    //
    CashManager::getInstance()->setBannerGrade(0);
}

void PopupPurchaseBanner::initUi()
{
    //
    _layerContainerTitle = LayerColor::create();
    _layerContainerTitle->setContentSize(Size(getContentSize().width, 80 + UserInfoManager::getInstance()->_fHeightIphoneX) );
    _layerContainerTitle->setIgnoreAnchorPointForPosition(false);
    _layerContainerTitle->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTitle->setPosition(getContentSize().width / 2, getContentSize().height );
    addChild(_layerContainerTitle);
    
    _layerContainerButton = LayerColor::create();
    _layerContainerButton->setContentSize( Size(getContentSize().width, 116) );
    _layerContainerButton->setIgnoreAnchorPointForPosition(false);
    _layerContainerButton->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerButton->setPosition( Vec2(getContentSize().width / 2, 25) );
    addChild(_layerContainerButton);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize(Size(getContentSize().width, getContentSize().height - _layerContainerTitle->getContentSize().height - _layerContainerButton->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerContents->setPosition(getContentSize().width / 2, getContentSize().height - _layerContainerTitle->getContentSize().height );
    addChild(_layerContainerContents);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainerTitle->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupPurchaseBanner::onClickClose, this));
    itemClose->setPosition(_layerContainerTitle->getContentSize().width - itemClose->getContentSize().width, (_layerContainerTitle->getContentSize().height - UserInfoManager::getInstance()->_fHeightIphoneX) / 2);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    
    // label
    _labelTime = Label::createWithTTF("00:00:00", GAME_FONT, 24);
    _labelTime->setPosition(_layerContainerTitle->getContentSize().width / 2, 20);
    _labelTime->setColor(COLOR_COIN);
    _layerContainerTitle->addChild(_labelTime);
    
    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
    
        if ( EventManager::getInstance()->isActivatedEvent(21) == false && EventManager::getInstance()->isActivatedEvent(22) == false )
        {
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
            ((Label*)sender)->stopAllActions();
            return;
        }

        int64_t nTimeDiff = CashManager::getInstance()->getBannerTimeRemaining();
        if ( nTimeDiff < 0 )
        {
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
            ((Label*)sender)->stopAllActions();
            return;
        }

        std::string strSaleTime = UtilsDate::getInstance()->getTimeString(nTimeDiff, UtilsDate::eType::date_user_type_1);
        ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", strSaleTime.c_str()));
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    _labelTime->runAction( RepeatForever::create(seq) );
    
    //
    uiContents();
    uiButton();
}

#pragma mark - ui
void PopupPurchaseBanner::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    //
    _table = extension::TableView::create(this, Size(_layerContainerContents->getContentSize().width, _layerContainerContents->getContentSize().height));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _table->setPosition(_layerContainerContents->getContentSize().width / 2, 0);
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerContents->addChild(_table);
    
    //
    _table->reloadData();
}

void PopupPurchaseBanner::uiButton()
{
    //
    _layerContainerButton->removeAllChildren();
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(_layerContainerButton->getContentSize().width / 2, _layerContainerButton->getContentSize().height / 2);
    _layerContainerButton->addChild(menu);


    //
    auto spriteMove = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteMove->setContentSize(Size(556, 78));
    auto spriteSelectMove = ui::Scale9Sprite::create("Assets/ui_common/btn_1_1_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteSelectMove->setContentSize(Size(556, 78));
    
    auto itemMove = MafNode::MafMenuItemSprite::create(spriteMove, spriteSelectMove, nullptr, CC_CALLBACK_1(PopupPurchaseBanner::onClickMove, this));
    menu->addChild(itemMove);
    {
        _labelMove = Label::createWithTTF(GAME_TEXT("t_ui_shop_banner_pro"), GAME_FONT, 24);
        _labelMove->setPosition(itemMove->getContentSize().width / 2, itemMove->getContentSize().height / 2);
        itemMove->addChild(_labelMove);
    }
    
    //
    menu->alignItemsHorizontallyWithPadding(10);
    
    //
    drawButton();
}

#pragma mark - draw
void PopupPurchaseBanner::drawContetns()
{
    _nBannerCount = 0;
    for ( const auto &obj : _listBanner)
    {
        if ( obj->getItemIdx() != 0 && obj->isReceive() == true )
        {
            _nBannerCount++;
        }
    }
    
    //
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    if ( pos.y <= 0 )
    {
        _table->setContentOffset(pos);
    }
}

void PopupPurchaseBanner::drawButton()
{
    if ( CashManager::getInstance()->getBannerGrade() == 0 )
    {
        _labelMove->setString(GAME_TEXT("t_ui_shop_banner_pro"));
    }
    else
    {
        _labelMove->setString(GAME_TEXT("t_ui_shop_banner_normal"));
    }
}

#pragma mark - set, get, other


#pragma mark - click
void PopupPurchaseBanner::onClickClose(Ref* sender)
{
    hide();
}

void PopupPurchaseBanner::onClickMove(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    //
    if ( _nBannerBonusCount == 0 && _nBannerTotal <= _nBannerCount )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_shop_banner_bonus"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else
    {
        int nGrade = CashManager::getInstance()->getBannerGrade();
        CashManager::getInstance()->setBannerGrade(nGrade == 0 ? 1 : 0);
        
        // network
        PopupLoading::show();
        CashManager::getInstance()->requestBannerInfo(CC_CALLBACK_1(PopupPurchaseBanner::callbackInfo, this));
    }
}
    
void PopupPurchaseBanner::onClickBuy(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    _nBannerSelectIdx = item->getTag();
    auto objBanner = _listBanner.at(_nBannerSelectIdx);
    if ( objBanner->isReceive() == true )
    {
        return;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(objBanner->getItemIdx());
    if ( objProduct == nullptr )
    {
        return;
    }
    
    int nTimeRemaining = CashManager::getInstance()->getBannerTimeRemaining();
    if ( nTimeRemaining <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_9_6"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            PopupLoading::show();
            CashManager::getInstance()->requestBannerInfo(CC_CALLBACK_1(PopupPurchaseBanner::callbackInfo, this));
        });
        popup->show();
        return;
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    int nItemIdx = objBanner->getItemIdx();
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupPurchaseBanner::callbackPurchase, this));
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
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupPurchaseBanner::callbackPurchase, this));
        }
    });
}

void PopupPurchaseBanner::onClickBonus(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    
    //
    SoundManager::SoundEffectStart(SOUND_UPGRADE);
    
    _nBannerSelectIdx = item->getTag();
    auto objBanner = _listBanner.at(_nBannerSelectIdx);
    if ( objBanner->isReceive() == true )
    {
        return;
    }
    
    int nCount = 0;
    for ( const auto &obj : _listBanner)
    {
        if ( obj->getItemIdx() != 0 && obj->isReceive() == true )
        {
            nCount++;
        }
    }
    _nBannerCount = nCount;
    if ( _nBannerCount < _nBannerTotal )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_popshop_bonus_guide"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    PopupLoading::show();
    CashManager::getInstance()->requestBannerBonusReceive(CC_CALLBACK_1(PopupPurchaseBanner::callbackBonus, this));
}

#pragma mark - callback
void PopupPurchaseBanner::callbackInfo(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        hide();
        return;
    }
    
    _listBanner.clear();
    _listBanner = CashManager::getInstance()->getBannerAll();
    _nBannerIdx = CashManager::getInstance()->getBannerIdx();
    
    _nBannerCount = 0;
    _nBannerBonusCount = 0;
    _nBannerTotal = 0;
    
    _listDownload.clear();
    for ( const auto &obj : _listBanner)
    {
        if ( FileUtils::getInstance()->isFileExist(obj->getBGPath()) == false )
        {
            _listDownload.push_back(obj->getBGPath());
        }
        
        if ( obj->getItemIdx() == 0 )
        {
            if ( obj->isReceive() == true )
            {
                _nBannerBonusCount++;
            }
        }
        else
        {
            _nBannerTotal++;
            
            if ( obj->isReceive() == true )
            {
                _nBannerCount++;
            }
        }
        
    }
    
    // label : time
    _labelTime->stopAllActions();
    _labelTime->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
    auto action1 = CallFuncN::create( [=]( Ref* sender ) {
    
        if ( EventManager::getInstance()->isActivatedEvent(21) == false && EventManager::getInstance()->isActivatedEvent(22) == false )
        {
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
            ((Label*)sender)->stopAllActions();
            return;
        }

        if ( _nBannerIdx != CashManager::getInstance()->getBannerIdx() )
        {
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
            ((Label*)sender)->stopAllActions();
            
            callbackInfo(true);
            return;
        }
        
        int nTimeDiff = CashManager::getInstance()->getBannerTimeRemaining();
        if ( nTimeDiff <= 0 )
        {
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", "00:00:00"));
        }
        else
        {
            std::string strSaleTime = UtilsDate::getInstance()->getTimeString(nTimeDiff, UtilsDate::eType::date_user_type_1);
            ((Label*)sender)->setString(GAME_TEXTFORMAT("t_ui_popshop_time", strSaleTime.c_str()));
        }
    });
    auto action2 = DelayTime::create(1.0);
    auto seq = Sequence::create(action1, action2, nullptr);
    if ( CashManager::getInstance()->getBannerTimeRemaining() > 0 )
    {
        _labelTime->runAction( RepeatForever::create(seq) );
    }
    
    
    //
    PopupLoading::show();
    
    _nDownloadSuccess = 0;
    _nDownloadError = 0;
    downloadStart();
}

void PopupPurchaseBanner::callbackPurchaseCheck(bool bResult)
{
    PopupLoading::hide();
    
    auto objBanner = _listBanner.at(_nBannerSelectIdx);
    if ( bResult == false )
    {
        objBanner->setReceive(true);
        drawContetns();
        drawButton();
        return;
    }
    
    auto objProduct = CashManager::getInstance()->getProduct(objBanner->getItemIdx());
    if ( objProduct == nullptr )
    {
        return;
    }
    
    /*
    0 : 오류 없음
    1 : 다른상품 결제 오류 있음.
    2 : 같은상품 결제 오류 있음.
    */
    int nItemIdx = objBanner->getItemIdx();
    CashManager::getInstance()->purchaseHas(nItemIdx, [=](int nHas){
        if ( nHas == 2 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_44"));
            popup->addButton(GAME_TEXT("t_ui_shop_msg_6"), true, [=](Ref* sender){
                CashManager::getInstance()->purchaseConsumeAll();
            });
            popup->addButton(GAME_TEXT("t_ui_confirm"), false, [=](Ref* sender){
                PopupLoading::show();
                CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupPurchaseBanner::callbackPurchase, this));
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
            CashManager::getInstance()->purchaseInitiate(nItemIdx, CC_CALLBACK_4(PopupPurchaseBanner::callbackPurchase, this));
        }
    });
}

void PopupPurchaseBanner::callbackPurchase(MafGooglePlay::RESULT eResult, int nResult, int nIdx, std::string strItems)
{
    //
    PopupLoading::hide();
    
    //
    if ( eResult == MafGooglePlay::SUCCESS )
    {
        //
        auto objBanner = _listBanner.at(_nBannerSelectIdx);
        objBanner->setReceive(true);
        
        //
        drawContetns();
        drawButton();
        
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

void PopupPurchaseBanner::callbackBonusCheck(bool bResult)
{
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        drawContetns();
        drawButton();
        return;
    }
    
    PopupLoading::show();
    CashManager::getInstance()->requestBannerBonusReceive(CC_CALLBACK_1(PopupPurchaseBanner::callbackBonus, this));
}

void PopupPurchaseBanner::callbackBonus(bool bResult)
{
    //
    PopupLoading::hide();
    
    //
    if ( bResult == false )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        drawContetns();
        drawButton();
        return;
    }
    
    //
    auto objBanner = _listBanner.at(_nBannerSelectIdx);
    objBanner->setReceive(true);
    
    _nBannerBonusCount++;
    
    //
    drawContetns();
    drawButton();
    
    // alert
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_cash_msg_1"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
}

#pragma mark - download
void PopupPurchaseBanner::downloadStart()
{
    if ( _listDownload.size() == 0 )
    {
        downloadEnd();
        return;
    }
    
    // 파일 경로
    std::string strFilePath = _listDownload.at(0);
    _listDownload.erase(_listDownload.begin());
    
    // 다운 경로
    std::string strUrl = "https://storage.googleapis.com/indigo-lambda-174903_gamedata/images/";
    strUrl.append(strFilePath);
    
    // 저장 경로
    std::string strStoragePathFull = FileUtils::getInstance()->getWritablePath() + strFilePath;
    std::string strStoragePath = strStoragePathFull.substr(0, strStoragePathFull.find_last_of("/") + 1);
    
    // 파일 생성
    if ( FileUtils::getInstance()->isDirectoryExist(strStoragePath) == false )
    {
        FileUtils::getInstance()->createDirectory(strStoragePath);
    }
    
    // 이전에 파일이 있으면 삭제.
    FileUtils::getInstance()->removeDirectory(strStoragePathFull);
    FileUtils::getInstance()->removeDirectory(strStoragePathFull +  + ".tmp");
    FileUtils::getInstance()->removeFile(strStoragePathFull);
    FileUtils::getInstance()->removeFile(strStoragePathFull + ".tmp");
    
    //
    _downloader->createDownloadFileTask(strUrl, strStoragePathFull);
}

void PopupPurchaseBanner::downloadEnd()
{
    //
    PopupLoading::hide();
    
    if ( _nDownloadError > 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    
    //
    drawContetns();
    drawButton();
}

void PopupPurchaseBanner::downloadTaskProgress(const cocos2d::network::DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
{
    
}

void PopupPurchaseBanner::downloadFileSuccess(const cocos2d::network::DownloadTask& task)
{
    _nDownloadSuccess++;
    
    //
    downloadStart();
}

void PopupPurchaseBanner::downloadTaskError(const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    _nDownloadError++;
    
    //
    downloadStart();
}
