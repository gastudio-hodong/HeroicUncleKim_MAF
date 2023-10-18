//
//  MailPopup.cpp
//  FantasyClicker
//
//  Created by KangChangJu on 2016. 2. 5..
//
//

#include "MailPopup.h"

#include "GameObject/InfoMail.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUIPopup/Other/Settings/Mail/MailReceivePopup.h"

#include "ManagerEvent/EventVoteManager.h"
#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/CashManager.h"

USING_NS_CC;

MailPopup::MailPopup(void) :
_nMailReceiveSuccess(0),
_nMailReceiveError(0),
_strAdsPath(""),
_strAdsLink(""),

_layerContainerMail(nullptr),

_table(nullptr),
_labelListNothing(nullptr)
{
    
}

MailPopup::~MailPopup(void)
{
    MafGoogleRewardAds::HideGoogleBanner();
}

bool MailPopup::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
 
    // init
    initVar();
    initUi();
    
    return true;
}

void MailPopup::onEnter()
{
    PopupBase::onEnter();
    
        // network
    if(EventVoteManager::getInstance()->getResultCode() == 1 ||
       EventVoteManager::getInstance()->getResultCode() == 20001)
    {
        EventVoteManager::getInstance()->requestInfo(true, [=](bool, int){
            auto action = CallFunc::create(CC_CALLBACK_0(MailPopup::requestMailList, this));
            this->runAction(action);
        });
    }
    else
    {
        auto action = CallFunc::create(CC_CALLBACK_0(MailPopup::requestMailList, this));
        this->runAction(action);
    }
}

#pragma mark - init
void MailPopup::initVar()
{
    std::string text = FileUtils::getInstance()->getStringFromFile(DATA_FILE_ADS);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)text.c_str());
    if ( jsonParser.HasParseError() == false )
    {
        int nIdx = -1;
        if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
        {
            if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
            {
                nIdx = 0;
            }
            else if ( TextManager::getInstance()->getLang().compare("jp") == 0 )
            {
                nIdx = 1;
            }
        }
        else
        {
            if ( TextManager::getInstance()->getLang().compare("ko") == 0 )
            {
                nIdx = 0;
            }
            else if ( TextManager::getInstance()->getLang().compare("jp") == 0 )
            {
                nIdx = 1;
            }
        }
        
        if ( nIdx != -1 )
        {
            for ( int i = 0; i < jsonParser["ads"].Size(); i++ )
            {
                const rapidjson::Value& jsonAds = jsonParser["ads"][i];

                std::string strLink = "";
                std::string strPathImage = "";
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                strLink = jsonAds["_ios"].GetString();
                strPathImage = jsonAds["_img_ios"].GetString();
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                strLink = jsonAds["_aos"].GetString();
                strPathImage = jsonAds["_img_aos"].GetString();
    #endif

                if ( strLink.length() == 0 || strPathImage.length() == 0 )
                {
                    continue;
                }

                if ( nIdx == i )
                {
                    _strAdsPath = strPathImage;
                    _strAdsLink = strLink;
                }
            }
        }
    }
}

void MailPopup::initUi()
{
    _spriteContainer->setContentSize( Size(729, 1070) );
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _spriteContainer->setPositionY(160);
    if ( _strAdsPath.empty() == true )
    {
        _spriteContainer->setPositionY(100);
    }
    
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_present"), GAME_FONT, 32);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(MailPopup::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5));
    itemClose->setTouchRect(itemClose->getContentSize()*2);
    menu->addChild(itemClose);
    
    // otehr
    uiAds();
    uiMail();
    
    //
    SpecialMissionManager::getInstance()->clearSpecialMission(84);
}

#pragma mark - touch
void MailPopup::tableCellTouched(extension::TableView* table, extension::TableViewCell* cell)
{
    
}

#pragma mark - table
Size MailPopup::tableCellSizeForIndex(extension::TableView *table, ssize_t idx)
{
    return Size(table->getContentSize().width, 210);
}

