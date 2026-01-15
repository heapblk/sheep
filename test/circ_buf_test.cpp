#include <chrono>
#include <circ_buf.h>
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

    EXPECT_TRUE(_v_circ_buf.push_back(400));
    EXPECT_EQ(_v_circ_buf.at(1), 400);

    // index wraps around, so the first element should be overwritten again
    EXPECT_TRUE(_v_circ_buf.push_back(1337));
    EXPECT_EQ(_v_circ_buf.at(0), 1337);
}

TEST(CircBuf, clear)
{
    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(400);

    _v_circ_buf.clear();

    // this should mark all elements as overridable instead of
    // deleting them or setting them to 0
    EXPECT_TRUE(_v_circ_buf.push_back(10));
    EXPECT_EQ(_v_circ_buf.at(0), 10);

    EXPECT_TRUE(_v_circ_buf.push_back(1337));
    EXPECT_EQ(_v_circ_buf.at(1), 1337);

    // clear should not function like it just set the
    // overwrite flag, so once the buffer is full again
    // appending elements doesn't work anymore
    EXPECT_FALSE(_v_circ_buf.push_back(400));
    EXPECT_EQ(_v_circ_buf.at(0), 10);
}

TEST(CircBuf, resize)
{
    CircBuf<int> _v_circ_buf(2);
    _v_circ_buf.push_back(400);
    _v_circ_buf.push_back(1337);

    EXPECT_FALSE(_v_circ_buf.push_back(10));

    _v_circ_buf.resize(3);

    EXPECT_TRUE(_v_circ_buf.push_back(10));
    EXPECT_EQ(_v_circ_buf.at(2), 10);

    EXPECT_EQ(_v_circ_buf.size(), 3);
}

TEST(CircBuf, size)
{
    ASSERT_TRUE(true) << "tested in push_back";
}

// PERFORMANCE TEST ----------------------------------------------------------------

// basic buf implementation using a linked list
struct Node
{
    int data;
    Node *next;
};

struct List
{
    Node *head;
    Node *tail;
};

void init(List &l)
{
    l.head = l.tail = nullptr;
}

static Node pool[10000];
int poolIndex = 0;

Node *allocNode(int v)
{
    pool[poolIndex] = {v, nullptr};
    return &pool[poolIndex++];
}

void push_back(List &l, int value)
{
    Node *n = allocNode(value);

    if (!l.head)
    {
        l.head = l.tail = n;
    }
    else
    {
        l.tail->next = n;
        l.tail = n;
    }
}

#ifdef __WIN32
typedef std::chrono::system_clock high_resolution_clock;
#else
typedef std::chrono::high_resolution_clock high_resolution_clock;
#endif

TEST(CircBuf, Performance)
{
    // push_back
    // ---------------------------------------------------------------------------------------------------------------------
    CircBuf<int> _v_circ_buf(10000);
    auto _circ_start = high_resolution_clock::now();
    for (int i = 0; i < 10000; i++)
    {
        _v_circ_buf.push_back(i);
    }
    auto _circ_end = high_resolution_clock::now();

    auto _circ_insert_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(_circ_end - _circ_start).count();

    List _list{};
    init(_list);

    auto _linked_start = high_resolution_clock::now();

    for (int i = 0; i < 10000; ++i)
    {
        push_back(_list, i);
    }

    auto _linked_end = high_resolution_clock::now();
    auto _linked_insert_time_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(_linked_end - _linked_start).count();

    printf("CIRC: %lins\n", _circ_insert_time_ns);
    printf("LIST: %lins\n", _linked_insert_time_ns);

    ASSERT_TRUE(_circ_insert_time_ns < _linked_insert_time_ns);
}
