//
//  PopupActivateInfiniteArtifact.cpp
//  FantasyClicker-mobile
//
//  Created by MAF_TG on 2020/10/28.
//

#include "PopupUpgradeInfiniteArtifact.h"

#include "ManagerGame/InfiniteArtifactManager.h"

USING_NS_CC;

#pragma mark - FirstOpen
PopupFirstUpgradeInfiniteArtifact* PopupFirstUpgradeInfiniteArtifact::create()
{
    PopupFirstUpgradeInfiniteArtifact* pRet = new(std::nothrow) PopupFirstUpgradeInfiniteArtifact();
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

PopupFirstUpgradeInfiniteArtifact::PopupFirstUpgradeInfiniteArtifact(void)
{
}

PopupFirstUpgradeInfiniteArtifact::~PopupFirstUpgradeInfiniteArtifact(void)
{
}

bool PopupFirstUpgradeInfiniteArtifact::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();
    

    return true;
}
void PopupFirstUpgradeInfiniteArtifact::initVar()
{
    double nWidth = 650;
    double nHeight = getContentSize().height;
    if ( nHeight > 850 )
        nHeight = 850;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
}

void PopupFirstUpgradeInfiniteArtifact::initUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_done_title"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    auto sprClose = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO);
    sprClose->setContentSize(Size(150, 100));
    
    auto itemClose = MafNode::MafMenuItemSprite::create(sprClose, nullptr, CC_CALLBACK_1(PopupFirstUpgradeInfiniteArtifact::onClickButton, this));
    
    itemClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width/2, 20));
    itemClose->setTouchRect( sprClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbClose = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 25);
    lbClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbClose->setHorizontalAlignment(TextHAlignment::CENTER);
    lbClose->setVerticalAlignment(TextVAlignment::CENTER);
    lbClose->setDimensions(itemClose->getContentSize().width-10, itemClose->getContentSize().height-5);
    lbClose->setOverflow(Label::Overflow::SHRINK);
    lbClose->setPosition(Vec2(itemClose->getContentSize()/2));
    itemClose->addChild(lbClose);
    
    initUIContents();
}