extension::TableViewCell* MailPopup::tableCellAtIndex(extension::TableView *table, ssize_t idx)
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
    auto obj = (InfoMail*)_listMail.at(idx);
    
    auto spriteBG = ui::Scale9Sprite::create(PIXEL,Rect(0,0,size.width,size.height - 10));
    spriteBG->setPosition(size.width / 2, size.height / 2);
    spriteBG->setColor(Color3B(29,41,51));
    cell->addChild(spriteBG);
    
    // top
    auto layerTop = Layer::create();
    layerTop->setContentSize(Size(spriteBG->getContentSize().width - 10, 65));
    layerTop->setIgnoreAnchorPointForPosition(false);
    layerTop->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    layerTop->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height - 5);
    spriteBG->addChild(layerTop);
    {
        auto labelTitle = Label::createWithSystemFont(obj->getTitle(), SYSTEM_FONT, 24);
        labelTitle->setAnchorPoint(Vec2::ZERO);
        labelTitle->setPosition(Vec2::ZERO);
        labelTitle->setDimensions(layerTop->getContentSize().width, layerTop->getContentSize().height);
        labelTitle->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
        labelTitle->setOverflow(Label::Overflow::SHRINK);
        layerTop->addChild(labelTitle);
    }
    
    // bottom
    auto layerBottom = LayerColor::create();
    layerBottom->setContentSize(Size(spriteBG->getContentSize().width - 10, 125));
    layerBottom->setIgnoreAnchorPointForPosition(false);
    layerBottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    layerBottom->setPosition(spriteBG->getContentSize().width / 2, 5);
    spriteBG->addChild(layerBottom);
    {
        auto layerScroll = ui::ScrollView::create();
        layerScroll->setBounceEnabled(false);
        layerScroll->setDirection(ui::ScrollView::Direction::HORIZONTAL);
        layerScroll->setContentSize(Size(layerBottom->getContentSize().width - 30 - 150, 116));
        layerScroll->setIgnoreAnchorPointForPosition(false);
        layerScroll->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layerScroll->setPosition(Vec2(10, layerBottom->getContentSize().height / 2));
        layerScroll->setSwallowTouches(false);
        layerBottom->addChild(layerScroll);
        
        double innerWidth = 0;
        double innerHeight = layerScroll->getContentSize().height;
        
        auto listItems = obj->getListItems();
        for ( int i = 0; i < listItems.size(); i++ )
        {
            auto objItem = listItems.at(i);
            
            double posX = 116 * i + 10 * i;
            double posY = layerScroll->getContentSize().height / 2;
            innerWidth = posX + 116;
            
            auto spriteItemBG = Sprite::create("Assets/ui_common/box_quest.png");
            spriteItemBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            spriteItemBG->setPosition(posX, posY);
            layerScroll->addChild(spriteItemBG);
            
            auto spriteItems = Sprite::create(objItem->getPath());
            if ( spriteItems != nullptr )
            {
                spriteItems->setPosition(spriteItemBG->getContentSize().width / 2, spriteItemBG->getContentSize().height / 2 + 10);
                spriteItems->getTexture()->setTexParameters(texParams);

                switch((E_ITEMS)objItem->getIdx())
                {
                    case E_ITEMS::BADGE : spriteItems->setScale(1); break;
                    case E_ITEMS::INFINITE_ARTIFACT : spriteItems->setScale(1); break;
                    case E_ITEMS::WEAPON_SPIRIT : spriteItems->setScale(1); break;
                    default: spriteItems->setScale(2); break;
                }
                spriteItemBG->addChild(spriteItems);
            }
            
            std::string strItemsCount = objItem->getCount();
            if ( objItem != nullptr && objItem->isString() == true )
            {
                MafUtils::convertNumberToShort(strItemsCount);
            }

            auto labelCount = Label::createWithTTF(strItemsCount, GAME_FONT, 24);
            labelCount->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelCount->setPosition(spriteItemBG->getContentSize().width / 2, 5);
            labelCount->setColor(COLOR_COIN);
            spriteItemBG->addChild(labelCount);
        }
        layerScroll->setInnerContainerSize(Size(innerWidth, innerHeight));
        
        //
        auto menu = MafNode::MafMenu::create();
        menu->setPosition(Vec2::ZERO);
        layerBottom->addChild(menu);
        
        auto itemReceive = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_upgrade_bg_2.png"), nullptr, Sprite::create("Assets/ui_common/btn_upgrade_bg_2_max.png"), CC_CALLBACK_1(MailPopup::onClickMailRecevice,this));
        itemReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        itemReceive->setPosition(layerBottom->getContentSize().width - 10, layerBottom->getContentSize().height / 2);
        itemReceive->setTag((int)idx);
        menu->addChild(itemReceive);
        {
            auto labelReceive = Label::createWithTTF(GAME_TEXT("t_ui_attend_receive"), GAME_FONT, 24);
            labelReceive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelReceive->setPosition(itemReceive->getContentSize().width / 2, itemReceive->getContentSize().height / 2);
            labelReceive->setDimensions(itemReceive->getContentSize().width - 5, labelReceive->getContentSize().height);
            labelReceive->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            labelReceive->setOverflow(cocos2d::Label::Overflow::SHRINK);
            itemReceive->addChild(labelReceive);
        }
        
        {
            int64_t nItemTimeEnd = obj->getDeleteTime();
            auto action1 = CallFuncN::create([=](Node* sender){
                
                auto itemReceive = (MafNode::MafMenuItemSprite*)sender;
                auto labelTime = (Label*)sender->getChildByName("WIDGET_TIME");
                
                int nTime = (int)(nItemTimeEnd - UtilsDate::getInstance()->getTime());
                if ( nTime <= 0 )
                {
                    itemReceive->stopAllActions();
                    itemReceive->setEnabled(false);
                    return;
                }
                
                int nDay = nTime / 86400;
                if ( nDay < 0 )
                {
                    nDay = 0;
                }
                
                labelTime->setString(GAME_TEXTFORMAT("t_ui_menu_present_timeleft", nDay));
                labelTime->setOverflow(Label::Overflow::SHRINK);
            });
            auto action2 = DelayTime::create(1.0);
            auto seq = Sequence::create(action1, action2, nullptr);
            
            auto labelTime = Label::createWithTTF("", GAME_FONT, 18);
            labelTime->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            labelTime->setPosition(itemReceive->getContentSize().width / 2, 10);
            labelTime->setDimensions(itemReceive->getContentSize().width - 10, labelTime->getContentSize().height);
            labelTime->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
            labelTime->setOverflow(Label::Overflow::SHRINK);
            labelTime->setName("WIDGET_TIME");
            itemReceive->addChild(labelTime);
            itemReceive->runAction(RepeatForever::create(seq));
        }
    }
    
    
    return cell;
}

