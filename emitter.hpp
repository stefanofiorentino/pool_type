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

template<typename T, typename E>
struct Emitter {
    struct Handler {
        using Listener = std::function<void(E &, T &)>;
        using ListenerList = std::list<Listener>;
        using Connection = typename ListenerList::iterator;

        Connection on(Listener f) noexcept {
            return onL.emplace(onL.cend(), std::move(f));
        }

        void publish(E event, T &ref) noexcept {
            std::for_each(onL.cbegin(), onL.cend(), [&event, &ref](auto &&element) {
                return element(event, ref);
            });
        }

        [[nodiscard]] bool empty() const noexcept {
            return onL.empty();
        }

        void clear() noexcept {
            onL.clear();
        }

    private:
        ListenerList onL{};
    };

    using Connection = typename Handler::Connection;
    using Listener = typename Handler::Listener;

    Connection on(Listener f) {
        return handler.on(std::move(f));
    }

    void publish(E event) {
        handler.publish(std::move(event), *static_cast<T *>(this));
    }

    [[nodiscard]] bool empty() const noexcept {
        return handler.empty();
    }

    void clear() noexcept {
        handler.clear();
    }

    Handler handler;
};


#endif //POOL_TYPE_EMITTER_HPP