void PopupFirstUpgradeInfiniteArtifact::initUIContents()
{
    auto lbDesc1 = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_done_desc_1"), GAME_FONT, 24);
    lbDesc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbDesc1->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbDesc1->setPosition(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 95);
    lbDesc1->setDimensions(_spriteContainer->getContentSize().width - 50, 80);
    lbDesc1->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(lbDesc1);
    
    auto lbArti = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_btn"), GAME_FONT, 24);
    lbArti->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbArti->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    lbArti->setPosition(lbDesc1->getPosition() + Vec2::DOWN * (lbDesc1->getContentSize().height - 15));
    lbArti->setDimensions(_spriteContainer->getContentSize().width - 50, 80);
    lbArti->setOverflow(Label::Overflow::SHRINK);
    lbArti->setColor(COLOR_COIN);
    _spriteContainer->addChild(lbArti);
    
    std::vector<std::pair<std::string, std::string>> vecArtiPath;
    vecArtiPath.clear();
    
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_104.png", GAME_TEXT("t_arti_d1_unlimit_title_1")));
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_105.png", GAME_TEXT("t_arti_d1_unlimit_title_2")));
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_106.png", GAME_TEXT("t_arti_d1_unlimit_title_3")));
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_107.png", GAME_TEXT("t_arti_d1_unlimit_title_4")));
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_108.png", GAME_TEXT("t_arti_d1_unlimit_title_5")));
    //일반 보물
    for(int i = 0; i < vecArtiPath.size(); ++i)
    {
        auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_bg_costume_02.png");
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBox->setContentSize(Size(160, 160));
        
        auto sprArti = ui::Scale9Sprite::create(vecArtiPath[i].first);
        sprArti->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprArti->setPosition(sprBox->getContentSize()/2);
        sprArti->setScale(0.7);
        sprArti->getSprite()->getTexture()->setTexParameters(texParams);
        _spriteContainer->addChild(sprBox);
        sprBox->addChild(sprArti);
        
        auto lbName = Label::createWithTTF(vecArtiPath[i].second, GAME_FONT, 22);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbName->setPosition(sprBox->getContentSize().width/2, 20);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        lbName->setDimensions(sprBox->getContentSize().width - 70 , 40);
        lbName->setOverflow(Label::Overflow::SHRINK);
        sprBox->addChild(lbName);
           
        float startX = 0;
        Vec2 pos = Vec2::ZERO;
           
        startX = _spriteContainer->getContentSize().width / (vecArtiPath.size() + 1);
        
        pos = Vec2(startX + i * startX , _spriteContainer->getContentSize().height - _spriteContainer->getContentSize().height / 3.1f);
           
        float lastPosX = startX + (vecArtiPath.size() - 1) * startX;
           
           
        if(lastPosX + sprBox->getContentSize().width / 2 > _spriteContainer->getContentSize().width)
        {
            float ratio = _spriteContainer->getContentSize().width / (lastPosX + sprBox->getContentSize().width / 2);
            sprBox->setScale(ratio);
        }
        sprBox->setPosition(pos);
    }
    
    auto lbSpArti = Label::createWithTTF(GAME_TEXT("t_ui_artifact_tab_2"), GAME_FONT, 24);
    lbSpArti->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbSpArti->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    lbSpArti->setPosition(Vec2(_spriteContainer->getContentSize()/2) + Vec2::UP * 50);
    lbSpArti->setDimensions(_spriteContainer->getContentSize().width - 50, 75);
    lbSpArti->setOverflow(Label::Overflow::SHRINK);
    lbSpArti->setColor(COLOR_COIN);
    _spriteContainer->addChild(lbSpArti);
    
    vecArtiPath.clear();
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_109.png", GAME_TEXT("t_arti_d1_unlimit_title_6")));
    vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_110.png", GAME_TEXT("t_arti_d1_unlimit_title_7")));
    //증폭기 주석처리 (추가 예정)
    //vecArtiPath.push_back(std::make_pair("Assets/icon_artifact/artifact_111.png", GAME_TEXT("t_arti_d1_unlimit_title_8")));
    //스패셜 보물
    for(int i = 0; i < vecArtiPath.size(); ++i)
    {
        auto sprBox = ui::Scale9Sprite::create("Assets/ui_common/box_bg_costume_02.png");
        sprBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprBox->setContentSize(Size(160, 160));
        
        auto sprArti = ui::Scale9Sprite::create(vecArtiPath[i].first);
        sprArti->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sprArti->setPosition(sprBox->getContentSize()/2);
        sprArti->setScale(0.7);
        sprArti->getSprite()->getTexture()->setTexParameters(texParams);
        _spriteContainer->addChild(sprBox);
        sprBox->addChild(sprArti);
        
        auto lbName = Label::createWithTTF(vecArtiPath[i].second, GAME_FONT, 22);
        lbName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        lbName->setPosition(sprBox->getContentSize().width/2, 20);
        lbName->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
        lbName->setDimensions(sprBox->getContentSize().width , 25);
        lbName->setOverflow(Label::Overflow::SHRINK);
        sprBox->addChild(lbName);
        
        float startX = 0;
        Vec2 pos = Vec2::ZERO;
           
        startX = _spriteContainer->getContentSize().width / (vecArtiPath.size() + 1);
        
        pos = Vec2(startX + i * startX , _spriteContainer->getContentSize().height/2 - sprBox->getContentSize().height/2 + 35);
           
        float lastPosX = startX + (vecArtiPath.size() - 1) * startX;
           
           
        if(lastPosX + sprBox->getContentSize().width / 2 > _spriteContainer->getContentSize().width)
        {
            float ratio = _spriteContainer->getContentSize().width / (lastPosX + sprBox->getContentSize().width / 2);
            sprBox->setScale(ratio);
        }
        sprBox->setPosition(pos);
    }
    
    auto lbDesc2 = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_done_desc_2"), GAME_FONT, 24);
    lbDesc2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    lbDesc2->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    lbDesc2->setPosition(Vec2(_spriteContainer->getContentSize().width/2, 0) + Vec2::UP * 270);
    lbDesc2->setDimensions(_spriteContainer->getContentSize().width - 50, 150);
    lbDesc2->setOverflow(Label::Overflow::SHRINK);
    _spriteContainer->addChild(lbDesc2);
    
}

void PopupFirstUpgradeInfiniteArtifact::onClickButton(Ref* sender)
{
    Node* item = (Node*)sender;
    
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
}