ssize_t MailPopup::numberOfCellsInTableView(extension::TableView *table)
{
    return _listMail.size();
}

#pragma mark - ui
void MailPopup::uiAds()
{
    auto layout = _spriteContainer;
    auto size = layout->getContentSize();
    
    if ( _strAdsPath.length() != 0 )
    {
        auto uiAds = ui::Button::create(_strAdsPath);
        uiAds->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        uiAds->setPosition(Vec2(size.width / 2, 0));
        uiAds->addClickEventListener(CC_CALLBACK_1(MailPopup::onClickAds, this));
        layout->addChild(uiAds);
    }
    else
    {
        //
        if( MafRemoteConfig::GetRemoteValueBool("enable_banner") )
        {
            if(!CashManager::getInstance()->isAdsfreeTime())
                MafGoogleRewardAds::ShowGoogleBanner();
        }
    }
}

void MailPopup::uiMail()
{
    if ( _layerContainerMail == nullptr )
    {
        float heightTitle = 75;
        float height = _spriteContainer->getContentSize().height - heightTitle;
        
        _layerContainerMail = Layer::create();
        _layerContainerMail->setIgnoreAnchorPointForPosition(false);
        _layerContainerMail->setContentSize( Size(_spriteContainer->getContentSize().width - 2, height) );
        _layerContainerMail->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
        _layerContainerMail->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - heightTitle);
        _spriteContainer->addChild(_layerContainerMail);
    }
    _layerContainerMail->removeAllChildren();
    
    _table = extension::TableView::create(this, Size(_layerContainerMail->getContentSize().width - 20, _layerContainerMail->getContentSize().height - 100));
    _table->setIgnoreAnchorPointForPosition(false);
    _table->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _table->setPosition( Vec2(_layerContainerMail->getContentSize().width / 2, _layerContainerMail->getContentSize().height) );
    _table->setDirection(extension::TableView::Direction::VERTICAL);
    _table->setVerticalFillOrder(extension::TableView::VerticalFillOrder::TOP_DOWN);
    _table->setDelegate(this);
    _table->setBounceable(false);
    _layerContainerMail->addChild(_table);
    
    _labelListNothing = Label::createWithTTF(GAME_TEXT("t_ui_msg_nodata_gift"), GAME_FONT, 28);
    _labelListNothing->setPosition( Vec2(_layerContainerMail->getContentSize().width / 2, _layerContainerMail->getContentSize().height / 2) );
    _labelListNothing->setColor(Color3B::WHITE);
    _labelListNothing->setVisible(false);
    _layerContainerMail->addChild(_labelListNothing);
    
    
    auto menu = Menu::create();
    menu->setPosition(0, 0);
    _layerContainerMail->addChild(menu);
    
    auto spriteAll = ui::Scale9Sprite::create("Assets/ui_common/btn_1_2_on.png", Rect::ZERO, Rect(33,33,2,2));
    spriteAll->setContentSize(Size(_layerContainerMail->getContentSize().width - 20, 96));
    
    auto itemAll = MafNode::MafMenuItemSprite::create(spriteAll, nullptr, CC_CALLBACK_1(MailPopup::onClickMailReceviceAll, this));
    itemAll->setPosition(_layerContainerMail->getContentSize().width / 2, itemAll->getContentSize().height / 2 + 2);
    menu->addChild(itemAll);
    {
        auto label = Label::createWithTTF(GAME_TEXT("t_ui_attend_receive_all"), GAME_FONT, 24);
        label->setPosition( itemAll->getContentSize().width / 2, itemAll->getContentSize().height / 2);
        itemAll->addChild(label);
    }
}
#pragma mark - draw
void MailPopup::uiDrawList()
{
    auto listTemp = _listMail;
    
    for (auto iter = _listMail.rbegin(); iter != _listMail.rend(); ++iter)
    {
        auto obj = (InfoMail*)*iter;
        
        int nDiff = (int)(obj->getDeleteTime() - UtilsDate::getInstance()->getTime());
        if ( nDiff <= 0 )
        {
            _listMail.eraseObject(obj);
        }
    }
    
    //
    _table->reloadData();
    
    //
    if ( _listMail.size() == 0 )
    {
        UserInfoManager::getInstance()->setGiftHave(false);
        _labelListNothing->setVisible(true);
    }
    else
    {
        UserInfoManager::getInstance()->setGiftHave(true);
    }
    
    //
    RefreshManager::getInstance()->refreshUI(E_REFRESH::MAIL);
}


