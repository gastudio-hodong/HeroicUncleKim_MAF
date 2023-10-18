//
//  PopupAdsMafGames.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2021/04/01.
//

#include "PopupAdsMafGames.h"

#include "Common/ConfigGameNormal.h"

USING_NS_CC;

PopupAdsMafGames* PopupAdsMafGames::create()
{
    PopupAdsMafGames* pRet = new(std::nothrow) PopupAdsMafGames();
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

PopupAdsMafGames::PopupAdsMafGames()
:
_video(nullptr),
_itemClose(nullptr),
_lbTimer(nullptr),
_sprClose(nullptr),
_callbackAds(nullptr),
_timer(0),
_timerMax(30),
_eType(E_MAF_ADS_TYPE::NONE),
_strLogParams(""),
_strLogValue("1")
{
}

PopupAdsMafGames::~PopupAdsMafGames()
{
    
}

#pragma mark init
bool PopupAdsMafGames::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    if(PopupLoading::isShow())
        PopupLoading::hide();
    
    auto paramTimer = ParameterManager::getInstance()->getParameterDouble("ads_maf_max_timer");
    
    if(paramTimer > 0)
        _timerMax = paramTimer;
    
    loadMoreDataManager();
    
    setAdsType();
    initVar();
    initUI();
    _bHideBackKey = false;
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(PopupAdsMafGames::scheduleTimer), this, 1 , kRepeatForever, 1, false);
    scheduleUpdate();
    initSound();
    
    return true;
}

void PopupAdsMafGames::initVar()
{
    _spriteContainer->setContentSize(getContentSize());
    _spriteContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto menu = MafNode::MafMenu::create();
    menu->setPosition(Vec2::ZERO);
    menu->setAnchorPoint(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    
    std::string strBGPath = "";
    
    if(TextManager::getInstance()->getLang() == "ko")
    {
        switch (_eType)
        {
            case E_MAF_ADS_TYPE::GAMEMANG:
            {
                strBGPath = "Assets/MafAds/gamemang_ko.png";
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch (_eType)
        {
            case E_MAF_ADS_TYPE::GAMEMANG:
            {
                strBGPath = "Assets/MafAds/gamemang_en.png";
            }
                break;
                
            default:
                break;
        }
    }
    
    auto sprBG = ui::Scale9Sprite::create(strBGPath);
    sprBG->getSprite()->getTexture()->setTexParameters(texParams);
    sprBG->setScale(_spriteContainer->getContentSize().width/sprBG->getContentSize().width, _spriteContainer->getContentSize().height/sprBG->getContentSize().height);
    
    auto itemBGLink = MafNode::MafMenuItemSprite::create(sprBG, nullptr, CC_CALLBACK_1(PopupAdsMafGames::onClick, this));
    itemBGLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    itemBGLink->setPosition(_spriteContainer->getContentSize()/2);
    itemBGLink->setName("ADS_LINK");
    menu->addChild(itemBGLink);
}

void PopupAdsMafGames::initUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu, E_ZORER::UI+1);
    
    _itemClose = MafNode::MafMenuItemSprite::create(Sprite::create("Assets/ui_common/btn_x_3_1.png"),nullptr, CC_CALLBACK_1(PopupAdsMafGames::onClick, this));
    _itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width - _itemClose->getContentSize().width, _spriteContainer->getContentSize().height - 37.5) );
    _itemClose->setTouchRect( _itemClose->getContentSize() * 2);
    _itemClose->setName("CLOSE");
    menu->addChild(_itemClose);
    
    {
        _sprClose = Sprite::create("Assets/ui_common/btn_x_3_2.png");
        _sprClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _sprClose->setPosition(_itemClose->getContentSize()/2);
        _itemClose->addChild(_sprClose);
        _sprClose->setVisible(false);
        
        _lbTimer = Label::createWithTTF("", GAME_FONT, 22);
        _lbTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _lbTimer->setPosition(_itemClose->getContentSize()/2);
        _lbTimer->setDimensions(_itemClose->getContentSize().width, _itemClose->getContentSize().height);
        _lbTimer->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        _lbTimer->setOverflow(Label::Overflow::SHRINK);
        _itemClose->addChild(_lbTimer);
        
    }
    
    std::string strVideoPath = "";
    switch (_eType)
    {
        case E_MAF_ADS_TYPE::GAMEMANG:
        {
            strVideoPath = "Assets/MafAds/gamemang.mp4";
        }
            break;
            
        default:
            break;
    }
    
    _video = cocos2d::experimental::ui::VideoPlayer::create();
    _video->addEventListener(CC_CALLBACK_2(PopupAdsMafGames::videoEventCallback, this));
    _video->setContentSize( _spriteContainer->getContentSize() );
    _video->setPosition(_spriteContainer->getContentSize()/2);                // 동영상 위치
    _video->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _video->setKeepAspectRatioEnabled(false);            // 원본 종횡비 유지여부
    _video->setFileName(strVideoPath);
    _video->setFullScreenEnabled(true);                // true로 해야만 표시됨
    
    _video->play();
    _spriteContainer->addChild(_video);
}

void PopupAdsMafGames::initSound()
{
    /* sound 이슈
    _volumeBGM =  SoundManager::GetSoundBgmVol();
    _volumeEffect =  SoundManager::GetSoundEffectVol();
    
    SoundManager::SetSoundBgmVol(0);
    SoundManager::SetSoundEffectVol(0);
    */
}

