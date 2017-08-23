//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include <nietacka/PixelEventData.h>
#include <nietacka/StreamUtils.h>

#include "gtest/gtest.h"


TEST(GameEvent__Test, One)
{
    std::stringstream s;

    StreamUtils::write_int<uint32_t>(s, 123);
    char str[] = "foo";
    s.write(str, sizeof(str));
    ASSERT_EQ(8, s.str().size());

//    PixelEventData e1(123, 4, 567, 890);
//    e1.writeTo(s);
//
//    std::unique_ptr<GameEvent> e2 = GameEvent::readFrom(s);
//    ASSERT_EQ(e1.getEventNumber(), e2->getEventNumber());
//    ASSERT_EQ(e1.getType(), e2->getType());
}