#pragma mark - click
void MailPopup::onClickClose(Ref* sender)
{
    hide();
}

void MailPopup::onClickAds(cocos2d::Ref* sender)
{
    MafNative::OpenUrl(_strAdsLink.c_str());
}

void MailPopup::onClickMailRecevice(cocos2d::Ref* sender)
{
    auto item = (MenuItem*)sender;
    int nIdx = item->getTag();
    
    //
    _listMailReceive.clear();
    
    auto obj = _listMail.at(nIdx);
    int nDiff = (int)(obj->getDeleteTime() - UtilsDate::getInstance()->getTime());
    if ( nDiff <= 0 )
    {
        return;
    }
    
    _listMailReceive.pushBack(obj);
    
    // network
    _nMailReceiveSuccess = 0;
    _nMailReceiveError = 0;
    _listReward.clear();
    requestMailReceive();
}

void MailPopup::onClickMailReceviceAll(cocos2d::Ref* sender)
{
    _listMailReceive.clear();
    for ( auto obj : _listMail )
    {
        int nDiff = (int)(obj->getDeleteTime() - UtilsDate::getInstance()->getTime());
        if ( nDiff <= 0 )
        {
            continue;
        }
        
        _listMailReceive.pushBack(obj);
    }
    
    // network
    _nMailReceiveSuccess = 0;
    _nMailReceiveError = 0;
    
    _listReward.clear();
    requestMailReceive();
}

#pragma mark - callback

#pragma mark - network
void MailPopup::requestMailList()
{
    if ( AccountManager::getInstance()->getUserID().length() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    //
    PopupLoading::show();
    
    std::string url = "/v1/gift/list/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string data1 = data;
        responseMailList(response,data1);
    };
    req->send(cb);
}

void MailPopup::responseMailList(network::HttpResponse* response,std::string &data)
{
    //
    PopupLoading::hide();
    
    bool bError = false;
    if ( response == nullptr || response->isSucceed() == false || response->getResponseCode() != 200 )
    {
        bError = true;
    }
       
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)data.c_str());
    if ( data.length() == 0 || jsonParser.HasParseError() == true )
    {
        bError = true;
    }
       
    if ( bError == false )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int idx = jsonValue["_giftidx"].GetInt();
            std::string title = jsonValue["_title"].GetString();
            int64_t deleteTime = jsonValue["_delete_date"].GetInt64();
            std::string items = jsonValue["_reward_list"].GetString();
        
            auto obj = InfoMail::create();
            obj->setIdx(idx);
            obj->setTitle(title);
            obj->setDeleteTime(deleteTime);
            obj->setItems(items);
            
            _listMail.pushBack(obj);
        }
    }
    
    //
    uiDrawList();
}

