#include <gmock/gmock.h>
#include <emitter.hpp>

struct FakeEvent {
};

struct FakeEmitter : public Emitter<FakeEmitter, FakeEvent> {
};

struct FakeEmitterFixture : public ::testing::Test {
    FakeEmitterFixture() :
            fakeEmitter{std::make_shared<FakeEmitter>()} {
    }

    std::shared_ptr<FakeEmitter> fakeEmitter;
};

TEST_F(FakeEmitterFixture, e2e) {
    auto called{false};
    ASSERT_TRUE(fakeEmitter->empty());

    fakeEmitter->on([&called](const auto &, auto &) {
        called = true;
    });
    ASSERT_FALSE(fakeEmitter->empty());
    
    fakeEmitter->publish(FakeEvent{});
    ASSERT_TRUE(called);
    
    fakeEmitter->clear();
    ASSERT_TRUE(fakeEmitter->empty());
}
