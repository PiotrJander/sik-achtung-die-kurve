//
// Created by Piotr Jander on 22/08/2017.
//

#include <sstream>
#include <nietacka/PixelEvent.h>

#include "gtest/gtest.h"


TEST(GameEvent__Test, One)
{
    std::stringstream s;

    PixelEvent e1 {123, 4, 567, 890};
    e1.writeTo(s);
    
    auto _e2 = GameEvent::readFrom(s);
    auto *e2 = (PixelEvent *) _e2.get();

//    std::cout << sizeof(e1.header.getType());

    ASSERT_EQ(e1.header.getEventNo(), e2->header.getEventNo());
    ASSERT_EQ(e1.header.getType(), e2->header.getType());
    ASSERT_EQ(e1.data.getPlayerNumber(), e2->data.getPlayerNumber());
    ASSERT_EQ(e1.data.getX(), e2->data.getX());
    ASSERT_EQ(e1.data.getY(), e2->data.getY());
}
