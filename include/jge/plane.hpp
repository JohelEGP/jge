#ifndef JGE_PLANE_HPP
#define JGE_PLANE_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>
#include <jge/cartesian.hpp>
#include <jge/detail/memory.hpp>
#include <lift.hpp>

namespace jge
{
struct [[nodiscard]] default_initialize_t
{
    explicit default_initialize_t() = default;
};

inline constexpr default_initialize_t default_initialize{};

struct [[nodiscard]] value_initialize_t
{
    explicit value_initialize_t() = default;
};

inline constexpr value_initialize_t value_initialize{};

template <class T>
class [[nodiscard]] plane
{
public:
    using width_type = width<std::size_t>;
    using size_type  = size2d<width_type::rep>;
    using point_type = point2d<size_type::rep>;

private:
    T* data{};
    size_type sz{};

public:
    constexpr ~plane() requires std::destructible<T>
    {
        delete[] data;
    }

    plane() = default;

    constexpr plane(const plane& other) noexcept(
        (detail::default_allocator_is_nothrow &&
         std::is_nothrow_copy_constructible_v<T>)) requires std::copyable<T>
      : plane{to1d(other), other.sz.w}
    {
    }

    constexpr plane& operator=(const plane& other) noexcept(
        (std::is_nothrow_copy_constructible_v<plane> &&
         std::is_nothrow_copy_assignable_v<T>)) requires std::copyable<T>
    {
        assign(to1d(other), other.sz.w);
        return *this;
    }

    constexpr plane(plane&& other) noexcept
      : data{std::exchange(other.data, nullptr)},
        sz{std::exchange(other.sz, {})}
    {
    }

    constexpr plane& operator=(plane&& other) noexcept
    {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        return *this;
    }

    constexpr explicit plane(const size_type sz, default_initialize_t) noexcept(
        (detail::default_allocator_is_nothrow &&
         std::is_nothrow_default_constructible_v<T>)) requires
        std::default_initializable<T>
      : data{to1d(sz) == 0 ? nullptr : new T[to1d(sz)]}, sz{sz}
    {
        assert(sz == size_type{} || to1d(sz) != 0);
    }

    constexpr explicit plane(const size_type sz, value_initialize_t) noexcept(
        (detail::default_allocator_is_nothrow &&
         std::is_nothrow_default_constructible_v<T>)) requires
        std::default_initializable<T>
      : data{to1d(sz) == 0 ? nullptr : std::allocator<T>{}.allocate(to1d(sz))},
        sz{sz}
    {
        assert(sz == size_type{} || to1d(sz) != 0);
        detail::uninitialized_value_construct(to1d(*this));
    }

    constexpr plane(const std::initializer_list<std::initializer_list<T>> il2d) noexcept(
        (detail::default_allocator_is_nothrow &&
         std::is_nothrow_copy_constructible_v<T>)) requires std::copyable<T>
    {
        *this = il2d;
    }

    constexpr plane&
    operator=(const std::initializer_list<std::initializer_list<T>> il2d) noexcept(
        (std::is_nothrow_constructible_v<plane, decltype(il2d)> &&
         std::is_nothrow_copy_assignable_v<T>)) requires std::copyable<T>
    {
        const width_type w{empty(il2d) ? 0 : il2d.begin()->size()};
        assert(std::ranges::all_of(il2d, lift::equal(w()), std::ranges::size));
        const std::ranges::join_view il1d{il2d};
        assign(std::ranges::subrange{il1d, il2d.size() * w()}, w);
        return *this;
    }

    constexpr plane(const std::initializer_list<T> il, const width_type w) noexcept(
        (detail::default_allocator_is_nothrow &&
         std::is_nothrow_copy_constructible_v<T>)) requires std::copyable<T>
      : plane{std::span{il}, w}
    {
    }

    constexpr void
    assign(const std::initializer_list<T> il, const width_type w) noexcept(
        (std::is_nothrow_constructible_v<plane, decltype(il), width_type> &&
         std::is_nothrow_copy_assignable_v<T>)) requires std::copyable<T>
    {
        assign(std::span{il}, w);
    }

    template <std::ranges::sized_range R>
        requires std::ranges::input_range<R> &&
            std::constructible_from<T, std::ranges::range_reference_t<R>>
    constexpr plane(R&& r, const width_type w)
      : data{w() == 0 ? nullptr : std::allocator<T>{}.allocate(std::ranges::size(r))},
        sz{to_size(std::ranges::size(r), w)}
    {
        detail::uninitialized_copy(std::forward<R>(r), to1d(*this));
    }

    template <class R>
        requires std::constructible_from<plane, R, width_type>
    constexpr void assign(R&& r, const width_type w)
    {
        if (to1d(sz) != std::ranges::size(r))
        {
            delete[] data;
            *this = plane{std::forward<R>(r), w};
            return;
        }

        sz = to_size(std::ranges::size(r), w);
        std::ranges::copy(std::forward<R>(r), data);
    }

    [[nodiscard]] constexpr T& operator[](const point_type pt) noexcept
    {
        return const_cast<T&>(std::as_const(*this)[pt]);
    }

    [[nodiscard]] constexpr const T&
    operator[](const point_type pt) const noexcept
    {
        assert(contains(sz, pt));
        return data[to1d(pt, sz)];
    }

    constexpr size_type size() const noexcept
    {
        return sz;
    }

    [[nodiscard]] constexpr bool operator==(
        const plane& other) const noexcept requires std::equality_comparable<T>
    {
        constexpr auto gcc95806 = std::views::transform(std::identity{});
        return sz == other.sz &&
               std::ranges::equal(to1d(*this) | gcc95806, to1d(other));
    }

    [[nodiscard]] friend constexpr std::span<T> to1d(plane& p) noexcept
    {
        return {p.data, to1d(p.sz)};
    }

    [[nodiscard]] friend constexpr std::span<const T>
    to1d(const plane& p) noexcept
    {
        return to1d(const_cast<plane&>(p));
    }
};

template <class R>
plane(R&&, typename plane<std::ranges::range_value_t<R>>::width_type)
    -> plane<std::ranges::range_value_t<R>>;

} // namespace jge

#endif // JGE_PLANE_HPP
