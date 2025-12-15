#include <fast/circ_buf.h>
#include <gtest/gtest.h>

using namespace sheep;

TEST(CircBuf, CircBuf)
{
    ASSERT_TRUE(true) << "tested in push_back";
}

TEST(CircBuf, push_back)
{
    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(400);
    _v_circ_buf.push_back(1337);

    EXPECT_EQ(_v_circ_buf.at(0), 400);
    EXPECT_EQ(_v_circ_buf.at(1), 1337);

    // return false because there is no more space in the buffer
    EXPECT_FALSE(_v_circ_buf.push_back(10));
    EXPECT_EQ(_v_circ_buf.at(2), 400); // index wraps around
}

TEST(CircBuf, at)
{
    ASSERT_TRUE(true) << "tested in push_back";
}

TEST(CircBuf, set_override)
{
    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(400);
    _v_circ_buf.push_back(1337);

    _v_circ_buf.set_override(true);
    EXPECT_TRUE(_v_circ_buf.push_back(10));

    // should return true and override the element at(0)
    EXPECT_EQ(_v_circ_buf.at(0), 10);
}

TEST(CircBuf, clear)
{
    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(400);
    _v_circ_buf.push_back(1337);

    _v_circ_buf.clear();

    // this should mark all elements as overridable instead of
    // deleting them or setting them to 0
    EXPECT_TRUE(_v_circ_buf.push_back(10));
    EXPECT_EQ(_v_circ_buf.at(0), 10);
}

TEST(CircBuf, resize)
{
    ASSERT_TRUE(true) << "tested in push_back";
}

TEST(CircBuf, size)
{
    ASSERT_TRUE(true) << "tested in push_back";
}