#pragma mark
void PopupAdsMafGames::loadMoreDataManager()
{
    std::string strText = TableManager::getInstance()->getTableData(E_TABLE::GAME_LIST);

    rapidjson::Document jsonParser;
    jsonParser.Parse<0>((char*)strText.c_str());
    if ( strText.length() != 0 && jsonParser.HasParseError() == false  )
    {
        for ( int i = 0; i < jsonParser.Size(); i++ )
        {
            const rapidjson::Value& jsonValue = jsonParser[i];
            
            int nIdx = jsonValue["idx"].GetInt();
            std::string strName = jsonValue["name"].GetString();
            std::string strCountry = jsonValue["country"].GetString();
            std::string strCountryExcept = jsonValue["country_except"].GetString();
            std::string strPlatform = jsonValue["platform"].GetString();
            std::string strAdsVideoPath = jsonValue["ads_video_path"].GetString();
            std::string strFileName = jsonValue["file_name"].GetString();
            std::string strLink = jsonValue["link"].GetString();
            
            auto info = InfoMoreGame::create();
            info->setIdx(nIdx);
            info->setName(strName);
            info->setCountry(strCountry);
            info->setCountryExcept(strCountryExcept);
            info->setPlatform(strPlatform);
            info->setAdsVideoPath(strAdsVideoPath);
            info->setIconFileName(strFileName);
            info->setLink(strLink);
            
            _listMoreGame.pushBack(info);
            
        }
    }
}
void PopupAdsMafGames::hide()
{
    /* sound 이슈
    SoundManager::SetSoundBgmVol(_volumeBGM);
    SoundManager::SetSoundEffectVol(_volumeEffect);
     */
    
    //log
    MafAnalyticsManager::LogEvent(kAnalEventAdsLoad, _strLogParams.c_str(), _strLogValue.c_str(), kRepeatTrue);
    
    PopupBase::hide();
}

void PopupAdsMafGames::scheduleTimer(float dt)
{
    _timer += dt;
    
    _lbTimer->setString(std::to_string((int) (_timerMax - _timer)));
    
    if(_timer >= _timerMax)
    {
        _timer = _timerMax;
        if(_video->isVisible())
        {
            _video->stop();
            _video->setFullScreenEnabled(false);
            _video->setVisible(false);
        }
        _lbTimer->setVisible(false);
        _sprClose->setVisible(true);
        
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(PopupAdsMafGames::scheduleTimer), this);
    }
}

void PopupAdsMafGames::videoEventCallback(Ref* p, cocos2d::experimental::ui::VideoPlayer::EventType evt)
{
    switch (evt) {
        case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
        {
            _video->stop();
            _video->setFullScreenEnabled(false);
            _video->setVisible(false);
            //hide();
        }
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::ERROR:
        {
            _video->stop();
            _video->setFullScreenEnabled(false);
            _video->setVisible(false);
        }
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
        {
        }
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
        {
        }
            break;
        case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
        {
            _video->stop();
            _video->setFullScreenEnabled(false);
            _video->setVisible(false);
        }
            break;
    }

}

#pragma mark

void PopupAdsMafGames::setAdsType()
{
    _eType = E_MAF_ADS_TYPE::GAMEMANG;
}

void PopupAdsMafGames::onClick(Ref* sender)
{
    auto item = (Node*) sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        onClickClose();
    }
    else if(item->getName().compare("ADS_LINK") == 0)
    {
        switch (_eType)
        {
            case E_MAF_ADS_TYPE::GAMEMANG:
            {
                auto info = getMoreGameFromIdx((int)_eType);
                
                std::string strLink = "";
                
                if(info != nullptr)
                {
                    strLink = info->getLink();
                }
                
                if(!strLink.empty())
                    MafNative::OpenUrl(strLink.c_str());
            }
                break;
                
            default:
                break;
        }
        
        
    }
}

void PopupAdsMafGames::onClickClose()
{
    if(_timer < _timerMax)
    {
        auto popup = PopupDefault::create(GAME_TEXT("t_ui_notice"), GAME_TEXT("t_ui_error_14"));
        popup->addButton(GAME_TEXT("t_ui_off"), false, [=](Ref*)
        {
            hide();
        });
        popup->addButton(GAME_TEXT("t_ui_confirm"), true, nullptr);
        popup->show();
    }
    else
    {
        if(_callbackAds != nullptr)
            _callbackAds(ADS_RESULT::SUCCESS);
        
        hide();
    }
}

void PopupAdsMafGames::setCallback(const std::function<void(ADS_RESULT)>& pCallback)
{
    _callbackAds = pCallback;
}

void PopupAdsMafGames::setLogParams(std::string logParams)
{
    _strLogParams = logParams;
}

void PopupAdsMafGames::setLogValue(std::string logValue)
{
    _strLogValue = logValue;
}

InfoMoreGame* PopupAdsMafGames::getMoreGameFromIdx(int nIdx)
{
    InfoMoreGame* result = nullptr;
    for(auto info : _listMoreGame)
    {
        if(info->getIdx() == nIdx)
        {
            result = info;
            break;
        }
    }
    
    return result;
}
