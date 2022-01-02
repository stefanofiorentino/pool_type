#include <gmock/gmock.h>
#include <emitter.hpp>

struct FakeEvent1 {
};

struct FakeHandle : public Emitter<FakeHandle, FakeEvent1> {
};

struct FakeHandleFixture : public ::testing::Test {
    FakeHandleFixture() :
            fakeHandle{std::make_shared<FakeHandle>()} {
    }

    std::shared_ptr<FakeHandle> fakeHandle;
};

TEST_F(FakeHandleFixture, on_publish) {
    auto called{false};
    fakeHandle->on<FakeEvent1>([&called](const auto &, auto &) {
        called = true;
    });
    fakeHandle->publish(FakeEvent1{});
    ASSERT_TRUE(called);
}

TEST_F(FakeHandleFixture, on_empty) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->on<FakeEvent1>([](const auto &, auto &) {
        std::puts(__PRETTY_FUNCTION__);
    });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->publish(FakeEvent1{});
    ASSERT_FALSE(fakeHandle->empty());
}

TEST_F(FakeHandleFixture, on_clear) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->on<FakeEvent1>([](const auto &, auto &) { FAIL(); });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->clear();
    ASSERT_TRUE(fakeHandle->empty());
}
