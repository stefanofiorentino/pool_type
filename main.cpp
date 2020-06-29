#include <iostream>
#include <tuple>

template<typename E>
struct Connection {
};

template<typename E>
struct Listener {
};

template<typename T, typename ...Events>
struct Emitter {
    template<typename E>
    struct Handler {
        Connection<E> once(Listener<E> f) {
            return Connection<E>{};
        }
    };

    template<typename E>
    Connection<E> once(Listener<E> f) {
        return std::get<Handler<E> >(pools).once(std::move(f));
    }

    std::tuple<Handler<Events>... > pools;
};

template<typename T>
struct Emitter<T> {
};

struct FakeHandle {
};
struct FakeEvent1 {
};
struct FakeEvent2 {
};

template<typename T>
void pretty_print(T &&t) {
    std::puts(__PRETTY_FUNCTION__);
}

int main() {
    Emitter<FakeHandle> e0;

    Emitter<FakeHandle, FakeEvent1> e1;
    pretty_print(e1.once(Listener<FakeEvent1>{}));

    Emitter<FakeHandle, FakeEvent1, FakeEvent2> e2;
    pretty_print(e2.once<FakeEvent1>(Listener<FakeEvent1>{}));
    pretty_print(e2.once(Listener<FakeEvent2>{}));

    return 0;
}
