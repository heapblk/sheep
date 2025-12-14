#include <fast/circ_buf.h>
#include <gtest/gtest.h>

using namespace sheep;
TEST(CircBuf, push_back)
{
    int first_index = 400;
    int second_index = 1337;

    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(first_index);
    _v_circ_buf.push_back(second_index);

    EXPECT_EQ(_v_circ_buf.at(0), first_index);
    EXPECT_EQ(_v_circ_buf.at(1), second_index);
}