#pragma mark - Upgrade

PopupUpgradeInfiniteArtifact* PopupUpgradeInfiniteArtifact::create()
{
    PopupUpgradeInfiniteArtifact* pRet = new(std::nothrow) PopupUpgradeInfiniteArtifact();
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

PopupUpgradeInfiniteArtifact::PopupUpgradeInfiniteArtifact(void):
_callbackMoveInfinite(nullptr)
{
}

PopupUpgradeInfiniteArtifact::~PopupUpgradeInfiniteArtifact(void)
{
}

bool PopupUpgradeInfiniteArtifact::init()
{
    if ( !PopupBase::init() )
    {
        return false;
    }
    
    initVar();
    initUI();
    

    return true;
}
void PopupUpgradeInfiniteArtifact::initVar()
{
    double nWidth = 650;
    double nHeight = getContentSize().height;
    if ( nHeight > 600 )
        nHeight = 600;
    
    _spriteContainer->setContentSize(Size(nWidth, nHeight));
}

void PopupUpgradeInfiniteArtifact::initUI()
{
    auto menu = MafNode::MafMenu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(Vec2::ZERO);
    _spriteContainer->addChild(menu);
    
    auto lbTitle = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_title"), GAME_FONT, 30);
    lbTitle->setColor(COLOR_TITLE);
    lbTitle->setPosition(Vec2(_spriteContainer->getContentSize().width/2, _spriteContainer->getContentSize().height - 40));
    _spriteContainer->addChild(lbTitle);
    
    auto sprClose = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO);
    sprClose->setContentSize(Size(150, 100));
    
    auto itemClose = MafNode::MafMenuItemSprite::create(sprClose, nullptr, CC_CALLBACK_1(PopupUpgradeInfiniteArtifact::onClickButton, this));
    
    itemClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemClose->setPosition( Vec2(_spriteContainer->getContentSize().width/2, 20) + Vec2::LEFT * (sprClose->getContentSize().width/2 + 10));
    itemClose->setTouchRect( sprClose->getContentSize());
    itemClose->setName("CLOSE");
    menu->addChild(itemClose);
    
    auto lbClose = Label::createWithTTF(GAME_TEXT("t_ui_close"), GAME_FONT, 25);
    lbClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbClose->setHorizontalAlignment(TextHAlignment::CENTER);
    lbClose->setVerticalAlignment(TextVAlignment::CENTER);
    lbClose->setDimensions(itemClose->getContentSize().width-10, itemClose->getContentSize().height-5);
    lbClose->setOverflow(Label::Overflow::SHRINK);
    lbClose->setPosition(Vec2(itemClose->getContentSize()/2));
    itemClose->addChild(lbClose);
    
    auto sprActivate = ui::Scale9Sprite::create("Assets/ui_common/btn_2_1_on.png", Rect::ZERO);
    sprActivate->setContentSize(Size(150, 100));
    
    auto itemActivate = MafNode::MafMenuItemSprite::create(sprActivate, nullptr, CC_CALLBACK_1(PopupUpgradeInfiniteArtifact::onClickButton, this));
    itemActivate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    itemActivate->setPosition( Vec2(_spriteContainer->getContentSize().width/2, 20) + Vec2::RIGHT * (sprActivate->getContentSize().width/2 + 10));
    itemActivate->setTouchRect( sprActivate->getContentSize());
    itemActivate->setName("BUY_INFINITE_ARTIFACT");
    menu->addChild(itemActivate);
    
    auto lbActivate = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_btn"), GAME_FONT, 25);
    lbActivate->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    lbActivate->setHorizontalAlignment(TextHAlignment::CENTER);
    lbActivate->setVerticalAlignment(TextVAlignment::CENTER);
    lbActivate->setDimensions(itemActivate->getContentSize().width-10, itemActivate->getContentSize().height-5);
    lbActivate->setOverflow(Label::Overflow::SHRINK);
    lbActivate->setPosition(Vec2(itemActivate->getContentSize()/2) + Vec2::UP * 25);
    itemActivate->addChild(lbActivate);
    
    auto sprCoin = Sprite::create("Assets/icon/icon_coin_2.png");
    sprCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprCoin->setPosition(Vec2(itemActivate->getContentSize()/2) + Vec2::LEFT * sprCoin->getContentSize().width/2 + Vec2::DOWN * 15);
    itemActivate->addChild(sprCoin);
    
    auto lbPrice = Label::createWithTTF("-", GAME_FONT, 25);
    lbPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbPrice->setHorizontalAlignment(TextHAlignment::LEFT);
    lbPrice->setVerticalAlignment(TextVAlignment::CENTER);
    lbPrice->setDimensions(itemActivate->getContentSize().width-10, 50);
    lbPrice->setOverflow(Label::Overflow::SHRINK);
    lbPrice->setPosition(sprCoin->getPosition() + Vec2::RIGHT * 20);
    lbPrice->setColor(COLOR_COIN);
    
    auto baseInfo = InfiniteArtifactManager::getInstance()->getInfiniteBasicInfoFromGrade(InfiniteArtifactManager::getInstance()->getCurGrade()+1);
    if(baseInfo != nullptr)
        lbPrice->setString(MafUtils::toString(baseInfo->getDemandCoin()));
    
    itemActivate->addChild(lbPrice);
    
    initUIContents();
}

