//
//  PopupAttend.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_Mac on 06/11/2019.
//

#include "PopupAttend.h"

#include "GameObject/InfoItems.h"
#include "GameObject/InfoAttend.h"

#include "GameScene/HelloWorldScene.h"

#include "GameUIPopup/Other/Ads/PopupAdsMafGames.h"

#include "ManagerGame/UserInfoManager.h"
#include "ManagerGame/SaveManager.h"
#include "ManagerGame/AccountManager.h"
#include "ManagerGame/CostumeSkinManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"
#include "ManagerGame/SpecialMissionManager.h"
#include "ManagerGame/MineralManager.h"
#include "ManagerGame/CashManager.h"
#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

PopupAttend* PopupAttend::create()
{
    PopupAttend* pRet = new(std::nothrow) PopupAttend();
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

PopupAttend::PopupAttend(void) :
_result(0),
_totalDay(0),
_day(0),
_vip(0),

_layerContainerTop(nullptr),
_layerContainerContents(nullptr),
_layerContainerDetail(nullptr),
_layerContainerBonus(nullptr),

_labelVip(nullptr),
_labelDetail(nullptr)
{
    
}

PopupAttend::~PopupAttend(void)
{
    
}

bool PopupAttend::init()
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

void PopupAttend::onEnter()
{
    PopupBase::onEnter();
    
    //
    requestAttendData();
}

#pragma mark - init
void PopupAttend::initVar()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::ATTENDANCE_DAILY);
    
    rapidjson::Document jsonParser;
    jsonParser.ParseInsitu<0>((char*)strText.c_str());
    if( jsonParser.HasParseError() == false )
    {
        _listAttendAll.clear();
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int type = jsonValue["vip"].GetInt();
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

void PopupAttend::initUi()
{
    Size size = Size(658, 1218);

    //
    _spriteContainer->setContentSize( size );
    
    //
    _layerContainerTop = LayerColor::create();
    _layerContainerTop->setContentSize( Size(size.width - 50, 175) );
    _layerContainerTop->setIgnoreAnchorPointForPosition(false);
    _layerContainerTop->setAnchorPoint( Vec2::ANCHOR_MIDDLE_TOP );
    _layerContainerTop->setPosition(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 75 - 25);
    _spriteContainer->addChild(_layerContainerTop);
    
    _layerContainerBonus = LayerColor::create();
    _layerContainerBonus->setContentSize( Size(size.width - 50, 260) );
    _layerContainerBonus->setIgnoreAnchorPointForPosition(false);
    _layerContainerBonus->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerBonus->setPosition(_spriteContainer->getContentSize().width / 2, 25);
    _spriteContainer->addChild(_layerContainerBonus);
    
    _layerContainerDetail = LayerColor::create();
    _layerContainerDetail->setContentSize( Size(size.width - 50, 140) );
    _layerContainerDetail->setIgnoreAnchorPointForPosition(false);
    _layerContainerDetail->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerDetail->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerBonus->getPositionY() + _layerContainerBonus->getContentSize().height);
    _spriteContainer->addChild(_layerContainerDetail);
    
    _layerContainerContents = LayerColor::create();
    _layerContainerContents->setContentSize( Size(size.width - 50, size.height - 75 - 25 - _layerContainerTop->getContentSize().height - _layerContainerDetail->getContentSize().height - _layerContainerBonus->getContentSize().height - 25) );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM );
    _layerContainerContents->setPosition(_spriteContainer->getContentSize().width / 2, _layerContainerDetail->getPositionY() + _layerContainerDetail->getContentSize().height);
    _spriteContainer->addChild(_layerContainerContents);
    
    // title
    auto labelTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_attend"), GAME_FONT, 28);
    labelTitle->setColor(COLOR_TITLE);
    labelTitle->setPosition( Vec2(_spriteContainer->getContentSize().width / 2, _spriteContainer->getContentSize().height - 37.5) );
    _spriteContainer->addChild(labelTitle);
    
    // menu
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    // item close
    auto itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x.png"), nullptr, CC_CALLBACK_1(PopupAttend::onClickClose, this));
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    itemClose->setTouchRect( itemClose->getContentSize() * 2 );
    menu->addChild(itemClose);
    
    //
    uiTop();
    uiDetail();
    uiBonus();
}

