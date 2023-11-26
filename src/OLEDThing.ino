#include <Thing.h>
#include <BlinkPattern.h>
#include "io/SPIBus.h"
#include "screenbuffer/ByteUpdateScreenBuffer.h"
#include "devices/SSD1306.h"

using namespace ootb;
using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
SPIBus io(D3, D2, D1);
ByteUpdateScreenBuffer buffer(128, 64, 1);
SSD1306 oled(&io, &buffer);

BlinkPattern::Pattern<2> initialize{{1,1},50};
BlinkPattern::Pattern<2> on{{39,1},25};

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("ClientID:" + thing.clientId());

    oled.begin();
    oled.flipHorizontal(true);
    oled.flipVertical(true);
    led.setPattern(initialize);

    showText(thing.clientId());

    thing.onStateChange([](const String& msg)
    {
        showText(msg);
        Serial.println(msg);
    });

    thing.begin();
    thing.setClientBufferSize(1024);

    String topic = "things/" + thing.clientId() + "/oled/text";
    thing.addActuator(topic, [](Value& value)
    {
        showText((String)value);
        Serial.println((String)value);
    });

    led.setPattern(on);
}

void loop()
{
    thing.handle();
}

void showText(const String& value)
{
    oled.clear();
    oled.setCursor(0,0);
    oled.print(value);
    oled.display();
}
