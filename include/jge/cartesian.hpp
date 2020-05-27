#ifndef JGE_CARTESIAN_HPP
#define JGE_CARTESIAN_HPP

#include <cassert>
#include <concepts>
#include <type_traits>
#include <jge/detail/quantity_wrappers.hpp>
#include <units/quantity.h>

namespace jge
{
// Dimensions

template <std::regular Rep>
class [[nodiscard]] width : public detail::quantity_kind<width, Rep>
{
public:
    using detail::quantity_kind<width, Rep>::quantity_kind;
};

template <std::regular Rep>
width(Rep) -> width<Rep>;

template <std::regular Rep>
class [[nodiscard]] height : public detail::quantity_kind<height, Rep>
{
public:
    using detail::quantity_kind<height, Rep>::quantity_kind;
};

template <std::regular Rep>
height(Rep) -> height<Rep>;

template <std::regular Rep>
class [[nodiscard]] depth : public detail::quantity_kind<depth, Rep>
{
public:
    using detail::quantity_kind<depth, Rep>::quantity_kind;
};

template <std::regular Rep>
depth(Rep) -> depth<Rep>;

// Sizes

template <std::regular Rep>
struct [[nodiscard]] size2d
{
    using rep = Rep;

    width<Rep> w;
    height<Rep> h;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep&, Rep2>
    constexpr operator size2d<Rep2>() const
        noexcept(std::is_nothrow_constructible_v<Rep2, const Rep&>)
    {
        return {width<Rep2>(w), height<Rep2>(h)};
    }

    friend constexpr size2d& operator*=(size2d& l, const auto& r) noexcept(
        noexcept(l.w *= r)) requires(requires { l.w *= r; })
    {
        l.w *= r;
        l.h *= r;
        return l;
    }

    friend constexpr size2d& operator/=(size2d& l, const auto& r) noexcept(
        noexcept(l.w /= r)) requires(requires { l.w /= r; })
    {
        l.w /= r;
        l.h /= r;
        return l;
    }

    friend constexpr auto operator*(const size2d& l, const auto& r) noexcept(
        noexcept(l.w* r)) requires(requires { l.w* r; })
    {
        return size2d<decltype(l.w() * r)>{l.w * r, l.h * r};
    }

    template <class D, class U, class Rep2>
    friend constexpr auto
    operator*(const size2d& l, const units::quantity<D, U, Rep2>& r) noexcept(
        noexcept(l.w* r)) requires(requires { l.w* r; })
    {
        return size2d<decltype(l.w() * r)>{l.w * r, l.h * r};
    }

    friend constexpr auto operator*(const auto& l, const size2d& r) noexcept(
        noexcept(l* r.w)) requires(requires { l* r.w; })
    {
        return size2d<decltype(l * r.w())>{l * r.w, l * r.h};
    }

    template <class D, class U, class Rep2>
    friend constexpr auto operator*(
        const units::quantity<D, U, Rep2>& l,
        const size2d& r) noexcept(noexcept(r* l)) requires(requires { r* l; })
    {
        return r * l;
    }

    friend constexpr auto operator/(const size2d& l, const auto& r) noexcept(
        noexcept(l.w / r)) requires(requires { l.w / r; })
    {
        return size2d<decltype(l.w() / r)>{l.w / r, l.h / r};
    }

    template <class D, class U, class Rep2>
    friend constexpr auto
    operator/(const size2d& l, const units::quantity<D, U, Rep2>& r) noexcept(
        noexcept(l.w / r)) requires(requires { l.w / r; })
    {
        return jge::size2d{width{l.w() / r}, height{l.h() / r}};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto operator==(
        const size2d& l,
        const size2d<Rep2>& r) noexcept(noexcept(l.w == r.w && l.h == r.h))
    {
        return l.w == r.w && l.h == r.h;
    }
};

template <std::regular Rep>
constexpr auto operator+(const width<Rep>& w, const height<Rep>& h) noexcept(
    std::is_nothrow_copy_constructible_v<Rep>)
{
    return size2d{w, h};
}

template <std::regular Rep>
struct [[nodiscard]] size3d
{
    using rep = Rep;