#pragma mark - ui
void PopupAttend::uiTop()
{
    //
    _layerContainerTop->removeAllChildren();
    
    //
    _labelVip = Label::createWithTTF(MafUtils::format("VIP LV.%d", _vip), GAME_FONT, 30);
    _labelVip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _labelVip->setPosition(_layerContainerTop->getContentSize().width / 2, _layerContainerTop->getContentSize().height);
    _labelVip->setColor(Color3B(92, 203, 255));
    _labelVip->enableBold();
    _layerContainerTop->addChild(_labelVip);
    
    auto labelVipExplain = Label::createWithTTF(GAME_TEXT("t_ui_attend_msg_1"), GAME_FONT, 25);
    labelVipExplain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    labelVipExplain->setPosition(_layerContainerTop->getContentSize().width / 2, 25);
    labelVipExplain->setDimensions(_layerContainerTop->getContentSize().width * 0.9, labelVipExplain->getContentSize().height);
    labelVipExplain->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelVipExplain->setOverflow(Label::Overflow::SHRINK);
    _layerContainerTop->addChild(labelVipExplain);
}

void PopupAttend::uiContents()
{
    
}

void PopupAttend::uiDetail()
{
    //
    _layerContainerDetail->removeAllChildren();
    
    //
    auto spriteBG = ui::ImageView::create("Assets/ui_common/base_text_1_1.png");
    spriteBG->setScale9Enabled(true);
    spriteBG->setContentSize(_layerContainerDetail->getContentSize());
    spriteBG->setPosition(Vec2(_layerContainerDetail->getContentSize().width / 2, _layerContainerDetail->getContentSize().height / 2));
    _layerContainerDetail->addChild(spriteBG);
    
    _labelDetail = Label::createWithTTF("", GAME_FONT, 24);
    _labelDetail->setPosition(_layerContainerDetail->getContentSize().width / 2, _layerContainerDetail->getContentSize().height / 2);
    _labelDetail->setDimensions(_layerContainerDetail->getContentSize().width, _layerContainerDetail->getContentSize().height);
    _labelDetail->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    _labelDetail->setOverflow(Label::Overflow::SHRINK);
    _layerContainerDetail->addChild(_labelDetail);
}

