//
//  PopupStory.cpp
//  FantasyClicker-mobile
//
//  Created by MAF Games on 2018. 7. 31..
//

#include "PopupStory.h"

#include "ManagerGame/StoryManager.h"
#include "ManagerGame/PetNewManager.h"
#include "ManagerGame/CostumeManager.h"

USING_NS_CC;

PopupStory* PopupStory::create(int nIdx)
{
    PopupStory* pRet = new(std::nothrow) PopupStory();
    if ( pRet && pRet->init(nIdx) )
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

PopupStory::PopupStory(void) :
_nIdx(0),
_nPos(0),
_callbackFinsh(nullptr),

_layerContainer(nullptr)
{
    
}

PopupStory::~PopupStory(void)
{
    
}

bool PopupStory::init(int nIdx)
{
    if ( !PopupBase::init(PopupBaseType::TRANSPARENT) )
    {
        return false;
    }
    
    //
    setName("POPUP_STORY");
    
    //
    _nIdx = nIdx;
    
    // init
    initVar();
    initUi();
    
    return true;
}

#pragma mark - override
void PopupStory::show()
{
    auto scene = UtilsScene::getInstance()->getRunningScene();
    if ( scene == nullptr )
    {
        return;
    }
    
    if ( scene != nullptr )
    {
        scene->addChild(this, ZORDER_SCENE_POPUP_STORY, TAG_SCENE_POPUP);
    }
    
    setOpacity(255);
}
#pragma mark - init
void PopupStory::initVar()
{
    auto obj = StoryManager::getInstance()->getStory(_nIdx);
    _listStoryImage = obj->getFileNameImage();
    _listStoryText = obj->getFileNameText();
}

void PopupStory::initUi()
{
    //
    _layerContainer = Layer::create();
    _layerContainer->setContentSize( Size(getContentSize().width, getContentSize().height));
    _layerContainer->setIgnoreAnchorPointForPosition(false);
    _layerContainer->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    _layerContainer->setPosition( getContentSize().width / 2, getContentSize().height / 2 );
    addChild(_layerContainer);
    
    //
    uiContents();
}


#pragma mark - ui
void PopupStory::uiContents()
{
    //
    _layerContainer->removeAllChildren();
    
    //
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _layerContainer->addChild(menu);
    
    auto layer = Layer::create();
    layer->setContentSize(Size(_layerContainer->getContentSize().width, _layerContainer->getContentSize().height));
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto itemStory = MafNode::MafMenuItemSprite::create(layer, nullptr, CC_CALLBACK_1(PopupStory::onClickStory, this));
    itemStory->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2);
    itemStory->setTag(1);
    menu->addChild(itemStory);
    
    // story image
    std::string strPathImage = _listStoryImage.at(_nPos);
    std::string strPathText = _listStoryText.at(_nPos);
    
    _spriteStoryImage = Sprite::create(strPathImage);
    _spriteStoryImage->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2);
    _spriteStoryImage->setFlippedX(false);
    _layerContainer->addChild(_spriteStoryImage);
    
    _spriteStoryText = Sprite::create(strPathText);
    _spriteStoryText->setPosition(_layerContainer->getContentSize().width / 2, _layerContainer->getContentSize().height / 2);
    _layerContainer->addChild(_spriteStoryText);
    
    if ( TextManager::getInstance()->getLang().compare("jp") == 0 && strPathText.find("Assets/story/text_jp/") == 0 )
    {
        _spriteStoryImage->setFlippedX(true);
    }
    if ( TextManager::getInstance()->getLang().compare("zh_tw") == 0 && strPathText.find("Assets/story/text_zh_tw/") == 0 )
    {
        _spriteStoryImage->setFlippedX(true);
    }
    
    // next image
    auto spriteNext = Sprite::create("Assets/ui/story/btn_arrows_next.png");
    spriteNext->setPosition(_layerContainer->getContentSize().width - 20 - spriteNext->getContentSize().width / 2, 70);
    _layerContainer->addChild(spriteNext);
    
    auto spritePrev = Sprite::create("Assets/ui/story/btn_arrows_back.png");
    auto btnPrev = MafNode::MafMenuItemSprite::create(spritePrev, nullptr, CC_CALLBACK_1(PopupStory::onClickStory,this));
    btnPrev->setAnchorPoint(Vec2(0.5,0.5));
    btnPrev->setTag(-1);
    btnPrev->setPosition(Vec2(60, 70));
    menu->addChild(btnPrev);
    _layerContainer->reorderChild(menu, 20);

    //
    drawContents();
}

