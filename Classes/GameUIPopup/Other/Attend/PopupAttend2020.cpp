//
//  PopupAttend2020.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupAttend2020.h"

#include "Common/ConfigGameNormal.h"

#include "GameObject/InfoAttend.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatchDouble.h"
#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/CashManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"

USING_NS_CC;

PopupAttend2020* PopupAttend2020::create()
{
    PopupAttend2020* pRet = new(std::nothrow) PopupAttend2020();
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

PopupAttend2020::PopupAttend2020(void) :
_nMonth(0),
_nToday(0),
_bBonus(false),

_nSelectday(0),
_bAds(false),
_nReceiveCount(0),

_layerContainerContents(nullptr),
_layerContainerInfo(nullptr),
_labelTitle(nullptr),
_table(nullptr)
{
    
}

PopupAttend2020::~PopupAttend2020(void)
{
    
}

bool PopupAttend2020::init()
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

void PopupAttend2020::onEnter()
{
    PopupBase::onEnter();
    
    // network
    requestInfo();
}

#pragma mark - table
void PopupAttend2020::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

Size PopupAttend2020::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 116);
}

extension::TableViewCell* PopupAttend2020::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    
 
    // obj
    ssize_t nIdxStart = idx * 5;
    ssize_t nIdxEnd = nIdxStart + 5;
    int nSize = (int)_listAttend.size();
    if ( nIdxEnd > nSize )
    {
        nIdxEnd = nSize;
    }
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    cell->addChild(menu);
    
    for ( int i = 0; nIdxStart + i < nIdxEnd; i++ )
    {
        int nIdx = (int)nIdxStart + i;
        
        auto objAttend = _listAttend.at(nIdx);
        auto objItem = objAttend->getReward();
        
        
        int nDay = objAttend->getDay();
        
        double nPosX = 120 * i;
        double nPosY = size.height / 2;
        

        if ( nDay == 0 )
        {
            auto spriteCellBG = Sprite::create("Assets/ui/daily_event/event_attendance_gift_01.png");
            spriteCellBG->setCascadeColorEnabled(true);
            spriteCellBG->setCascadeOpacityEnabled(true);

            auto item = MafNode::MafMenuItemSprite::create(spriteCellBG, nullptr, CC_CALLBACK_1(PopupAttend2020::onClickReward, this));
            item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            item->setPosition(nPosX, nPosY);
            item->setTag(nIdx);
            item->setEnabled(false);
            menu->addChild(item);

            auto labelText = MafNode::createBmFont("DAY", GAME_BM_FONT_AL, 18);
            labelText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            labelText->setPosition(item->getContentSize().width / 2 - 10, 10);
            labelText->enableBold();
            item->addChild(labelText);

            auto labelCount = MafNode::createBmFont(MafUtils::format("%d / 20", _nReceiveCount), GAME_BM_FONT_AL, 18);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            labelCount->setPosition(item->getContentSize().width / 2 + 10, 10);
            labelCount->enableBold();
            labelCount->setColor(COLOR_TITLE);
            item->addChild(labelCount);

            if ( _bBonus == true )
            {
                auto spriteComplete = Sprite::create("Assets/ui/daily_event/event_attendance_gift_02.png");
                spriteComplete->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
                item->addChild(spriteComplete);
            }
        }
        else
        {
            std::string strCellBG = "Assets/ui/daily_event/event_attendance_box1_1.png";
            if ( objAttend->isReceive() == true )     strCellBG = "Assets/ui/daily_event/event_attendance_box2_1.png";
            else if ( nDay == _nToday )         strCellBG = "Assets/ui/daily_event/event_attendance_box1_2.png";
            
            auto spriteCellBG = Sprite::create(strCellBG);
            spriteCellBG->setCascadeColorEnabled(true);
            spriteCellBG->setCascadeOpacityEnabled(true);
            
            auto item = MafNode::MafMenuItemSprite::create(spriteCellBG, nullptr, CC_CALLBACK_1(PopupAttend2020::onClickReward, this));
            item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            item->setPosition(nPosX, nPosY);
            item->setTag(nIdx);
            menu->addChild(item);
            
            auto labelAttendDay = Label::createWithTTF(MafUtils::toString(nDay), GAME_FONT, 19);
            labelAttendDay->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            labelAttendDay->setPosition(35, item->getContentSize().height - 35);
            labelAttendDay->setColor(Color3B(199, 172, 143));
            labelAttendDay->enableBold();
            item->addChild(labelAttendDay);
            
            auto spriteReward = Sprite::create(objItem->getPath());
            spriteReward->setPosition(item->getContentSize().width / 2, 30 + 40);
            spriteReward->setScale(1.5);
            item->addChild(spriteReward);
            
            auto labelReward = Label::createWithTTF(objItem->getCount(), GAME_FONT, 24);
            labelReward->setPosition(item->getContentSize().width / 2, 30);
            labelReward->enableBold();
            item->addChild(labelReward);
            if ( nDay == _nToday  )
            {
                if ( objAttend->isReceive() == false )
                {
                    labelReward->setColor(Color3B::RED);
                    
                    auto animation = Animation::create();
                    animation->setDelayPerUnit(0.15);
                    for ( int i = 1; i <= 4; i++ )
                    {
                        animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/tutorial/guide_finger%d_%d.png",MafRemoteConfig::GetRemoteValueInt("tutorial_hand_color"),i));
                    }
                    
                    auto spriteAds = Sprite::create(MafUtils::format("Assets/ui/tutorial/guide_finger%d_1.png",MafRemoteConfig::GetRemoteValueInt("tutorial_hand_color")));
                    spriteAds->setPosition(item->getContentSize().width / 2 + 20, item->getContentSize().height / 2);
                    spriteAds->runAction(RepeatForever::create(Animate::create(animation)));
                    item->addChild(spriteAds);
                }
            }
            
            if ( nDay < _nToday && objAttend->isReceive() == false )
            {
                auto spriteAds = Sprite::create("Assets/ui/daily_event/event_attendance_box2_2.png");
                spriteAds->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
                item->addChild(spriteAds);
            }
            else if ( objAttend->isReceive() == true )
            {
                labelReward->setColor(Color3B(173,173,173));
                
                auto spriteAds = Sprite::create("Assets/ui/daily_event/event_attendance_box2_1.png");
                spriteAds->setPosition(item->getContentSize().width / 2, item->getContentSize().height / 2);
                item->addChild(spriteAds);
            }
        }
    }
    
    return cell;
}

