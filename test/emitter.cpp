#include <gmock/gmock.h>
#include <emitter.hpp>

struct FakeEvent_int {
    int v;
};

struct FakeEvent_float {
    float v;
};

struct FakeEmitter : public Emitter<FakeEmitter, FakeEvent_int, FakeEvent_float> {
};

TEST(FakeEmitter, e2e) {
    FakeEmitter fakeEmitter;
    auto called{false};

    ASSERT_TRUE(fakeEmitter.empty<FakeEvent_int>());

    fakeEmitter.on<FakeEvent_int>([&called](const auto &, auto &) {
        called = true;
    });
    ASSERT_FALSE(fakeEmitter.empty<FakeEvent_int>());
    
    fakeEmitter.publish(FakeEvent_int{});
    ASSERT_TRUE(called);
    
    fakeEmitter.clear<FakeEvent_float>();
    ASSERT_TRUE(fakeEmitter.empty<FakeEvent_float>());

    fakeEmitter.on<FakeEvent_float>([&called](const auto &, auto &) {
        called = true;
    });
    ASSERT_FALSE(fakeEmitter.empty<FakeEvent_float>());
    
    fakeEmitter.publish(FakeEvent_float{});
    ASSERT_TRUE(called);
    
    fakeEmitter.clear<FakeEvent_float>();
    ASSERT_TRUE(fakeEmitter.empty<FakeEvent_float>());
}
