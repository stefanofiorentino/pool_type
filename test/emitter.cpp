#include <gmock/gmock.h>
#include <emitter.hpp>

struct FakeEvent {
};

struct FakeEmitter : public Emitter<FakeEmitter, FakeEvent> {
};

TEST(FakeEmitter, e2e) {
    FakeEmitter fakeEmitter;
    auto called{false};
    
    ASSERT_TRUE(fakeEmitter.empty());

    fakeEmitter.on([&called](const auto &, auto &) {
        called = true;
    });
    ASSERT_FALSE(fakeEmitter.empty());
    
    fakeEmitter.publish(FakeEvent{});
    ASSERT_TRUE(called);
    
    fakeEmitter.clear();
    ASSERT_TRUE(fakeEmitter.empty());
}
