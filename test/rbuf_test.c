/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2019-03-07T13:46:33-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2019-03-07T22:59:29-06:00
 */

#include "ring_buffer.h"
#include "CppUTest/TestHarness_c.h"
#include <time.h>

static int stat;
static Rbuf *rbuf;
unsigned long long range = 100;


TEST_GROUP_C_SETUP(RbufTest)
{
    stat = rbuf_new(&rbuf);
}

TEST_GROUP_C_TEARDOWN(RbufTest)
{
    rbuf_destroy(rbuf);
}

TEST_C(RbufTest, RbufEnqueue)
{
    uint64_t items[10];
    memset(items, 0, sizeof(uint64_t)*10);
    srand((unsigned int) time(NULL));
    for (int i = 0; i < 10; i++) {
      uint64_t item = rand() % range;
      rbuf_enqueue(rbuf, item);
      items[i] = item;
    }

    for (int i = 0; i < 10; i++)
        CHECK_EQUAL_C_INT(rbuf->buf[i], items[i]);
}

TEST_C(RbufTest, RbufDequeue)
{
     uint64_t items[10];
     memset(items, 0, sizeof(uint64_t)*10);
     srand((unsigned int) time(NULL));
     for (int i = 0; i < 10; i++) {
       uint64_t item = rand() % range + 1;
       items[i] = item;
       rbuf_enqueue(rbuf, item);
     }

     uint64_t *out = rbuf->mem_alloc(sizeof(uint64_t));
     for (int i = 0; i < 10; i++) {
       rbuf_dequeue(rbuf, out);
       CHECK_EQUAL_C_INT(items[i], *out);
       memset(out, 0, sizeof(uint64_t));
     }
     rbuf->mem_free(out);
}
