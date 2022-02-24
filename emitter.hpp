#ifndef POOL_TYPE_EMITTER_HPP
#define POOL_TYPE_EMITTER_HPP

#include <tuple>

template<typename T, typename Event1, typename Event2>
struct Emitter {
    template <typename E>
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

    template <typename E>
    using Connection = typename Handler<E>::Connection;
    template <typename E>
    using Listener = typename Handler<E>::Listener;

    template <typename E>
    Connection<E> on(Listener<E> f) {
        return std::get<Handler<E>>(handler).on(std::move(f));
    }

    template <typename E>
    void publish(E event) {
        std::get<Handler<E>>(handler).publish(std::move(event), *static_cast<T *>(this));
    }

    template <typename E>
    [[nodiscard]] bool empty() const noexcept {
        return std::get<Handler<E>>(handler).empty();
    }

    template <typename E>
    void clear() noexcept {
        std::get<Handler<E>>(handler).clear();
    }

    std::tuple<Handler<Event1>, Handler<Event2>> handler;
};


#endif //POOL_TYPE_EMITTER_HPP