    width<Rep> w;
    height<Rep> h;
    depth<Rep> d;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep&, Rep2>
    constexpr operator size3d<Rep2>() const
        noexcept(std::is_nothrow_constructible_v<Rep2, const Rep&>)
    {
        return {width<Rep2>(w), height<Rep2>(h), depth<Rep2>(d)};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto
    operator==(const size3d& l, const size3d<Rep2>& r) noexcept(
        noexcept(l.w == r.w && l.h == r.h && l.d == r.d))
    {
        return l.w == r.w && l.h == r.h && l.d == r.d;
    }
};

// Coordinates

template <std::regular Rep>
class [[nodiscard]] abscissa
  : public detail::quantity_point<abscissa, width, Rep>
{
public:
    using detail::quantity_point<abscissa, width, Rep>::quantity_point;
};

template <std::regular Rep>
abscissa(Rep) -> abscissa<Rep>;

template <std::regular Rep>
class [[nodiscard]] ordinate
  : public detail::quantity_point<ordinate, height, Rep>
{
public:
    using detail::quantity_point<ordinate, height, Rep>::quantity_point;
};

template <std::regular Rep>
ordinate(Rep) -> ordinate<Rep>;

template <std::regular Rep>
class [[nodiscard]] applicate
  : public detail::quantity_point<applicate, depth, Rep>
{
public:
    using detail::quantity_point<applicate, depth, Rep>::quantity_point;
};

template <std::regular Rep>
applicate(Rep) -> applicate<Rep>;

// Points

template <std::regular Rep>
struct [[nodiscard]] point2d
{
    using rep = Rep;

    abscissa<Rep> x;
    ordinate<Rep> y;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep&, Rep2>
    constexpr operator point2d<Rep2>() const
        noexcept(std::is_nothrow_constructible_v<Rep2, const Rep&>)
    {
        return {abscissa<Rep2>(x), ordinate<Rep2>(y)};
    }

    template <std::regular Rep2>
    friend constexpr point2d&
    operator*=(point2d& l, const scale<Rep2>& r) noexcept(
        noexcept(l.x *= r)) requires(requires { l.x *= r; })
    {
        l.x *= r;
        l.y *= r;
        return l;
    }

    template <std::regular Rep2>
    friend constexpr point2d&
    operator/=(point2d& l, const scale<Rep2>& r) noexcept(
        noexcept(l.x /= r)) requires(requires { l.x /= r; })
    {
        l.x /= r;
        l.y /= r;
        return l;
    }

    template <std::regular Rep2>
    friend constexpr auto
    operator*(const point2d& l, const scale<Rep2>& r) noexcept(
        noexcept(l.x* r)) requires(requires { l.x* r; })
    {
        return point2d<decltype(l.x() * r())>{l.x * r, l.y * r};
    }

    template <std::regular Rep2>
    friend constexpr auto operator*(
        const scale<Rep2>& l,
        const point2d& r) noexcept(noexcept(r* l)) requires(requires { r* l; })
    {
        return r * l;
    }

    template <std::regular Rep2>
    friend constexpr auto
    operator/(const point2d& l, const scale<Rep2>& r) noexcept(
        noexcept(l.x / r)) requires(requires { l.x / r; })
    {
        return point2d<decltype(l.x() / r())>{l.x / r, l.y / r};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto operator==(
        const point2d& l,
        const point2d<Rep2>& r) noexcept(noexcept(l.x == r.x && l.y == r.y))
    {
        return l.x == r.x && l.y == r.y;
    }
};

template <std::regular Rep>
constexpr auto operator+(
    const abscissa<Rep>& x,
    const ordinate<Rep>& y) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
{
    return point2d{x, y};
}

template <std::regular Rep>
struct [[nodiscard]] point3d
{
    using rep = Rep;