ssize_t PopupAttend2020::numberOfCellsInTableView(extension::TableView *table)
{
    size_t sizeOriginal = _listAttend.size();
    size_t size = sizeOriginal / 5;
    if ( sizeOriginal % 5 != 0 )
        size++;
    
    return size;
}

#pragma mark - init
void PopupAttend2020::initVar()
{
    time_t nTime = UtilsDate::getInstance()->getTime();
    struct tm *info = localtime(&nTime);
    
    _nMonth = info->tm_mon + 1;
    
    //
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ATTENDANCE_MONTHLY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        _listAttendAll.clear();
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int type = jsonValue["month"].GetInt();
            int day = jsonValue["day"].GetInt();
            std::string reward = jsonValue["reward"].GetString();
            
            // obj
            auto obj = InfoAttend::create();
            obj->setType(type);
            obj->setDay(day);
            obj->setReward(reward);
            
            _listAttendAll.pushBack(obj);
        }
    }
}

void PopupAttend2020::initUi()
{
    //
    auto spriteBG = Sprite::create("Assets/ui/daily_event/event_attendance_bg1.png");
    spriteBG->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 100);
    addChild(spriteBG);
    
    
    //
    _layerContainerInfo = Layer::create();
    _layerContainerInfo->setContentSize(Size(645, 95) );
    _layerContainerInfo->setIgnoreAnchorPointForPosition(false);
    _layerContainerInfo->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerInfo->setPosition(spriteBG->getContentSize().width / 2, 35 );
    spriteBG->addChild(_layerContainerInfo);
    
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize(Size(600, spriteBG->getContentSize().height - 76 - 25 - 25 - _layerContainerInfo->getContentSize().height - 30));
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerContents->setPosition(spriteBG->getContentSize().width / 2, 35 + _layerContainerInfo->getContentSize().height + 25 );
    spriteBG->addChild(_layerContainerContents);
    
    // sprite icon
    std::string strIconPath = "Assets/ui/daily_event/event_attendance_bg2_1.png";
    switch (_nMonth) {
        case 3:case 4:case 5:
            strIconPath = "Assets/ui/daily_event/event_attendance_bg2_1.png";       break;
        case 6:case 7:case 8:
            strIconPath = "Assets/ui/daily_event/event_attendance_bg2_2.png";       break;
        case 9:case 10:case 11:
            strIconPath = "Assets/ui/daily_event/event_attendance_bg2_3.png";       break;
        case 12:case 1:case 2:
            strIconPath = "Assets/ui/daily_event/event_attendance_bg2_4.png";       break;
        default:
            break;
    }
    auto spriteIcon = Sprite::create(strIconPath);
    spriteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    spriteIcon->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 40);
    spriteBG->addChild(spriteIcon);
    
    // label title
    std::string strTitle = "";
    std::string strMonth = MafUtils::format("t_ui_month_%d", _nMonth);
    strMonth = GAME_TEXT(strMonth);
    strTitle = GAME_TEXTFORMAT("t_ui_menu_monthly_attend", strMonth.c_str());
    
    _labelTitle = Label::createWithTTF(strTitle, GAME_FONT, 32);
    _labelTitle->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 38);
    _labelTitle->setDimensions(450, _labelTitle->getContentSize().height);
    _labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelTitle->setOverflow(Label::Overflow::SHRINK);
    _labelTitle->setColor(COLOR_TITLE);
    spriteBG->addChild(_labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    spriteBG->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x_2.png"), nullptr, CC_CALLBACK_1(PopupAttend2020::onClickClose, this));
    itemClose->setPosition(spriteBG->getContentSize().width - itemClose->getContentSize().width, spriteBG->getContentSize().height - 38);
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiContents();
    uiInfo();
}