void PopupStory::drawContents()
{
    std::string strPathImage = _listStoryImage.at(_nPos);
    std::string strPathText = _listStoryText.at(_nPos);
    
    _spriteStoryImage->setTexture(strPathImage);
    _spriteStoryText->setTexture(strPathText);
    
    if ( _nIdx == (int)E_COMIC::DEVIL_10000 && strPathText.find("d_10_7.png") != std::string::npos)
    {
        _spriteStoryText->setOpacity(0);
        _spriteStoryText->runAction(FadeIn::create(2.5));
        
        auto obj = StoryManager::getInstance()->getStory(_nIdx);
        std::string strLang = obj->getFileLanguage();
        std::string strPathTemp = std::string("Assets/story/text_%s/").append("d_10_7_1.png");
        strPathTemp = MafUtils::format(strPathTemp.c_str(), strLang.c_str());
        
        auto spriteTemp = Sprite::create(strPathTemp);
        spriteTemp->setPosition(_spriteStoryText->getContentSize().width / 2, _spriteStoryText->getContentSize().height / 2);
        spriteTemp->setOpacity(0);
        _spriteStoryText->addChild(spriteTemp);
        
        auto action1 = DelayTime::create(2.5);
        auto action2 = FadeIn::create(2.5);
        auto seq = Sequence::create(action1, action2, nullptr);
        spriteTemp->runAction(seq);
    }
    else if ( _nIdx == (int)E_COMIC::EVENT_HALLOWEEN )
    {
        int nShowCount = 0;
        for ( int i = 1; i <= C_COUNT_PET; i++ )
        {
            if ( PetNewManager::getInstance()->isPetSkinBuy((E_PET)i, 2) == false )
                continue;

            std::string strPathTemp = std::string("Assets/story/image/event_2_1_%d.png");
            strPathTemp = MafUtils::format(strPathTemp.c_str(), i);
            
            auto spriteTemp = Sprite::create(strPathTemp);
            spriteTemp->setPosition(_spriteStoryText->getContentSize().width / 2, _spriteStoryText->getContentSize().height / 2);
            spriteTemp->setOpacity(0);
            _spriteStoryText->addChild(spriteTemp, 2);
            if ( i == E_PET::HANDSOME_8 )
            {
                spriteTemp->setLocalZOrder(1);
            }
            
            nShowCount++;
            auto action1 = DelayTime::create(0.5 * nShowCount);
            auto action2 = FadeIn::create(0.5);
            auto seq = Sequence::create(action1, action2, nullptr);
            spriteTemp->runAction(seq);
        }

    }
}

#pragma mark - set, get, other
void PopupStory::setCallbackFinish(const std::function<void(void)>& callback)
{
    _callbackFinsh = callback;
}

void PopupStory::setStoryPage(int movePage)
{
    _nPos += movePage;
    if(_nPos < 0)
    {
        _nPos = 0;
        return;
    }
    
    auto obj = StoryManager::getInstance()->getStory(_nIdx);
    int nSize = obj->getFileCount();
    
    if ( _nIdx == (int)E_COMIC::EVENT_SUMMER )
    {
        for ( int i = 1; i <= C_COUNT_PET; i++ )
        {
            if ( PetNewManager::getInstance()->isPetSkinBuy((E_PET)i, 1) == false )
            {
                nSize--;
                break;
            }
        }
    }
    if (_nIdx == (int)E_COMIC::BEELZEBUB)
    {
        if( CostumeManager::getInstance()->isExist(E_COSTUME::JEWEL, E_COSTUME_IDX::IDX_18)  == false)
        {
            nSize -= 2;
        }
    }
    if (_nIdx == (int)E_COMIC::EVENT_CHRISTMAS)
    {
        nSize--;
        if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::CHICK_1, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::HAMSTER_2, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::DRAGON_3, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::PRINCESS_4, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::PURIRING_5, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::HONG_6, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::MOLE_7, 3) == false){}
        else if( PetNewManager::getInstance()->isPetSkinBuy(E_PET::HANDSOME_8, 3) == false){}
        else
        {
            nSize++;
        }
    }
    
    if( _nIdx == (int)E_COMIC::TUTORIAL)
    {
        std::string logContent = MafUtils::format(kAnalEventStoryTuto, _nPos);
        MafAnalyticsManager::LogEvent(logContent.c_str(), kRepeatFalse);
    }
    
    if ( _nPos >= nSize )
    {
        if ( _callbackFinsh != nullptr )
        {
            _callbackFinsh();
        }
        
        hide();
        return;
    }
    
    
    drawContents();
}

#pragma mark - click
void PopupStory::onClickStory(cocos2d::Ref* sender)
{
    auto item = (MafNode::MafMenuItem*)sender;
    auto tag = item->getTag();
    setStoryPage(tag);
}
