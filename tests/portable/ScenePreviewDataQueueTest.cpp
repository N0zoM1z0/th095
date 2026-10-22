#include "ScenePreviewDataQueue.hpp"
#include <array>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>
#include <vector>

namespace th095 {
#include "ScenePreviewDataQueue.inl"
}
using th095::ScenePreviewDataQueue;
typedef std::array<uintptr_t, 18> Words;
static_assert(sizeof(void *) == 8, "Require native x86-64 addresses");
static_assert(sizeof(ScenePreviewDataQueue) == 144, "Eighteen native address words");
static_assert(sizeof(th095::SceneValueQueue) == 72, "Integer channels stay narrow");
static_assert(std::is_same<decltype(ScenePreviewDataQueue().Size()), i32>::value,
              "Count API remains a four-byte integer");
static void Check(bool condition, int line) {
    if (!condition) { fprintf(stderr, "preview data oracle failed at %d\n", line); exit(1); }
}
#define CHECK(x) Check((x), __LINE__)
static void Publish(ScenePreviewDataQueue *queue, u8 **value) {
using namespace th095;
#include "ScenePreviewDataPublish.inl"
}
static void PublishNull(ScenePreviewDataQueue *queue) {
using namespace th095;
#include "ScenePreviewDataPublishNull.inl"
}
static Words Snapshot(const ScenePreviewDataQueue &queue) {
    Words result;
    for (int i=0;i<18;++i) result[i]=queue.words[i].bits;
    return result;
}
static uintptr_t ModelPop(Words &state) {
    if (static_cast<i32>(state[16]) <= 0) return 0;
    Words before=state;
    for(int i=0;i<15;++i) state[i]=before[i+1];
    state[15]=before[16]-1;
    state[16]=before[16]-1;
    return before[1];
}
int main() {
    std::array<std::unique_ptr<u8[]>,16> buffers;
    for (int i=0;i<16;++i) {
        buffers[i].reset(new u8[8]); buffers[i][0]=static_cast<u8>(i+1);
        CHECK(reinterpret_cast<uintptr_t>(buffers[i].get()) > 0xffffffffu);
    }
    for (int count=0;count<=16;++count) {
        ScenePreviewDataQueue queue;
        for(int i=0;i<count;++i) { u8 *p=buffers[i].get(); Publish(&queue,&p); }
        CHECK(queue.Size()==count);
        CHECK(queue.Front()==(count ? buffers[0].get() : 0));
        for(int i=0;i<count;++i) CHECK(queue.words[i].bits==reinterpret_cast<uintptr_t>(buffers[i].get()));
        Words expected=Snapshot(queue);
        uintptr_t result=ModelPop(expected);
        CHECK(queue.Pop()==result); CHECK(Snapshot(queue)==expected);
    }
    // Drop-if-full publication retains the original payload, unlike raw Push.
    for(int capacity=1;capacity<=16;++capacity) {
        ScenePreviewDataQueue queue;
        queue.words[17].bits=static_cast<uintptr_t>(capacity);
        for(int i=0;i<16;++i) {u8 *p=buffers[i].get(); Publish(&queue,&p);}
        CHECK(queue.Size()==capacity);
        for(int i=0;i<capacity;++i) CHECK(queue.words[i].bits==reinterpret_cast<uintptr_t>(buffers[i].get()));
    }
    // Actual native buffers follow the scene-update read/free-before-Pop lane.
    ScenePreviewDataQueue primary,secondary;
    u8 *p=buffers[0].get(),*s=buffers[1].get();
    Publish(&primary,&p); Publish(&secondary,&s);
    CHECK(primary.Front()[0]==1 && secondary.Front()[0]==2);
    CHECK(primary.Front()==buffers[0].get() && secondary.Front()==buffers[1].get());
    buffers[0].reset(); buffers[1].reset();
    CHECK(primary.Pop()==0 && secondary.Pop()==0);
    CHECK(primary.Size()==0 && secondary.Size()==0);
    // Null secondary publication is still a real queue item.
    PublishNull(&secondary); CHECK(secondary.Size()==1 && secondary.Front()==0);
    CHECK(secondary.Pop()==0 && secondary.Size()==0);
    // Observe teardown arguments without calling free on synthetic metadata.
    ScenePreviewDataQueue drain;
    for(int i=2;i<16;++i) {u8 *item=buffers[i].get(); Publish(&drain,&item);}
    std::vector<uintptr_t> released;
    while(drain.Size()!=0) released.push_back(drain.Pop());
    CHECK(released.size()==14);
    CHECK(released.front()==reinterpret_cast<uintptr_t>(buffers[3].get()));
    CHECK(released.back()==0);
    // A full synthetic queue eventually exposes count metadata as an argument.
    ScenePreviewDataQueue full;
    for(int i=0;i<16;++i) full.words[i].bits=0x100000000ull+static_cast<uintptr_t>(i)*16;
    full.words[16].bits=16;
    uintptr_t last=0;
    while(full.Size()!=0) last=full.Pop();
    CHECK(last==15 && full.Front()==0);
    puts("preview data: native high addresses, publication, full-state shift, release-order traces passed");
}