void PopupUpgradeInfiniteArtifact::initUIContents()
{
    auto sprIconBox = Sprite::create("Assets/ui_common/box_bg_costume_02.png");
    sprIconBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprIconBox->setPosition(Vec2(_spriteContainer->getContentSize().width/2 ,_spriteContainer->getContentSize().height - _spriteContainer->getContentSize().height / 3.3f ));
    _spriteContainer->addChild(sprIconBox);
    
    auto sprIconEffect = Sprite::create("Assets/ui_common/box_transcendence1_2.png");
    sprIconEffect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprIconEffect->setPosition(sprIconBox->getContentSize()/2);
    sprIconBox->addChild(sprIconEffect);
    
    
    auto rotate = RotateBy::create(10, 360);
    sprIconEffect->runAction(RepeatForever::create(rotate));
    
    auto sprInfiniteIcon = Sprite::create();
    sprInfiniteIcon->setPosition(sprIconBox->getContentSize()/2);
    sprInfiniteIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprIconBox->addChild(sprInfiniteIcon);
    sprInfiniteIcon->setScale(1.3f);
    
    // 왼쪽 버튼 보물 아이콘 이미지
    if(!SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded("Assets/icon_artifact/transcendence/infinite_icon.plist"))
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Assets/icon_artifact/transcendence/infinite_icon.plist");
    
    sprInfiniteIcon->getTexture()->setTexParameters(texParams);
    //animation  객체 생성
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.1f);
    for (int i = 1; i < 12; i++)
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("icon_transcendence_%02d.png", i+1));

        animation->addSpriteFrame(frame);
    }

    //animate 객체 생성, animation의 액션형
    auto animate = Animate::create(animation);

    //런액션 실행
    sprInfiniteIcon->runAction(RepeatForever::create(animate));
    
    auto lbContent = Label::createWithTTF(GAME_TEXT("t_ui_menu_arti_ascend_detail"), GAME_FONT, 25);
    lbContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    lbContent->setPosition(_spriteContainer->getContentSize().width/2, 130);
    lbContent->setDimensions(_spriteContainer->getContentSize().width - 50, 250);
    lbContent->setOverflow(Label::Overflow::SHRINK);
    lbContent->setHorizontalAlignment(TextHAlignment::CENTER);
    lbContent->setVerticalAlignment(TextVAlignment::CENTER);
    _spriteContainer->addChild(lbContent);
    
}

void PopupUpgradeInfiniteArtifact::onClickButton(Ref* sender)
{
    
    Node* item = (Node*)sender;
    if(item->getName().compare("CLOSE") == 0)
    {
        hide();
    }
    else if(item->getName().compare("BUY_INFINITE_ARTIFACT") == 0)
    {
        InfiniteArtifactManager::getInstance()->requestInfiniteArtiBuy(CC_CALLBACK_1(PopupUpgradeInfiniteArtifact::callbackBuy, this));
    }
    
}

void PopupUpgradeInfiniteArtifact::callbackBuy(bool nResult)
{
    if(_callbackMoveInfinite != nullptr)
        _callbackMoveInfinite(nResult);
    hide();
}

void PopupUpgradeInfiniteArtifact::setCallbackMoveInfinite(std::function<void(bool)> callback)
{
    _callbackMoveInfinite = callback;
}
