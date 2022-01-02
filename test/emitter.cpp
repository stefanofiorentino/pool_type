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

TEST_F(FakeHandleFixture, once_publish) {
    auto called{false};
    fakeHandle->once<FakeEvent1>([&called](const auto &, auto &) {
        called = true;
    });
    fakeHandle->publish(FakeEvent1{});
    ASSERT_TRUE(called);
}

TEST_F(FakeHandleFixture, once_empty) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->once<FakeEvent1>([](const auto &, auto &) {
        std::puts(__PRETTY_FUNCTION__);
    });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->publish(FakeEvent1{});
    ASSERT_TRUE(fakeHandle->empty());
}

TEST_F(FakeHandleFixture, once_clear) {
    ASSERT_TRUE(fakeHandle->empty());
    fakeHandle->once<FakeEvent1>([](const auto &, auto &) { FAIL(); });
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->clear();
    ASSERT_TRUE(fakeHandle->empty());
}

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

TEST_F(FakeHandleFixture, on_and_once) {
    ASSERT_TRUE(fakeHandle->empty());
    auto on_called{false};
    auto once_called{false};
    fakeHandle->on<FakeEvent1>([&on_called](const auto &, auto &) {
        ASSERT_FALSE(on_called);
        on_called = true;
    });
    fakeHandle->once<FakeEvent1>([&once_called](const auto &, auto &) {
        ASSERT_FALSE(once_called);
        once_called = true;
    });
    fakeHandle->publish(FakeEvent1{});
    ASSERT_TRUE(on_called);
    ASSERT_TRUE(once_called);
    ASSERT_FALSE(fakeHandle->empty());
    fakeHandle->clear();
    ASSERT_TRUE(fakeHandle->empty());
}
