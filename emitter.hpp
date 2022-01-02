#ifndef POOL_TYPE_EMITTER_HPP
#define POOL_TYPE_EMITTER_HPP

#include <iostream>
#include <tuple>
#include <list>
#include <functional>

#include "defines.h"

template<typename ...T>
inline void pretty_print(T &&...) {
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

        Handler() = default;

        Connection once(Listener f) {
            return onceL.emplace(onceL.cend(), false, std::move(f));
        }

        Connection on(Listener f) {
            return onL.emplace(onL.cend(), false, std::move(f));
        }

        void publish(E event, T &ref) {
            ListenerList currentL;
            onceL.swap(currentL);

            auto func = [&event, &ref](auto &&element) {
                return element.first ? void() : element.second(event, ref);
            };

            std::for_each(currentL.rbegin(), currentL.rend(), func);
            std::for_each(onL.cbegin(), onL.cend(), func);
        }

        [[nodiscard]] bool empty() const noexcept {
            return onceL.empty() && onL.empty();
        }

        void clear() noexcept {
            onceL.clear();
            onL.clear();
        }

    private:
        ListenerList onceL{};
        ListenerList onL{};
    };

    template <typename E>
    using Connection = typename Handler<E>::Connection;

    template<typename E> 
    using Listener = typename Handler<E>::Listener;

    template<typename E>
    Connection<E> once(Listener<E> f) {
        return std::get<Handler<E> >(pools).once(std::move(f));
    }

    template<typename E>
    Connection<E> on(Listener<E> f) {
        return std::get<Handler<E> >(pools).on(std::move(f));
    }

    template<typename E>
    void publish(E event) {
        pretty_print(event);
        std::get<Handler<E>>(pools).publish(std::move(event), *static_cast<T *>(this));
    }

    [[nodiscard]] bool empty() const noexcept {
        auto empty{true};
        std::apply([&empty](auto &&... args) {
            ((empty &= args.empty()), ...);
        }, pools);
        return empty;
    }

    void clear() noexcept {
        std::apply([](auto &&... args) {
            ((args.clear()), ...);
        }, pools);
    }

    std::tuple<Handler<Events>...> pools;
};


#endif //POOL_TYPE_EMITTER_HPP
