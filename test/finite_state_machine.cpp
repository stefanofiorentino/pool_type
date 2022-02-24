#include <gmock/gmock.h>
#include <emitter.hpp>

struct state_1 {
};

struct state_2 {
};

struct state_3 {
};

struct Handle : Emitter<Handle, state_1, state_2, state_3> {

};


TEST(emitter, transition) {
    auto called{false};
    Handle h;
    h.on<state_1>([](auto &e, auto&h){
        h.publish(state_2{});
    });
    h.on<state_2>([](auto &e, auto&h){
        h.publish(state_3{});
    });
    h.on<state_3>([&called](auto &e, auto&h){
        called = true;
    });
    h.publish(state_1{});
    ASSERT_TRUE(called);
}

bool finished()
{
    static uint8_t counter = 0;
    return counter++ > 10;
}

struct start{};
struct working{};
struct finish{};

struct ProcessHandle : Emitter<ProcessHandle, start, working, finish> {};

TEST(emitter, process_manager) {
    auto called{false};
    ProcessHandle ph;
    ph.on<start>([](auto &e, auto&h){
        h.publish(working{});
    });
    ph.on<working>([](auto &e, auto&h){
        while (!finished())
            h.publish(working{});
        h.publish(finish{});
    });
    ph.on<finish>([&called](auto &e, auto&h){
        called = true;
    });
    ph.publish(start{});
    ASSERT_TRUE(called);
}
