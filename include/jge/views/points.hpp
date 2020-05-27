#ifndef JGE_VIEWS_POINTS_HPP
#define JGE_VIEWS_POINTS_HPP

#include <cassert>
#include <concepts>
#include <cstddef>
#include <ranges>
#include <type_traits>
#include <utility>
#include <jge/cartesian.hpp>
#include <range/v3/iterator/basic_iterator.hpp>

namespace jge
{
template <std::regular Rep>
class [[nodiscard]] points_view
  : public std::ranges::view_interface<points_view<Rep>>
{
    size2d<Rep> sz{};

public:
    points_view() = default;

    constexpr explicit points_view(const size2d<Rep> sz) noexcept(
        std::is_nothrow_copy_constructible_v<Rep>)
      : sz{sz}
    {
    }

    [[nodiscard]] constexpr auto begin() const
        noexcept(std::is_nothrow_copy_constructible_v<Rep>)
    {
        return iterator{sz.w};
    }

    [[nodiscard]] constexpr auto end() const
        noexcept(std::is_nothrow_copy_constructible_v<Rep>)
    {
        return iterator{{sz.w, to1d(sz)}};
    }

private:
    class cursor
    {
        std::ptrdiff_t w{};
        std::ptrdiff_t pt1d{};

    public:
        cursor() = default;

        constexpr cursor(const width<Rep> w, const Rep pt1d = {}) noexcept(
            noexcept(static_cast<std::ptrdiff_t>(pt1d / Rep{1})))
          : w{static_cast<std::ptrdiff_t>(w() / Rep{1})},
            pt1d{static_cast<std::ptrdiff_t>(pt1d / Rep{1})}
        {
        }

        constexpr point2d<Rep> read() const
            noexcept(std::is_nothrow_constructible_v<Rep, std::ptrdiff_t>)
        {
            assert(w != 0);
            return to2d(static_cast<Rep>(pt1d), width{static_cast<Rep>(w)});
        }

        constexpr void next() noexcept
        {
            ++pt1d;
        }

        constexpr void prev() noexcept
        {
            assert(pt1d != 0);
            --pt1d;
        }

        constexpr void advance(const std::ptrdiff_t n) noexcept
        {
            assert(0 <= pt1d + n && pt1d + n <= w);
            pt1d += n;
        }

        constexpr std::ptrdiff_t distance_to(const cursor other) const noexcept
        {
            assert(w == other.w);
            return other.pt1d - pt1d;
        }

        constexpr bool equal(const cursor other) const noexcept
        {
            assert(w == other.w);
            return pt1d == other.pt1d;
        }
    };

    using iterator = ranges::basic_iterator<cursor>;
};

namespace views
{
    inline constexpr auto points =
        []<class T>(T&& sz) noexcept(noexcept(points_view{std::forward<T>(
            sz)})) -> decltype(points_view{std::forward<T>(sz)}) {
        return points_view{std::forward<T>(sz)};
    };

} // namespace views

} // namespace jge

#endif // JGE_VIEWS_POINTS_HPP