#pragma mark - ui
void PopupAttend2020::uiContents()
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

void PopupAttend2020::uiInfo()
{
    //
    _layerContainerInfo->removeAllChildren();
    
    auto layerBG = LayerColor::create(Color4B(199, 172, 143, 255));
    layerBG->setContentSize(_layerContainerInfo->getContentSize());
    layerBG->setIgnoreAnchorPointForPosition(false);
    layerBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    layerBG->setPosition(_layerContainerInfo->getContentSize().width / 2, _layerContainerInfo->getContentSize().height / 2);
    _layerContainerInfo->addChild(layerBG);
    
    //
    auto labeText = Label::createWithTTF(GAME_TEXT("t_ui_menu_monthly_attend_msg_1"), GAME_FONT, 20);
    labeText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    labeText->setPosition(20, layerBG->getContentSize().height / 2);
    labeText->setDimensions(layerBG->getContentSize().width - 40, labeText->getContentSize().height);
    labeText->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    labeText->setOverflow(Label::Overflow::SHRINK);
    labeText->setColor(Color3B(77, 77, 77));
    layerBG->addChild(labeText);
}

#pragma mark - draw
void PopupAttend2020::drawTitle()
{
    std::string strMonth = MafUtils::format("t_ui_month_%d", _nMonth);
    strMonth = GAME_TEXT(strMonth);
    
    std::string strTitle = GAME_TEXTFORMAT("t_ui_menu_monthly_attend", strMonth.c_str());
    _labelTitle->setString(strTitle);
}

void PopupAttend2020::drawContetns()
{
    //
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    if ( pos.y <= 0 )
    {
        _table->setContentOffset(pos);
    }
}

#pragma mark - set, get, other
void PopupAttend2020::setLoad(int month)
{
    InfoAttend* objAttendBonus = nullptr;
    
    _listAttend.clear();
    for ( auto& obj : _listAttendAll )
    {
        if ( obj->getType() == month )
        {
            if ( obj->getDay() == 0 )
            {
                objAttendBonus = obj;
            }
            else
            {
                _listAttend.pushBack(obj);
            }
        }
    }
    
    // 마지막에 보너스 추가
    if ( objAttendBonus != nullptr )
    {
        _listAttend.pushBack(objAttendBonus);
    }
}

#pragma mark - click
void PopupAttend2020::onClickClose(Ref* sender)
{
    hide();
}