void PopupAttend::uiBonus()
{
    //
    _layerContainerBonus->removeAllChildren();
    uiVipBonus();
    
    //
    std::string strAttend = MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_ATTEND_ADS));
    if( strAttend == "" )
    {
        strAttend = "1";
    }
    
    std::string strPath = "Assets/ui/attendance/daily_bonus_ad1_on.png";
    std::string name = "AD";
    if(strAttend != "1")
    {
        strPath = "Assets/ui/attendance/daily_bonus_ad1_off.png";
        name = "CLOSE";
    }
    
    auto btnBonus = ui::Button::create(strPath);
    btnBonus->addClickEventListener(CC_CALLBACK_1(PopupAttend::onClickBonus, this));
    btnBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    btnBonus->setPosition(Vec2(_layerContainerBonus->getContentSize().width / 2, 30));
    btnBonus->setName(name);
    _layerContainerBonus->addChild(btnBonus);
    
    
    if ( strAttend.compare("1") == 0 )
    {
        auto animation = Animation::create();
        animation->setDelayPerUnit(0.08f);
        for ( int i = 1; i <= 12; i++ )
        {
            animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/attendance/daily_bonus_ad1_%d.png", i));
        }

        auto animate = Animate::create(animation);
        btnBonus->getRendererNormal()->runAction(RepeatForever::create(animate));
    }
    else
    {
        
        auto lbText = Label::createWithTTF(GAME_TEXT("t_ui_confirm"), GAME_FONT, 32);
        lbText->setPosition(btnBonus->getContentSize().width / 2, btnBonus->getContentSize().height / 2);
        lbText->enableOutline(Color4B::BLACK, 2);
        btnBonus->addChild(lbText);
    }
}
void PopupAttend::uiVipBonus()
{
    bool hasSkin = CostumeSkinManager::getInstance()->isCostumeSkinBuy(5);
    //
    auto sprVipBonusBox = ui::Scale9Sprite::create("Assets/ui/tower/tower_bottom_btn.png",Rect::ZERO, Rect(11,11,22,22));
    sprVipBonusBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    sprVipBonusBox->setContentSize(Size(596,92));
    sprVipBonusBox->setPosition(_layerContainerBonus->getContentSize().width/2, _layerContainerBonus->getContentSize().height - 5);
    _layerContainerBonus->addChild(sprVipBonusBox);
    
    auto sprIconBox = Sprite::create("Assets/ui/main/main_hero_edge.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    sprIconBox->setPosition(15, sprVipBonusBox->getContentSize().height/2);
    sprVipBonusBox->addChild(sprIconBox);
    {
        auto sprHeroIcon = Sprite::create(MafUtils::format("Assets/icon_hero/skin_5.png"));
        sprHeroIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprHeroIcon->setPosition(sprIconBox->getContentSize()/2);
        sprHeroIcon->setScale(sprIconBox->getContentSize().width / sprHeroIcon->getContentSize().width);
        sprIconBox->addChild(sprHeroIcon);
        
        if(hasSkin)
        {
            sprHeroIcon->setColor(Color3B(100,100,100));
            auto sprCheck = Sprite::create(MafUtils::format("Assets/ui/return_event/return_reward_check.png"));
            sprCheck->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            sprCheck->setPosition(sprIconBox->getContentSize()/2);
            sprIconBox->addChild(sprCheck);
        }
    }
    auto lbText = Label::createWithTTF(GAME_TEXT("t_costume_skin_contents_5"), GAME_FONT, 24);
    lbText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbText->setAlignment(cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::CENTER);
    lbText->setDimensions(sprVipBonusBox->getContentSize().width - sprIconBox->getContentSize().width - 200, sprVipBonusBox->getContentSize().height - 5);
    lbText->setPosition(sprIconBox->getPosition() + Vec2::RIGHT * ( sprIconBox->getContentSize().width + 10));
    sprVipBonusBox->addChild(lbText);
    
    std::string strBtnText = GAME_TEXT("t_ui_prison_jelly_reward_bt");
    if(hasSkin)
        strBtnText = GAME_TEXT("t_ui_complete");
    
    auto btnVipBonus = ui::Button::create("Assets/ui_common/tower_btn.png");
    btnVipBonus->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btnVipBonus->setPosition(Vec2(sprVipBonusBox->getContentSize().width - 15, sprVipBonusBox->getContentSize().height/2));
    btnVipBonus->addClickEventListener(CC_CALLBACK_1(PopupAttend::onClickVip10Bonus, this));
    btnVipBonus->setTitleText(strBtnText);
    btnVipBonus->setTitleAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
    btnVipBonus->getTitleLabel()->setDimensions(btnVipBonus->getContentSize().width - 10, btnVipBonus->getContentSize().height - 10);
    btnVipBonus->setTitleFontSize(24);
    btnVipBonus->getTitleLabel()->setOverflow(cocos2d::Label::Overflow::SHRINK);
    sprVipBonusBox->addChild(btnVipBonus);
    
    if(_vip >= 10)
        btnVipBonus->setEnabled(!hasSkin);
    else
        btnVipBonus->setEnabled(false);
}

#pragma mark - draw
void PopupAttend::drawTop()
{
    _labelVip->setString(MafUtils::format("VIP LV.%d", _vip));
}

void PopupAttend::drawList()
{
    //
    auto layout = _layerContainerContents;
    
    
    //
    for ( int i = 0; i < _listAttend.size(); i++ )
    {
        auto objAttend = _listAttend.at(i);
        auto objItem = objAttend->getReward();
        
        int nDay = objAttend->getDay();
        
        int nCol = i % 5;
        int nRow = i / 5;
        int posX = nCol * 120 + nCol * 1.6;
        int posY = _layerContainerContents->getContentSize().height - nRow * 116 - nRow * 5;
        
        //
        auto uiBoxBG = ui::ImageView::create("Assets/ui/attendance/daily_box.png");
        uiBoxBG->setAnchorPoint(Vec2(0, 1));
        uiBoxBG->setPosition(Vec2(posX, posY));
        layout->addChild(uiBoxBG);
        
        auto uiTextDay = Label::createWithTTF(MafUtils::toString(objAttend->getDay()), GAME_FONT, 20);
        uiTextDay->setPosition(Vec2(15, uiBoxBG->getContentSize().height - 15));
        uiTextDay->setColor(Color3B(92, 203, 255));
        uiBoxBG->addChild(uiTextDay);
        
        if ( objItem != nullptr )
        {
            auto uiItemIcon = ui::ImageView::create(objItem->getPath());
            uiItemIcon->setPosition(Vec2(uiBoxBG->getContentSize().width / 2, uiBoxBG->getContentSize().height / 2 + 10));
            uiItemIcon->setScale(1.4);
            uiBoxBG->addChild(uiItemIcon);
            
            auto uiItemCount = ui::CText::create(objItem->getCount(), GAME_FONT, 24);
            uiItemCount->setPosition(Vec2(uiBoxBG->getContentSize().width / 2, uiBoxBG->getContentSize().height / 6));
            uiBoxBG->addChild(uiItemCount);
            if ( objItem->getIdx() == (int)E_ITEMS::COSTUME_COIN )
            {
                uiItemCount->setColor(COLOR_COSTUME_COIN);
            }
            else if ( objItem->getIdx() == (int)E_ITEMS::GEM )
            {
                uiItemCount->setColor(COLOR_GEM);
            }
            else if ( objItem->getIdx() == (int)E_ITEMS::MATERIAL )
            {
                uiItemCount->setColor(COLOR_MATERIAL);
            }
        }
        
        //
        if ( nDay < _day )
        {
            auto uiBlack = ui::ImageView::create("Assets/ui/attendance/daily_box.png");
            uiBlack->setPosition(Vec2(uiBoxBG->getContentSize().width / 2, uiBoxBG->getContentSize().height / 2));
            uiBlack->setColor(Color3B::BLACK);
            uiBlack->setOpacity(150);
            uiBoxBG->addChild(uiBlack);
        }
        else if ( nDay == _day ) // effect
        {
            Vec2 posEffect = Vec2(posX + uiBoxBG->getContentSize().width / 2, posY - uiBoxBG->getContentSize().height / 2);
            
            auto spriteEffect = Sprite::create("Assets/ui/attendance/ef_stamp_6.png");
            spriteEffect->setPosition(posEffect);
            layout->addChild(spriteEffect, 10);
            
            if ( _result == 1 )
            {
                auto animation = Animation::create();
                animation->setDelayPerUnit(0.15f);
                for ( int i = 1; i <= 6; i++ )
                {
                    animation->addSpriteFrameWithFile(MafUtils::format("Assets/ui/attendance/ef_stamp_%d.png", i));
                }
                spriteEffect->setTexture("Assets/ui/attendance/ef_stamp_1.png");
                
                auto action1 = Animate::create(animation);
                auto action2 = CallFunc::create([=](){
                    if ( _listReward.size() != 0 )
                    {
                        PopupToastItems::showAttend(_listReward);
                    }
                });
                auto seq = Sequence::create(action1, action2, NULL);
                spriteEffect->runAction(seq);
            }
        }
    }
}

void PopupAttend::drawDetail()
{
    if ( _listReward.size() == 0 )
    {
        std::string strText = GAME_TEXT("t_ui_attend_msg_3");
        
        std::string strAttend = MafAes256::Decrypt(UserDefault::getInstance()->getStringForKey(KEY_ATTEND_ADS));
        if ( strAttend.length() == 0 || strAttend.compare("1") == 0 )
        {
            strText.append("\n");
            strText.append(GAME_TEXT("t_ui_attend_msg_7"));
        }
        
        _labelDetail->setString(strText);
    }
    else
    {
        auto objAttend = _listAttend.at(_day - 1);
        auto objAttendItem = objAttend->getReward();
        
        auto objItem = _listReward.at(0);
        
        //
        std::string strText = GAME_TEXTFORMAT("t_ui_attend_msg_2", objItem->getText().c_str(), std::atoi(objItem->getCount().c_str()));
        _labelDetail->setString(strText);
        
        //
        if ( objAttendItem->getType().compare("COSTUME") == 0 && objItem->getType().compare("COSTUME") != 0 )
        {
            auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXTFORMAT("t_ui_attend_msg_6",  objAttendItem->getText().c_str(), std::atoi(objItem->getCount().c_str())));
            popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
            popup->show();
        }
    }
}

