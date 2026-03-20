#include <chrono>
#include <containers/circ_buf.h>
#include <gtest/gtest.h>

using namespace sheep;

TEST(CircBuf, CircBuf) { ASSERT_TRUE(true) << "tested in push_back"; }

TEST(CircBuf, push_back) {
  CircBuf<int> v_circ_buf(2);
  v_circ_buf.push_back(400);
  v_circ_buf.push_back(1337);

  EXPECT_EQ(v_circ_buf.at(0), 400);
  EXPECT_EQ(v_circ_buf.at(1), 1337);

  // return false because there is no more space in the buffer
  EXPECT_FALSE(v_circ_buf.push_back(10));
  EXPECT_EQ(v_circ_buf.at(2), 400); // index wraps around
}

TEST(CircBuf, at) { ASSERT_TRUE(true) << "tested in push_back"; }

TEST(CircBuf, set_override) {
  CircBuf<int> v_circ_buf(2);
  v_circ_buf.push_back(400);
  v_circ_buf.push_back(1337);

  v_circ_buf.set_override(true);
  EXPECT_TRUE(v_circ_buf.push_back(10));

  // should return true and override the element at(0)
  EXPECT_EQ(v_circ_buf.at(0), 10);

  EXPECT_TRUE(v_circ_buf.push_back(400));
  EXPECT_EQ(v_circ_buf.at(1), 400);

  // index wraps around, so the first element should be overwritten again
  EXPECT_TRUE(v_circ_buf.push_back(1337));
  EXPECT_EQ(v_circ_buf.at(0), 1337);
}

TEST(CircBuf, clear) {
  CircBuf<int> v_circ_buf(2);
  v_circ_buf.push_back(400);

  v_circ_buf.clear();

  // this should mark all elements as overridable instead of
  // deleting them or setting them to 0
  EXPECT_TRUE(v_circ_buf.push_back(10));
  EXPECT_EQ(v_circ_buf.at(0), 10);

  EXPECT_TRUE(v_circ_buf.push_back(1337));
  EXPECT_EQ(v_circ_buf.at(1), 1337);

  // clear should not function like it just set the
  // overwrite flag, so once the buffer is full again
  // appending elements doesn't work anymore
  EXPECT_FALSE(v_circ_buf.push_back(400));
  EXPECT_EQ(v_circ_buf.at(0), 10);
}

TEST(CircBuf, resize) {
  CircBuf<int> v_circ_buf(2);
  v_circ_buf.push_back(400);
  v_circ_buf.push_back(1337);

  EXPECT_FALSE(v_circ_buf.push_back(10));

  v_circ_buf.resize(3);

  EXPECT_TRUE(v_circ_buf.push_back(10));
  EXPECT_EQ(v_circ_buf.at(2), 10);

  EXPECT_EQ(v_circ_buf.size(), 3);
}

TEST(CircBuf, size) { ASSERT_TRUE(true) << "tested in push_back"; }

// PERFORMANCE TEST
// ----------------------------------------------------------------

// basic buf implementation using a linked list
struct Node {
  int _data;
  Node *_next;
};

struct List {
  Node *_head;
  Node *_tail;
};

void init(List &l) { l._head = l._tail = nullptr; }

static Node _pool[100000];
int _poolIndex = 0;

Node *allocNode(int v) {
  _pool[_poolIndex] = {v, nullptr};
  return &_pool[_poolIndex++];
}

void push_back(List &l, int value) {
  Node *_n = allocNode(value);

  if (!l._head) {
    l._head = l._tail = _n;
  } else {
    l._tail->_next = _n;
    l._tail = _n;
  }
}

#ifdef __WIN32
typedef std::chrono::system_clock high_resolution_clock;
#else
typedef std::chrono::high_resolution_clock high_resolution_clock;
#endif

TEST(CircBuf, Performance) {
  // push_back
  // ---------------------------------------------------------------------------------------------------------------------
  CircBuf<int> v_circ_buf(100000);
  auto circ_start = high_resolution_clock::now();
  for (int i = 0; i < 100000; i++) {
    v_circ_buf.push_back(i);
  }
  auto circ_end = high_resolution_clock::now();

  auto circ_insert_time_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(circ_end -
                                                           circ_start)
          .count();

  List list{};
  init(list);

  auto linked_start = high_resolution_clock::now();

  for (int i = 0; i < 100000; ++i) {
    push_back(list, i);
  }

  auto linked_end = high_resolution_clock::now();
  auto linked_insert_time_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(linked_end -
                                                           linked_start)
          .count();

  printf("CIRC: %llins\n", circ_insert_time_ns);
  printf("LIST: %llins\n", linked_insert_time_ns);

  ASSERT_TRUE(circ_insert_time_ns < linked_insert_time_ns);
}
