//
//  PopupEventNewYear2020.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupEventNewYear2020.h"

#include "Common/ConfigGameNormal.h"

#include "GameUIPopup/Other/Ads/PopupAdsWatch.h"
#include "GameUIPopup/Other/Ads/PopupAdsWatchDouble.h"

#include "ManagerGame/AccountManager.h"
#include "ManagerGame/ItemsManager.h"
#include "ManagerGame/ItemsMoneyManager.h"

USING_NS_CC;

PopupEventNewYear2020* PopupEventNewYear2020::create()
{
    PopupEventNewYear2020* pRet = new(std::nothrow) PopupEventNewYear2020();
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

PopupEventNewYear2020::PopupEventNewYear2020(void) :
_bReward(false),

_layerContainerContents(nullptr),
_spriteReward(nullptr)
{
    
}

PopupEventNewYear2020::~PopupEventNewYear2020(void)
{
    
}

bool PopupEventNewYear2020::init()
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    // touch
    _eventDispatcher->removeEventListenersForTarget(this);
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](cocos2d::Touch *touch, cocos2d::Event *event)->bool {
        cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
        if (rect.containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            return true;
        }
        
        return false;
    };
    listener->onTouchEnded = [=](cocos2d::Touch *touch, cocos2d::Event *event)->void {
        hide();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // init
    initVar();
    initUi();

    
    return true;
}

void PopupEventNewYear2020::onEnter()
{
    PopupBase::onEnter();
    
    // network
    requestInfo();
}

#pragma mark - init
void PopupEventNewYear2020::initVar()
{
    
}

void PopupEventNewYear2020::initUi()
{
    //
    auto spriteBG = Sprite::create("Assets/ui/event/newmail_1.png");
    spriteBG->setPosition(getContentSize().width / 2, getContentSize().height / 2);
    addChild(spriteBG);
    
    //
    _layerContainerContents = Layer::create();
    _layerContainerContents->setContentSize( spriteBG->getContentSize() );
    _layerContainerContents->setIgnoreAnchorPointForPosition(false);
    _layerContainerContents->setPosition(spriteBG->getContentSize().width / 2, spriteBG->getContentSize().height / 2);
    spriteBG->addChild(_layerContainerContents);
    
    //
    uiContents();
}

#pragma mark - ui
void PopupEventNewYear2020::uiContents()
{
    //
    _layerContainerContents->removeAllChildren();
    
    //
    _spriteReward = Sprite::create("Assets/ui/event/newmail_2.png");
    _spriteReward->setPosition(_layerContainerContents->getContentSize().width / 2, 200);
    _spriteReward->setVisible(false);
    _layerContainerContents->addChild(_spriteReward);
}

#pragma mark - draw
void PopupEventNewYear2020::drawContents()
{
    _spriteReward->setVisible(_bReward);
}

#pragma mark - set, get, other

#pragma mark - click
void PopupEventNewYear2020::onClickClose(Ref* sender)
{
    hide();
}


#pragma mark - network
void PopupEventNewYear2020::requestInfo()
{
    PopupLoading::show();
    
    //
    std::string url = "/event/card/reward/%d";
    url = MafUtils::format(url.c_str(), AccountManager::getInstance()->getUserIdx());
    
    auto req = new MafHttpNew(network::HttpRequest::Type::GET, url.c_str());
    MafResponseNew cb = [&](void* http,network::HttpResponse* response,const char *data){
        std::string dd = data;
        responseInfo(response,dd);
    };
    req->send(cb);
}

void PopupEventNewYear2020::responseInfo(cocos2d::network::HttpResponse* response,std::string &data)
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
    
    if ( bError == false && jsonParser.HasMember("_reward") == false )
    {
        bError = true;
    }
    
    if ( bError == true )
    {
        hide();
        return;
    }
    
    //
    _bReward = jsonParser["_reward"].GetBool();
    if ( _bReward == true )
    {
        int nCount = jsonParser["_count"].GetInt();
        ItemsMoneyManager::getInstance()->setGem(nCount);
        
        //
        cocos2d::Vector<InfoItems*> listReward;
        
        auto obj = ItemsManager::getInstance()->getInfoItems(E_ITEMS::GEM);
        obj->setCount(nCount);
        listReward.pushBack(obj);
        
        PopupToastItems::show(listReward);
    }
    
    //
    drawContents();
}