#pragma mark - set, get, other
void PopupAttend::setLoad(int vip)
{
    _listAttend.clear();
    
    for ( auto& obj : _listAttendAll )
    {
        if ( obj->getType() == vip )
        {
            _listAttend.pushBack(obj);
        }
    }
}

void PopupAttend::setResetDaliy()
{
    // 업적
    MafGooglePlay::IncrementAchievement(ACHIEVE_ATTEND_20, 1, 20);
        
    //광물초기화
    MineralManager::getInstance()->onBuyInit();
    
    // 만능 포인트 보관함 초기화
    UserInfoManager::getInstance()->setArtifactPointGain(E_PLACE::DG_NORMAL, 0);
    
    // 피버 회수 초기화
    UserInfoManager::getInstance()->setFeverDay(C_MAX_FEVER_DAY);
    
    // 큐브 회수 초기화
    UserInfoManager::getInstance()->setCubeDay(C_MAX_CUBE_DAY);
    
    //출석체크 후 광고 시청 기능 초기화
    UserDefault::getInstance()->setStringForKey(KEY_ATTEND_ADS, MafAes256::Encrypt("1"));
    
    // 보물 1000원, 3000원의 행복 초기화
    for(int i=E_ARTIFACT::ARTI_1_EVOL1; i< E_ARTIFACT::E_ARTIFACT_999_NONE; i++)
    {
        std::string str = "";
        str = MafUtils::format("pack_%d",i).c_str();
        UserDefault::getInstance()->setIntegerForKey(str.c_str(), 0);
        
        str = MafUtils::format("pack_e_%d",i).c_str();
        UserDefault::getInstance()->setIntegerForKey(str.c_str(), 0);
        
        str = MafUtils::format("pack_w_%d",i).c_str();
        UserDefault::getInstance()->setIntegerForKey(str.c_str(), 0);
    }
    
    for(int i = 1; i <= InfiniteArtifactManager::getInstance()->getMaxGrade(); i++)
    {
        std::string str = "";
        
        for(int j = 1; j <= INFINITE_ARTIFACT_KIND; ++j)
        {
            str = MafUtils::format("Infinite_pack_%d_%d",i, j).c_str();
            UserDefault::getInstance()->setIntegerForKey(str.c_str(), 0);
        }
    }
    
    //
    for(int i = 1; i <= C_MAX_MYHOME; i++ )
    {
        int level = UserInfoManager::getInstance()->getMyhomeLevel(i);
        if ( level >= 10 )
        {
            UserInfoManager::getInstance()->setMyhomeWatchcount(i, 0);
        }
    }
    
    if( UserInfoManager::getInstance()->getCurrentPlace() == DG_NORMAL )
    {
        auto scene = HelloWorldScene::getInstance();
        scene->reDrawArtifact();
    }
    else if( UserInfoManager::getInstance()->getCurrentPlace() == DG_DEVIL_DOM )
    {
        
    }

    // analytics
    int nTotalDay = SaveManager::getHistoryDataInt(E_HISTORY_DATA::K_VIP_DAY);
    if ( nTotalDay >= 1 && nTotalDay <= 30 )
    {
        std::string strEventName = MafUtils::format(kAnalEventAttendDay, nTotalDay);
        MafAnalyticsManager::LogEvent(strEventName.c_str(), kRepeatTrue);
    }
    
    //아이템 피버
    UserInfoManager::getInstance()->setItemFeverCount(0);
}
 
