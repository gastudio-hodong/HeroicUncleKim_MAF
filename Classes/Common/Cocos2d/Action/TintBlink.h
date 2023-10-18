//
//  TintBlink.h
//
//

#ifndef TintBlink_h
#define TintBlink_h

#include <cocos2d.h>

NS_CC_BEGIN

class TintBlink : public ActionInterval
{
public:
    static TintBlink* create(float duration, cocos2d::Color3B color);
    static TintBlink* create(float duration, cocos2d::Color3B from, cocos2d::Color3B to);
    
    virtual TintBlink* clone() const override;
    virtual TintBlink* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void stop() override;
    /**
     * @param time in seconds
     */
    virtual void update(float time) override;
    
CC_CONSTRUCTOR_ACCESS:
    TintBlink();
    virtual ~TintBlink();
    
    bool initWithDuration(float duration, Color3B from, Color3B to);
    
protected:
    cocos2d::Color3B _fromColor;
    cocos2d::Color3B _toColor;
    
    float _tick;
    bool _isChange;
    
};

NS_CC_END

#endif