    abscissa<Rep> x;
    ordinate<Rep> y;
    applicate<Rep> z;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep&, Rep2>
    constexpr operator point3d<Rep2>() const
        noexcept(std::is_nothrow_constructible_v<Rep2, const Rep&>)
    {
        return {abscissa<Rep2>(x), ordinate<Rep2>(y), applicate<Rep2>(z)};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto
    operator==(const point3d& l, const point3d<Rep2>& r) noexcept(
        noexcept(l.x == r.x && l.y == r.y && l.z == r.z))
    {
        return l.x == r.x && l.y == r.y && l.z == r.z;
    }
};

template <std::regular Rep>
[[nodiscard]] constexpr bool contains(
    const size2d<Rep> sz,
    const point2d<Rep> pt) noexcept(std::is_arithmetic_v<Rep>)
{
    return Rep{0} <= pt.x() && pt.x() < sz.w() && //
           Rep{0} <= pt.y() && pt.y() < sz.h();
}

// 2d <-> 1d array mapping

template <std::regular Rep>
[[nodiscard]] constexpr auto
to1d(const size2d<Rep> sz) noexcept(noexcept(sz.w() * sz.h() / Rep{1}))
{
    assert(sz.w() >= Rep{0} && sz.h() >= Rep{0});
    return sz.w() * sz.h() / Rep{1};
}

template <std::regular Rep>
[[nodiscard]] constexpr auto to1d(
    const point2d<Rep> pt,
    const size2d<Rep> sz) noexcept(noexcept(pt.y() * sz.w() + pt.x()))
{
    assert(sz.w() > Rep{0} && sz.h() > Rep{0});
    assert(Rep{0} <= pt.x() && pt.x() < sz.w());
    assert(Rep{0} <= pt.y() && pt.y() < sz.h());
    return pt.y() * sz.w() + pt.x();
}

template <std::regular Rep>
constexpr auto
to2d(const Rep pt1d, const width<Rep> w) noexcept(std::is_arithmetic_v<Rep>)
{
    assert(w() != Rep{0});
    return point2d{abscissa{pt1d % w()}, ordinate{pt1d / w() * Rep{1}}};
}

template <std::regular Rep>
constexpr auto
to_size(const Rep sz1d, const width<Rep> w) noexcept(std::is_arithmetic_v<Rep>)
{
    assert(
        (sz1d == Rep{0} && w() == Rep{0}) ||
        (sz1d != Rep{0} && w() != Rep{0} && sz1d % w() == Rep{0}));
    return size2d{w, height{w() == Rep{0} ? Rep{0} : sz1d / w()}};
}

// Subplane

template <std::regular Rep>
struct [[nodiscard]] subplane
{
    using rep = Rep;

    point2d<Rep> top_left;
    size2d<Rep> size;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep&, Rep2>
    constexpr operator subplane<Rep2>() const
        noexcept(std::is_nothrow_constructible_v<Rep2, const Rep&>)
    {
        return {point2d<Rep2>(top_left), size2d<Rep2>(size)};
    }

    constexpr point2d<Rep> bottom_right() const
        noexcept(std::is_arithmetic_v<Rep>)
    {
        return {top_left.x + size.w, top_left.y + size.h};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto
    operator==(const subplane& l, const subplane<Rep2>& r) noexcept(
        noexcept(l.top_left == r.top_left && l.size == r.size))
    {
        return l.top_left == r.top_left && l.size == r.size;
    }
};

template <std::regular Rep>
[[nodiscard]] constexpr bool contains(
    const subplane<Rep> l,
    const point2d<Rep> r) noexcept(std::is_arithmetic_v<Rep>)
{
    return l.top_left.x <= r.x && r.x < l.bottom_right().x &&
           l.top_left.y <= r.y && r.y < l.bottom_right().y;
}

template <std::regular Rep>
[[nodiscard]] constexpr bool contains(
    const subplane<Rep> l,
    const subplane<Rep> r) noexcept(std::is_arithmetic_v<Rep>)
{
    return l.top_left.x <= r.top_left.x && //
           l.top_left.y <= r.top_left.y &&
           r.bottom_right().x <= l.bottom_right().x &&
           r.bottom_right().y <= l.bottom_right().y;
}

template <std::regular Rep>
[[nodiscard]] constexpr bool contains(
    const size2d<Rep> l,
    const subplane<Rep> r) noexcept(std::is_arithmetic_v<Rep>)
{
    return contains({{}, l}, r);
}

} // namespace jge

namespace std
{
template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::abscissa<Rep1>, jge::abscissa<Rep2>>
{
    using type = jge::abscissa<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::ordinate<Rep1>, jge::ordinate<Rep2>>
{
    using type = jge::ordinate<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::applicate<Rep1>, jge::applicate<Rep2>>
{
    using type = jge::applicate<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::point2d<Rep1>, jge::point2d<Rep2>>
{
    using type = jge::point2d<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::point3d<Rep1>, jge::point3d<Rep2>>
{
    using type = jge::point3d<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::width<Rep1>, jge::width<Rep2>>
{
    using type = jge::width<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::height<Rep1>, jge::height<Rep2>>
{
    using type = jge::height<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::depth<Rep1>, jge::depth<Rep2>>
{
    using type = jge::depth<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::size2d<Rep1>, jge::size2d<Rep2>>
{
    using type = jge::size2d<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::size3d<Rep1>, jge::size3d<Rep2>>
{
    using type = jge::size3d<common_type_t<Rep1, Rep2>>;
};

template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::subplane<Rep1>, jge::subplane<Rep2>>
{
    using type = jge::subplane<common_type_t<Rep1, Rep2>>;
};

} // namespace std

#endif // JGE_CARTESIAN_HPP