#pragma mark - click
void PopupAttend::onClickClose(cocos2d::Ref* sender)
{
    hide();
}

void PopupAttend::onClickBonus(cocos2d::Ref *sender)
{
    std::string name = ((MenuItem*)sender)->getName();
    
    if (name == "CLOSE")
    {
        onClickClose(sender);
    }
    else if (name == "AD")
    {
        // ads
        if( CashManager::getInstance()->isAdsfreeTime() == true )
        {
            callbackBonus(ADS_RESULT::SUCCESS);
        }
        else
        {
            MafGoogleRewardAds::ShowGoogleRewardedVideo(this, CC_CALLBACK_1(PopupAttend::callbackBonus, this));
        }
    }
}
void PopupAttend::onClickVip10Bonus(cocos2d::Ref *sender)
{
    PopupLoading::show();
    CostumeSkinManager::getInstance()->requestSkinBuy(5, CC_CALLBACK_3(PopupAttend::callbackVIPBonus, this));
}
#pragma mark - network
void PopupAttend::requestAttendData()
{
    if ( AccountManager::getInstance()->getUserID().length() == 0 )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_1"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    // network
    PopupLoading::show();
    
    std::string url = "/v4/daily/%s";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserID().c_str());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseAttendData(response,dd);
    };
    req->send(cb);
}

