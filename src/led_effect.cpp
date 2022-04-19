#include <FastLED.h>
FASTLED_USING_NAMESPACE

enum LED_EFFECT_TYPE
{
    RAINBOW,
    STATIC,
    NONE
};
class LED_EFFECT
{

public:
    LED_EFFECT(LED_EFFECT_TYPE type, CRGB color)
    {
        this->type = type;
        this->color = color;
    }
    LED_EFFECT_TYPE type;
    CRGB color;
};