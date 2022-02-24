#include <gmock/gmock.h>
#include <emitter.hpp>

struct FakeEvent {
};

struct FakeEmitter : public Emitter<FakeEmitter, FakeEvent> {
};

struct FakeEmitterFixture : public ::testing::Test {
    FakeEmitterFixture() :
            fakeHandle{std::make_shared<FakeEmitter>()} {
    }

    std::shared_ptr<FakeEmitter> fakeHandle;
};

TEST_F(FakeEmitterFixture, on_publish) {
    auto called{false};
    fakeHandle->on([&called](const auto &, auto &) {
        called = true;
    });
    fakeHandle->publish(FakeEvent{});
    ASSERT_TRUE(called);
}

TEST_F(FakeEmitterFixture, on_empty) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->on([](const auto &, auto &) {
    });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->publish(FakeEvent{});
    ASSERT_FALSE(fakeHandle->empty());
}

TEST_F(FakeEmitterFixture, on_clear) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->on([](const auto &, auto &) { FAIL(); });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->clear();
    ASSERT_TRUE(fakeHandle->empty());
}