void PopupAttend2020::onClickReward(Ref* sender)
{
    auto item = (MafNode::MafMenuItemSprite*)sender;

    int nIdx = item->getTag();
    
    auto objAttend = _listAttend.at(nIdx);
    if ( objAttend->isReceive() == true )
    {
        return;
    }
    
    int nDay = objAttend->getDay();
    if ( _nToday < nDay )
    {
        return;
    }
    
    auto objItem = objAttend->getReward();
    E_ADS_WATCH eTypeAdsWatch = E_ADS_WATCH::NONE;
    switch ((E_ITEMS)objItem->getIdx()) {
        case E_ITEMS::BUFF_COIN:        eTypeAdsWatch = E_ADS_WATCH::BUFF_COIN; break;
        case E_ITEMS::BUFF_ATK:         eTypeAdsWatch = E_ADS_WATCH::BUFF_ATK; break;
        case E_ITEMS::BUFF_SPD:         eTypeAdsWatch = E_ADS_WATCH::BUFF_SPD; break;
        case E_ITEMS::BUFF_ALL:         eTypeAdsWatch = E_ADS_WATCH::BUFF_ALL; break;
        case E_ITEMS::BUFF_ATK_9:       eTypeAdsWatch = E_ADS_WATCH::BUFF_ATK_9; break;
        case E_ITEMS::BUFF_ATK_10:      eTypeAdsWatch = E_ADS_WATCH::BUFF_ATK_10; break;
        case E_ITEMS::TICKET:           eTypeAdsWatch = E_ADS_WATCH::ITEM_TICKET; break;
        case E_ITEMS::GOLD:             eTypeAdsWatch = E_ADS_WATCH::ITEM_GOLD; break;
        case E_ITEMS::GEM:              eTypeAdsWatch = E_ADS_WATCH::ITEM_GEM; break;
        case E_ITEMS::CUBE:             eTypeAdsWatch = E_ADS_WATCH::ITEM_CUBE; break;
        case E_ITEMS::COSTUME_COIN:     eTypeAdsWatch = E_ADS_WATCH::ITEM_DEVILCOIN; break;
        case E_ITEMS::KEY:              eTypeAdsWatch = E_ADS_WATCH::ITEM_KEY; break;
        case E_ITEMS::POINT:            eTypeAdsWatch = E_ADS_WATCH::ITEM_POINT; break;
        case E_ITEMS::D_GOLD:           eTypeAdsWatch = E_ADS_WATCH::ITEM_DEVIL_GOLD; break;
        case E_ITEMS::PTICKET:          eTypeAdsWatch = E_ADS_WATCH::ITEM_PTICKET; break;
        case E_ITEMS::PET_EQUIP_BOX:    eTypeAdsWatch = E_ADS_WATCH::ITEM_PET_EQUIP_BOX; break;
        case E_ITEMS::MATERIAL:         eTypeAdsWatch = E_ADS_WATCH::ITEM_BONG; break;
        case E_ITEMS::MINE_SPECIAL_BOX: eTypeAdsWatch = E_ADS_WATCH::MINE_RAND_2; break;
        case E_ITEMS::MINE_BOX:         eTypeAdsWatch = E_ADS_WATCH::MINE_RAND_3; break;
//        case E_ITEMS::REWARD_BUFF_RAND : eType = E_ADS_WATCH::BUFF_RAND; break;
//        case E_ITEMS::REWARD_BUFF_DEVIL_RAND : eType = E_ADS_WATCH::BUFF_DEVIL_RAND; break;
//        case E_ITEMS::REWARD_SNOW_ADS : eType = E_ADS_WATCH::BUFF_EVENT_TIME; break;
//        case E_ITEMS::REWARD_MINE_N_R_RAND : eType = E_ADS_WATCH::MINE_RAND_1; break;
//        case E_ITEMS::REWARD_MINE_R_RAND : eType = E_ADS_WATCH::MINE_RAND_4; break;

        default:
            break;
    }
    
    
    _bAds = false;
    _nSelectday = nDay;
    if ( _nToday == nDay )
    {
        auto popup = PopupAdsWatchDouble::create(eTypeAdsWatch);
        popup->setCallbackResult([=](bool bResult){
            
            if ( bResult == false )
            {
                // network
                requestReward();
            }
            else
            {
                // ads
                if( CashManager::getInstance()->isAdsfreeTime() == true )
                {
                    callbackAds(ADS_RESULT::SUCCESS);
                }
                else
                {
                    MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupAttend2020::callbackAds, this));
                }
            }
        });
        popup->show();
    }
    else if ( objAttend->isReceive() == false )
    {
        // ads
        if( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            callbackAds(ADS_RESULT::SUCCESS);
        }
        else
        {
            auto popup = PopupAdsWatch::create(eTypeAdsWatch, 0);
            popup->setCallbackResult([=](){
                MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupAttend2020::callbackAds, this));
            });
            popup->show();
        }
    }
}

