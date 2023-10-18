//
//  Model.cpp
//  HexagonSnake
//
//  Created by SanHeo on 2014. 7. 27..
//
//

#include "UnitAnimationMonsterFrost.h"

#include "Common/ConfigGameNormal.h"
 
USING_NS_CC;

UnitAnimationMonsterFrost* UnitAnimationMonsterFrost::create()
{
    UnitAnimationMonsterFrost *pRet = new (std::nothrow) UnitAnimationMonsterFrost();
    if ( pRet && pRet->init() )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

UnitAnimationMonsterFrost::UnitAnimationMonsterFrost() :
_eType(E_TYPE::DEFAULT)

{
    
}

UnitAnimationMonsterFrost::~UnitAnimationMonsterFrost()
{

}

bool UnitAnimationMonsterFrost::init()
{
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 0)) )
    {
        return false;
    }
    
    setContentSize(Size(378, 342));
    setIgnoreAnchorPointForPosition(false);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

    Size size = getContentSize();
   
    Vec2 pos = Vec2(size.width + 204 - size.width / 2, -57);
    
    
    //
    std::vector<E_PART> listPart = {E_PART::BODY};
    for ( int i = 0; i < listPart.size(); i++ )
    {
        auto ePart = listPart.at(i);
        
        auto part = Sprite::create();
        part->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        part->setPosition(pos);
        part->setScale(3);
        part->setTag(enumToInt(ePart));
        addChild(part, 1);
    }
    
    //
    setMotion();
    
    return true;
}

#pragma mark -
void UnitAnimationMonsterFrost::setMotion()
{
    //
    _eType = E_TYPE::DEFAULT;
    
    //
    std::string pathIdle = "Assets/ui/frost_prison/mamon_wait_ani%02d.png";
    _listAnimateIdle.insert(E_PART::BODY,  getMotionAnimate(pathIdle, 0.125));

    //
    std::string pathAngry = "Assets/ui/frost_prison/mamon_transform_ani%02d.png";
    _listAnimateAngry.insert(E_PART::BODY, getMotionAnimate(pathAngry, 0.125));
    
    //
    std::string pathAngryMove = "Assets/ui/frost_prison/mamon_walking_ani%02d.png";
    _listAnimateAngryMove.insert(E_PART::BODY, getMotionAnimate(pathAngryMove, 0.125));
}

#pragma mark - animation
Animate* UnitAnimationMonsterFrost::getMotionAnimate(std::string path, double delayPerUnit)
{
    Vector<cocos2d::SpriteFrame*> list;
    
    int count = 0;
    
    for ( int i = 1; i <= 100; i++ )
    {
        std::string strTempPath = MafUtils::format(path.c_str(), i);
        std::string fullpath = FileUtils::getInstance()->fullPathForFilename(strTempPath);
        
        if(fullpath.size() == 0)
            break;
        
        auto spriteAni = Sprite::create(strTempPath);
        if ( spriteAni == nullptr )
        {
            return nullptr;
        }
        
        spriteAni->getTexture()->setTexParameters(texParams);
        list.pushBack(spriteAni->getSpriteFrame());
        count = i;
    }
    
    delayPerUnit = delayPerUnit / (count / 4);
    
    if ( list.size() <= 0 )
    {
        return nullptr;
    }
    else
    {
        auto animation = Animation::createWithSpriteFrames(list, delayPerUnit);
        auto animate = Animate::create(animation);
        return animate;
    }
}

void UnitAnimationMonsterFrost::animationIdle()
{
    auto listChild = getChildren();
    for ( int i = 0; i < listChild.size(); i++ )
    {
        auto child = listChild.at(i);
        child->stopAllActions();
        child->setVisible(false);
    }
    
    // body
    {
        auto animate = _listAnimateIdle.at(E_PART::BODY);
        animate->getAnimation()->setDelayPerUnit(0.125);

        auto part = getChildByTag<Sprite*>(E_PART::BODY);
        part->setVisible(true);
        part->runAction(RepeatForever::create(animate));
    }
}

void UnitAnimationMonsterFrost::animationAngry(cocos2d::CallFunc *callback)
{
    auto listChild = getChildren();
    for ( int i = 0; i < listChild.size(); i++ )
    {
        auto child = listChild.at(i);
        child->stopAllActions();
        child->setVisible(false);
    }
    
    // body
    {
        auto animate = _listAnimateAngry.at(E_PART::BODY);
        animate->getAnimation()->setDelayPerUnit(0.125);

        auto part = getChildByTag<Sprite*>(E_PART::BODY);
        part->setVisible(true);
        part->runAction(Sequence::create(animate, callback, nullptr));
    }
}

void UnitAnimationMonsterFrost::animationAngryWalk()
{
    auto listChild = getChildren();
    for ( int i = 0; i < listChild.size(); i++ )
    {
        auto child = listChild.at(i);
        child->stopAllActions();
        child->setVisible(false);
    }
    
    // body
    {
        auto animate = _listAnimateAngryMove.at(E_PART::BODY);
        animate->getAnimation()->setDelayPerUnit(0.125);

        auto part = getChildByTag<Sprite*>(E_PART::BODY);
        part->setVisible(true);
        part->runAction(RepeatForever::create(animate));
    }
}


