/**
* @ProjectName   WorldEngine
* @Date          2020-06-20
* @FilaeName     %{CurrentFile:Name}
* @Description   Simulation the std::optional(C++17) in C++11.
*/

#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <exception>
#include <utility>
#include <type_traits>

class EBadOptionalAccess : public std::exception
{
};

struct NullOptional
{
};
constexpr NullOptional null_optional;

template <class T>
class Optional
{
    template <class U>
    struct optional_construct_helper
    {
        constexpr static bool value =
            !std::is_constructible<T, Optional<U> &>::value &&
            !std::is_constructible<T, const Optional<U> &>::value &&
            !std::is_constructible<T, Optional<U> &&>::value &&
            !std::is_constructible<T, const Optional<U> &&>::value &&
            !std::is_convertible<Optional<U> &, T>::value &&
            !std::is_convertible<const Optional<U> &, T>::value &&
            !std::is_convertible<Optional<U> &&, T>::value &&
            !std::is_convertible<const Optional<U> &&, T>::value;
    };

    bool has_value;
    T value;

public:
    constexpr Optional() noexcept
        : has_value{false},
          value{}
    {
    }

    template <class U,
              typename std::enable_if<
                  std::is_constructible<T, const U &>::value &&
                      Optional<T>::optional_construct_helper<U>::value,
                  int>::type = 0>
    Optional(const Optional<U> &other)
        : has_value{other.hasValue()},
          value{static_cast<T>(*other)}
    {
    }

    template <class U,
              typename std::enable_if<
                  std::is_constructible<T, const U &>::value &&
                      Optional<T>::optional_construct_helper<U>::value,
                  int>::type = 0>
    Optional(Optional<U> &&other)
        : has_value{other.hasValue()},
          value{static_cast<T>(std::move(*other))}
    {
    }

    template <class U = T,
              typename std::enable_if<
                  !std::is_same<typename std::decay<U>::type, NullOptional>::value &&
                      std::is_constructible<T, U &&>::value &&
                      !std::is_same<typename std::decay<U>::type, Optional<T>>::value,
                  int>::type = 0>
    constexpr Optional(U &&_value)
        : value{static_cast<T>(std::forward<U>(_value))},
          has_value{true}
    {
    }

    template <class U = T,
              typename std::enable_if<
                  std::is_same<typename std::decay<U>::type, NullOptional>::value,
                  int>::type = 0>
    constexpr Optional(U &&)
        : value{},
          has_value{false}
    {
    }

    template <class U = T,
              typename std::enable_if<
                  !std::is_same<typename std::decay<U>::type, NullOptional>::value &&
                      std::is_constructible<T, U &&>::value &&
                      !std::is_same<typename std::decay<U>::type, Optional<T>>::value,
                  int>::type = 0>
    Optional<T> &operator=(U &&_value)
    {
        this->reset(_value);
        return *this;
    }

    template <class U = T,
              typename std::enable_if<
                  std::is_same<typename std::decay<U>::type, NullOptional>::value,
                  int>::type = 0>
    Optional<T> &operator=(U &&)
    {
        this->reset();
        return *this;
    }

    template <class U,
              typename std::enable_if<
                  std::is_constructible<T, const U &>::value &&
                      Optional<T>::optional_construct_helper<U>::value,
                  int>::type = 0>
    Optional<T> &operator=(const Optional<U> &other)
    {
        value = static_cast<T>(*other);
        has_value = other.hasValue();
        return *this;
    }

    template <class U,
              typename std::enable_if<
                  std::is_constructible<T, const U &>::value &&
                      Optional<T>::optional_construct_helper<U>::value,
                  int>::type = 0>
    Optional<T> &operator=(Optional<U> &&other)
    {
        has_value = other.hasValue(),
        value = static_cast<T>(std::move(*other));
        return *this;
    }

    ~Optional() = default;

    constexpr bool hasValue() const noexcept
    {
        return has_value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return hasValue();
    }

    inline const T *operator->() const
    {
        return &value;
    }

    inline T *operator->()
    {
        return &value;
    }

    inline const T &operator*() const &
    {
        return value;
    }

    inline T &operator*() &
    {
        return value;
    }

    inline const T &&operator*() const &&
    {
        return std::move(**this);
    }

    inline T &&operator*() &&
    {
        return std::move(**this);
    }

    inline T &getValue() &
    {
        if (has_value)
            return value;
        else
            throw EBadOptionalAccess{};
    }

    constexpr const T &getValue() const &
    {
        return getValue();
    }

    inline T &&getValue() &&
    {
        if (has_value)
            return std::move(**this);
        else
            throw EBadOptionalAccess{};
    }

    constexpr const T &&getValue() const &&
    {
        return getValue();
    }

    template <class U>
    constexpr T getValueOR(U &&defaultValue) const &
    {
        return bool(*this) ? **this : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <class U>
    constexpr T getValueOR(U &&defaultValue) &&
    {
        return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(defaultValue));
    }

    inline void reset() noexcept
    {
        has_value = false;
        value.T::~T();
    }

    template <class U = T,
              typename std::enable_if<
                  std::is_constructible<T, U &&>::value &&
                      !std::is_same<typename std::decay<U>::type, Optional<T>>::value,
                  int>::type = 0>
    void reset(U &&_value) noexcept
    {
        has_value = true;
        value = static_cast<T>(std::forward<U>(_value));
    }
};

#endif // OPTIONAL_H
