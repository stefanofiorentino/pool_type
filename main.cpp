#include <iostream>
#include <tuple>
#include <list>
#include <functional>

template<typename ...T>
void pretty_print(T &&...t) {
    std::puts(__PRETTY_FUNCTION__);
}


template<typename T, typename ...Events>
struct Emitter {
    template<typename E>
    struct Handler {
        using Listener = std::function<void(E &, T &)>;
        using Element = std::pair<bool, Listener>;
        using ListenerList = std::list<Element>;
        using Connection = typename ListenerList::iterator;

        Connection once(Listener f) {
            return onceL.emplace(onceL.cend(), false, std::move(f));
        }


        void publish(E event, T &ref) {
            ListenerList currentL;
            onceL.swap(currentL);

            auto func = [&event, &ref](auto &&element) {
                return element.first ? void() : element.second(event, ref);
            };

            publishing = true;

            std::for_each(currentL.rbegin(), currentL.rend(), func);

            publishing = false;
        }

        bool empty() {
            return true;
        }
    private:
        bool publishing{false};
        ListenerList onceL{};
    };

    template<typename E>
    struct Connection: private Handler<E>::Connection {
        template<typename, typename ...> friend class Emitter;

        Connection() = default;
        Connection(const Connection &) = default;
        Connection(Connection &&) = default;
        ~Connection() = default;

        Connection(typename Handler<E>::Connection conn)
        : Handler<E>::Connection{std::move(conn)}
        {}

        Connection & operator=(const Connection &) = default;
        Connection & operator=(Connection &&) = default;
    };

    template<typename E>
    using Listener = typename Handler<E>::Listener;

    template<typename E>
    Connection<E> once(Listener<E> f) {
        return std::get<Handler<E> >(pools).once(std::move(f));
    }

    template<typename E>
    void publish(E event) {
        pretty_print(event);
        std::get<Handler<E>>(pools).publish(std::move(event), *static_cast<T*>(this));
    }

    bool empty() const noexcept {
        auto empty{true};
        std::apply([&empty](auto&&... args) {((empty &= args.empty()), ...);}, pools);
        return empty;
    }

    std::tuple<Handler<Events>...> pools;
};

struct FakeEvent1 {
};
struct FakeEvent2 {
};

struct FakeHandle : public Emitter<FakeHandle, FakeEvent1, FakeEvent2> {
};

int main() {
    Emitter<FakeHandle, FakeEvent1, FakeEvent2> e2;
    auto connection = e2.once<FakeEvent2>([](const auto &, auto &) {
        std::puts(__PRETTY_FUNCTION__ );
    });
    e2.publish(FakeEvent2{});
    return 0;
}