#pragma mark - callback
void PopupAttend2020::callbackAds(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        if ( _nToday == _nSelectday )
        {
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotAttend2x, CashManager::getInstance()->isAdsfreeTime());
        }
        else
        {
            MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotAttendRe, CashManager::getInstance()->isAdsfreeTime());
        }
        
        //
        _bAds = true;
        
        //
        requestReward();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupAttend2020::callbackAds, this));
            
            if ( _nToday == _nSelectday )
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotAttend2x);
            }
            else
            {
                popup->setLogParams(kAnalEventAdsLoadParameterSlot);
                popup->setLogValue(kAdsSlotAttendRe);
            }
            popup->show();
        }
        else
        {
            std::string strMessage = GAME_TEXT("t_ui_error_14");
            
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
            
        }
    }
}

#pragma mark - network
void PopupAttend2020::requestInfo()
{
    PopupLoading::show();
    
    //
    std::string url = "/v1/event/monthlyAttendance/info/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void PopupAttend2020::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && (jsonParser.HasMember("_rewardlist") == false || jsonParser.HasMember("_today") == false) )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        hide();
        return;
    }
    
    
    int result = jsonParser["_result"].GetInt();
    if ( result == 1 )
    {
        _nMonth = jsonParser["_monthly"].GetInt();
        _nToday = jsonParser["_today"].GetInt();
        _bBonus = jsonParser["_bonus"].GetBool();
        
        // load
        setLoad(_nMonth);
        
        // reward
        _nReceiveCount = 0;
        for ( int i = 0; i < jsonParser["_rewardlist"].Size(); i++ )
        {
            bool bReceive = jsonParser["_rewardlist"][i].GetBool();
            if ( bReceive == true )
            {
                _nReceiveCount++;
            }
            
            auto objAttend = _listAttend.at(i);
            objAttend->setReceive(bReceive);
        }
        
    }
    else
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        
        hide();
        return;
    }
    
    if ( _nToday <= 0 )
    {
        hide();
        return;
    }
    
    //
    drawTitle();
    drawContetns();
}

void PopupAttend2020::requestReward()
{
    PopupLoading::show();
    
    //
    std::string url = "/v1/event/monthlyAttendance/reward";
    
    auto req = new MafHttpNew(network::HttpRequest::Type::POST, url.c_str());
    req->setData("_useridx", AccountManager::getInstance()->getUserIdx());
    req->setData("_monthly", _nMonth);
    req->setData("_today", _nToday);
    req->setData("_day", _nSelectday);
    req->setData("_is_ads", _bAds);
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseReward(response,dd);
    };
    req->send(cb);
}

void PopupAttend2020::responseReward(cocos2d::network::HttpResponse* response,std::string &data)
{
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if(jsonParser.HasParseError())
    {
        bError = true;
    }
    
    if ( bError == false && jsonParser.HasMember("_result") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    int nResult = jsonParser["_result"].GetInt();
    if ( nResult == 1 )
    {// 정상
        std::string strReward = jsonParser["_reward"].GetString();
        std::string strItems = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        // network
        requestInfo();
    }
    else if ( nResult == 2 )
    {// 이벤트 기간이 아님
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_6"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
    }
    else
    {// 0:기타 오류, 3: 오늘 이후 날짜의 보상을 요구함, 4: 이벤트 유저정보가 없음, 5 : 보상을 이미 받았음
        std::string strMessage = GAME_TEXT("t_ui_error_30");
        if ( nResult == 5 )
        {
            strMessage = GAME_TEXT("t_ui_attend_msg_5");
        }
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strMessage);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            
            requestInfo();
        });
        popup->show();
    }
    
}