void MailPopup::requestMailReceive()
{
    if ( AccountManager::getInstance()->getUserID().length() <= 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    if ( _listMailReceive.size() <= 0 )
    {
        return;
    }

    //
    PopupLoading::show();
    
    auto obj = _listMailReceive.at(0);

    std::string url = "/v2/gift/receive/%s/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str(), obj->getIdx());

    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [=](void* http,network::HttpResponse* response,const char *data){
        std::string data1 = data;
        responseMailReceive(response,data1);
    };
    req->send(cb);
}

void MailPopup::responseMailReceive(network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_receive") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_2"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
 
    bool nResult = jsonParser["_receive"].GetBool();
    if ( nResult == true )
    {
        std::string strReward         = jsonParser["_reward"].GetString();
        std::string strItems         = jsonParser["_items"].GetString();
        ItemsManager::getInstance()->addItems(strReward, false);
        ItemsManager::getInstance()->setItemsServer(strItems);
        
        rapidjson::Document jsonParserReward;
        jsonParserReward.Parse<0>((char*)strReward.c_str());
        if ( jsonParserReward.HasParseError() == false )
        {
            for ( int i = 0; i < jsonParserReward.Size(); i++ )
            {
                const rapidjson::Value& jsonValue = jsonParserReward[i];
                
                int nIdx = jsonValue["idx"].GetInt();
                int nTypeParams = 0;
                if ( jsonValue.HasMember("type_params") == true )
                {
                    nTypeParams = jsonValue["type_params"].GetInt();
                }
                
                std::string strCount = "0";
                if ( jsonValue["count"].IsString() == true )
                {
                    strCount = jsonValue["count"].GetString();
                }
                else
                {
                    int64_t nCount = jsonValue["count"].GetInt64();
                    strCount = MafUtils::toString(nCount);
                }
                
                auto obj = ItemsManager::getInstance()->getInfoItems(nIdx);
                if ( obj != nullptr )
                {
                    obj->setCount(strCount);
                    obj->setTypeParams(nTypeParams);
    
                    _listReward.pushBack(obj);
                }
            }
            
            if ( jsonParserReward.Size() > 0 )
            {
                _nMailReceiveSuccess++;
                
                //
                int giftCount = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_GIFT_COUNT);
                SaveManager::setHistoryData(E_HISTORY_DATA::K_GIFT_COUNT, giftCount+1);
            }
            else
            {
                _nMailReceiveError++;
            }
        }
        else
        {
            _nMailReceiveError++;
        }
    }
    else
    {
        _nMailReceiveError++;
    }
    
    //
    auto obj = _listMailReceive.at(0);
    _listMailReceive.erase(0);
    _listMail.eraseObject(obj);
    
    if ( _listMailReceive.size() == 0 )
    {
        // 빈 텍스트 출력
        _labelListNothing->setVisible(false);
        if ( _listMail.size() == 0 )
        {
            _labelListNothing->setVisible(true);
            UserInfoManager::getInstance()->setGiftHave(false);
        }
        
        //
        RefreshManager::getInstance()->refreshUI(E_REFRESH::MAIL);
        

        // 새로고침
        if ( UserInfoManager::getInstance()->getCurrentPlace() == E_PLACE::DG_NORMAL )
        {
            HelloWorldScene::getInstance()->reDrawArtifact();
        }
        
        // 팝업출력
        if ( _nMailReceiveError > 0 && _nMailReceiveSuccess == 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_mail_msg_3"));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
        else
        {
            cocos2d::Vector<InfoItems*> listRewardTemp;
            for ( auto obj : _listReward )
            {
                bool bApply = false;
                for ( int i = 0; i < listRewardTemp.size(); i++ )
                {
                    auto objTemp = listRewardTemp.at(i);
                    
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
        
                        listRewardTemp.pushBack(objTemp);
                    }
                }
            }

            auto popup = MailReceivePopup::create(listRewardTemp, _nMailReceiveSuccess, _nMailReceiveError);
            popup->show();
        }
    }
    else
    {
        requestMailReceive();
    }
    
    
    //
    Vec2 pos = _table->getContentOffset();
    _table->reloadData();
    _table->setContentOffset(pos);
}