void PopupAttend::responseAttendData(network::HttpResponse *response, std::string &data)
{
    //
    PopupLoading::hide();
    
    //
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
       
    if ( bError == true )
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_30"));
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, [=](Ref* sender){
            hide();
        });
        popup->show();
        return;
    }
    
    _result = jsonParser["_result"].GetInt();
    _totalDay = jsonParser["_totalday"].GetInt();
    _vip = jsonParser["_vip"].GetInt();
    _day = jsonParser["_day"].GetInt();
     
    std::string strReward = jsonParser["_reward"].GetString();
    std::string strItems = jsonParser["_items"].GetString();
    
    _listReward.clear();
    _listReward = ItemsManager::getInstance()->getConvertArray(strReward);
    ItemsManager::getInstance()->addItems(strReward, false);
    ItemsManager::getInstance()->setItemsServer(strItems);
    
    //
    SaveManager::setHistoryData(E_HISTORY_DATA::K_VIP_DAY, _totalDay);
    
    //
    if(_vip >= 1)
        SpecialMissionManager::getInstance()->clearSpecialMission(165);
    
    //
    setLoad(_vip);
    
    //
    std::string strAttendDayPrev = UserDefault::getInstance()->getStringForKey(KEY_ATTEND_DAY, "");
    if ( strAttendDayPrev.length() != 0 )
    {
        strAttendDayPrev = MafAes256::Decrypt(strAttendDayPrev);
    }
    
    if ( strAttendDayPrev.compare(MafUtils::toString(_day)) != 0 )
    {
        setResetDaliy();
    }
    
    //
    UserInfoManager::getInstance()->setAttendHave(false);
    
    //
    UserInfoManager::getInstance()->setAttendVip(_vip);
    UserInfoManager::getInstance()->setAttendDay(_day);
    SpecialMissionManager::getInstance()->clearSpecialMission(85);
    
    // draw
    drawTop();
    drawList();
    drawDetail();
    
    uiBonus();
}

#pragma mark - callback
void PopupAttend::callbackBonus(ADS_RESULT result)
{
    if ( result == ADS_RESULT::SUCCESS )
    {
        //
        MafRewardAdsManager::getInstance()->setHistoryData(kAdsSlotDailyAttend, CashManager::getInstance()->isAdsfreeTime());
        
        //
        ItemsMoneyManager::getInstance()->setGem(100);
        
        //이펙트
        cocos2d::Vector<InfoItems*> listReward;
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        obj->setCount(100);
        listReward.pushBack(obj);
        
        //
        PopupToastItems::show(listReward);
        
        //저장
        UserDefault::getInstance()->setStringForKey(KEY_ATTEND_ADS, MafAes256::Encrypt("0"));
        
        //
        hide();
    }
    else
    {
        if ( result == ADS_RESULT::LOAD )
        {
            auto popup = PopupAdsMafGames::create();
            popup->setCallback(CC_CALLBACK_1(PopupAttend::callbackBonus, this));
            popup->setLogParams(kAnalEventAdsLoadParameterSlot);
            popup->setLogValue(kAdsSlotDailyAttend);
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

void PopupAttend::callbackVIPBonus(bool bResult, int nResult, int nSkin)
{
    //
    PopupLoading::hide();
    
    if ( bResult == false )
    {
        std::string strError = GAME_TEXT("t_ui_error_30");
        if ( nResult == 0 )
        {
            strError = GAME_TEXT("t_ui_costume_skin_msg_3");
        }
        
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), strError);
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
        return;
    }
    
    
    auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_vip10_reward"));
    popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
    popup->show();
    
 
    uiBonus();
}
