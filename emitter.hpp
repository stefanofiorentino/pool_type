#ifndef POOL_TYPE_EMITTER_HPP
#define POOL_TYPE_EMITTER_HPP

#include <iostream>
#include <tuple>
#include <list>
#include <functional>

template<typename T, typename ...Events>
struct Emitter {
    template<typename E>
    struct Handler {
        using Listener = std::function<void(E const&, T &)>;
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

    template <typename E>
    using Connection = typename Handler<E>::Connection;

    template<typename E> 
    using Listener = typename Handler<E>::Listener;

    template<typename E>
    Connection<E> on(Listener<E> f) {
        return std::get<Handler<E>>(pools).on(std::move(f));
    }

    template<typename E>
    void publish(E event) {
        std::get<Handler<E>>(pools).publish(std::move(event), *static_cast<T *>(this));
    }

    [[nodiscard]] bool empty() const noexcept {
        auto empty{true};
        std::apply([&empty](auto const&... args) {
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
