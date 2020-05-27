#ifndef JGE_DETAIL_QUANTITY_WRAPPERS_HPP
#define JGE_DETAIL_QUANTITY_WRAPPERS_HPP

// Gelatin to keep formatting in its place.
#define JGE_IDENTITY(...) __VA_ARGS__

#include <compare>
#include <concepts>
#include <type_traits>

namespace jge::detail
{
template <std::regular Rep>
class [[nodiscard]] quantity_wrapper_common_base
{
public:
    using rep = Rep;

protected:
    Rep rep_;

public:
    quantity_wrapper_common_base() = default;

    template <class Rep2>
        requires std::convertible_to<const Rep2&, Rep>
    constexpr explicit quantity_wrapper_common_base(const Rep2& r) noexcept(
        std::is_nothrow_constructible_v<Rep, const Rep2&>)
      : rep_(r)
    {
    }

    [[nodiscard]] constexpr const Rep& operator()() const noexcept
    {
        return rep_;
    }
};

template <template <class> class Derived, std::regular Rep>
class [[nodiscard]] quantity_wrapper_common
  : public quantity_wrapper_common_base<Rep>
{
public:
    quantity_wrapper_common() = default;

    using quantity_wrapper_common_base<Rep>::quantity_wrapper_common_base;

    template <std::regular Rep2>
        requires std::convertible_to<const Rep2&, Rep>
    constexpr quantity_wrapper_common(const Derived<Rep2>& d) noexcept(
        std::is_nothrow_constructible_v<Rep, const Rep2&>)
      : quantity_wrapper_common_base<Rep>(d())
    {
    }

    friend constexpr Derived<Rep>&
    operator++(Derived<Rep>& p) noexcept(noexcept(++p.rep_)) requires requires
    {
        JGE_IDENTITY({ ++p.rep_ } -> std::same_as<Rep&>);
    }
    {
        ++p.rep_;
        return p;
    }

    friend constexpr Derived<Rep>&
    operator--(Derived<Rep>& p) noexcept(noexcept(--p.rep_)) requires requires
    {
        JGE_IDENTITY({ --p.rep_ } -> std::same_as<Rep&>);
    }
    {
        --p.rep_;
        return p;
    }

    [[nodiscard]] friend constexpr Derived<Rep>
    operator++(Derived<Rep>& p, int) noexcept(noexcept(Derived<Rep>{
        p.rep_++})) requires requires
    {
        JGE_IDENTITY({ p.rep_++ } -> std::same_as<Rep>);
    }
    {
        return Derived<Rep>{p.rep_++};
    }

    [[nodiscard]] friend constexpr Derived<Rep>
    operator--(Derived<Rep>& p, int) noexcept(noexcept(Derived<Rep>{
        p.rep_--})) requires requires
    {
        JGE_IDENTITY({ p.rep_-- } -> std::same_as<Rep>);
    }
    {
        return Derived<Rep>{p.rep_--};
    }

    template <std::equality_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto operator==(
        const Derived<Rep>& l,
        const Derived<Rep2>& r) noexcept(noexcept(l() == r()))
    {
        return l() == r();
    }

    template <std::three_way_comparable_with<Rep> Rep2>
    [[nodiscard]] friend constexpr auto operator<=>(
        const Derived<Rep>& l,
        const Derived<Rep2>& r) noexcept(noexcept(l() <=> r()))
    {
        return l() <=> r();
    }
};

template <template <class> class Kind, std::regular Rep>
class [[nodiscard]] quantity_kind : public quantity_wrapper_common<Kind, Rep>
{
public:
    using quantity_wrapper_common<Kind, Rep>::quantity_wrapper_common;

    [[nodiscard]] friend constexpr auto
    operator+(const Kind<Rep>& k) noexcept(noexcept(Kind<decltype(+k())>{
        +k()})) requires(requires { JGE_IDENTITY({ +k() } -> std::regular); })
    {
        return Kind<decltype(+k())>{+k()};
    }

    [[nodiscard]] friend constexpr auto
    operator-(const Kind<Rep>& k) noexcept(noexcept(Kind<decltype(-k())>{
        -k()})) requires(requires { JGE_IDENTITY({ -k() } -> std::regular); })
    {
        return Kind<decltype(-k())>{-k()};
    }

    template <std::regular Rep2>
    friend constexpr Kind<Rep>& operator+=(
        Kind<Rep>& l,
        const Kind<Rep2>& r) noexcept(noexcept(l.rep_ += r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ += r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ += r();
        return l;
    }

    template <std::regular Rep2>
    friend constexpr Kind<Rep>& operator-=(
        Kind<Rep>& l,
        const Kind<Rep2>& r) noexcept(noexcept(l.rep_ -= r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ -= r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ -= r();
        return l;
    }

    friend constexpr Kind<Rep>& operator*=(
        Kind<Rep>& l,
        const auto& r) noexcept(noexcept(l.rep_ *= r)) requires requires
    {
        JGE_IDENTITY({ l.rep_ *= r } -> std::same_as<Rep&>);
    }
    {
        l.rep_ *= r;
        return l;
    }

    friend constexpr Kind<Rep>& operator/=(
        Kind<Rep>& l,
        const auto& r) noexcept(noexcept(l.rep_ /= r)) requires requires
    {
        JGE_IDENTITY({ l.rep_ /= r } -> std::same_as<Rep&>);
    }
    {
        l.rep_ /= r;
        return l;
    }

    friend constexpr Kind<Rep>& operator%=(
        Kind<Rep>& l,
        const auto& r) noexcept(noexcept(l.rep_ %= r)) requires requires
    {
        JGE_IDENTITY({ l.rep_ %= r } -> std::same_as<Rep&>);
    }
    {
        l.rep_ %= r;
        return l;
    }

    template <std::regular Rep2>
    friend constexpr Kind<Rep>&
    operator%=(Kind<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(l %= r())) requires(requires { l %= r(); })
    {
        return l %= r();
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator+(const Kind<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(Kind<decltype(l() + r())>{l() + r()})) requires requires
    {
        JGE_IDENTITY({ l() + r() } -> std::regular);
    }
    {
        return Kind<decltype(l() + r())>{l() + r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator-(const Kind<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(Kind<decltype(l() - r())>{l() - r()})) requires requires
    {
        JGE_IDENTITY({ l() - r() } -> std::regular);
    }
    {
        return Kind<decltype(l() - r())>{l() - r()};
    }

    [[nodiscard]] friend constexpr auto
    operator*(const Kind<Rep>& l, const auto& r) noexcept(
        noexcept(Kind<decltype(l() * r)>{l() * r})) requires requires
    {
        JGE_IDENTITY({ l() * r } -> std::constructible_from<Rep>);
    }
    {
        return Kind<decltype(l() * r)>{l() * r};
    }

    [[nodiscard]] friend constexpr auto
    operator*(const auto& l, const Kind<Rep>& r) noexcept(
        noexcept(Kind<decltype(l * r())>{l * r()})) requires requires
    {
        JGE_IDENTITY({ l* r() } -> std::constructible_from<Rep>);
    }
    {
        return Kind<decltype(l * r())>{l * r()};
    }

    [[nodiscard]] friend constexpr auto
    operator/(const Kind<Rep>& l, const auto& r) noexcept(
        noexcept(Kind<decltype(l() / r)>{l() / r})) requires requires
    {
        JGE_IDENTITY({ l() / r } -> std::constructible_from<Rep>);
    }
    {
        return Kind<decltype(l() / r)>{l() / r};
    }

    template <std::regular Rep2>
        requires std::common_reference_with<const Rep&, const Rep2&>
    [[nodiscard]] friend constexpr auto operator/(
        const Kind<Rep>& l,
        const Kind<Rep2>& r) noexcept(noexcept(l() / r())) requires requires
    {
        JGE_IDENTITY({ l() / r() } -> std::regular);
    }
    {
        return l() / r();
    }

    [[nodiscard]] friend constexpr auto
    operator%(const Kind<Rep>& l, const auto& r) noexcept(
        noexcept(Kind<decltype(l() % r)>{l() % r})) requires requires
    {
        JGE_IDENTITY({ l() % r } -> std::constructible_from<Rep>);
    }
    {
        return Kind<decltype(l() % r)>{l() % r};
    }

    template <std::regular Rep2>
        requires std::common_reference_with<const Rep&, const Rep2&>
    [[nodiscard]] friend constexpr auto
    operator%(const Kind<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(Kind<decltype(l() % r())>{l() % r()})) requires requires
    {
        JGE_IDENTITY({ l() % r() } -> std::regular);
    }
    {
        return Kind<decltype(l() % r())>{l() % r()};
    }
};

} // namespace jge::detail

namespace jge
{
// Explicit point scaling

template <std::regular Rep>
class scale : public detail::quantity_kind<scale, Rep>
{
public:
    using detail::quantity_kind<scale, Rep>::quantity_kind;
};

template <std::regular Rep>
scale(Rep) -> scale<Rep>;

} // namespace jge

namespace jge::detail
{
template <
    template <class> class Point, template <class> class Kind, std::regular Rep>
class [[nodiscard]] quantity_point : public quantity_wrapper_common<Point, Rep>
{
public:
    using quantity_wrapper_common<Point, Rep>::quantity_wrapper_common;

    [[nodiscard]] friend constexpr auto
    operator+(const Point<Rep>& p) noexcept(noexcept(Point<decltype(+p())>{
        +p()})) requires(requires { JGE_IDENTITY({ +p() } -> std::regular); })
    {
        return Point<decltype(+p())>{+p()};
    }

    [[nodiscard]] friend constexpr auto
    operator-(const Point<Rep>& p) noexcept(noexcept(Point<decltype(-p())>{
        -p()})) requires(requires { JGE_IDENTITY({ -p() } -> std::regular); })
    {
        return Point<decltype(-p())>{-p()};
    }

    template <std::regular Rep2>
    friend constexpr Point<Rep>& operator+=(
        Point<Rep>& l,
        const Kind<Rep2>& r) noexcept(noexcept(l.rep_ += r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ += r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ += r();
        return l;
    }

    template <std::regular Rep2>
    friend constexpr Point<Rep>& operator-=(
        Point<Rep>& l,
        const Kind<Rep2>& r) noexcept(noexcept(l.rep_ -= r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ -= r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ -= r();
        return l;
    }

    template <std::regular Rep2>
    friend constexpr Point<Rep>&
    operator*=(Point<Rep>& l, const scale<Rep2>& r) noexcept(
        noexcept(l.rep_ *= r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ *= r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ *= r();
        return l;
    }

    template <std::regular Rep2>
    friend constexpr Point<Rep>&
    operator/=(Point<Rep>& l, const scale<Rep2>& r) noexcept(
        noexcept(l.rep_ /= r())) requires requires
    {
        JGE_IDENTITY({ l.rep_ /= r() } -> std::same_as<Rep&>);
    }
    {
        l.rep_ /= r();
        return l;
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator+(const Point<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(Point<decltype(l() + r())>{l() + r()})) requires requires
    {
        JGE_IDENTITY({ l() + r() } -> std::regular);
    }
    {
        return Point<decltype(l() + r())>{l() + r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator+(const Kind<Rep2>& l, const Point<Rep>& r) noexcept(
        noexcept(Point<decltype(l() + r())>{l() + r()})) requires requires
    {
        JGE_IDENTITY({ l() + r() } -> std::regular);
    }
    {
        return Point<decltype(l() + r())>{l() + r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator-(const Point<Rep>& l, const Kind<Rep2>& r) noexcept(
        noexcept(Point<decltype(l() - r())>{l() - r()})) requires requires
    {
        JGE_IDENTITY({ l() - r() } -> std::regular);
    }
    {
        return Point<decltype(l() - r())>{l() - r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator-(const Point<Rep>& l, const Point<Rep2>& r) noexcept(
        noexcept(Kind<decltype(l() - r())>{l() - r()})) requires requires
    {
        JGE_IDENTITY({ l() - r() } -> std::regular);
    }
    {
        return Kind<decltype(l() - r())>{l() - r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator*(const Point<Rep>& l, const scale<Rep2>& r) noexcept(
        noexcept(Point<decltype(l() * r())>{l() * r()})) requires requires
    {
        JGE_IDENTITY({ l() * r() } -> std::constructible_from<Rep>);
    }
    {
        return Point<decltype(l() * r())>{l() * r()};
    }

    template <std::regular Rep2>
    [[nodiscard]] friend constexpr auto
    operator*(const scale<Rep2>& l, const Point<Rep>& r) noexcept(
        noexcept(r* l)) requires(requires { r* l; })
    {
        return r * l;
    }

    template <std::regular Rep2>
        requires std::common_reference_with<const Rep&, const Rep2&>
    [[nodiscard]] friend constexpr auto
    operator/(const Point<Rep>& l, const scale<Rep2>& r) noexcept(
        noexcept(Point<decltype(l() / r())>{l() / r()})) requires requires
    {
        JGE_IDENTITY({ l() / r() } -> std::regular);
    }
    {
        return Point<decltype(l() / r())>{l() / r()};
    }
};

} // namespace jge::detail

namespace std
{
template <regular Rep1, common_with<Rep1> Rep2>
struct common_type<jge::scale<Rep1>, jge::scale<Rep2>>
{
    using type = jge::scale<common_type_t<Rep1, Rep2>>;
};

} // namespace std

#endif // JGE_DETAIL_QUANTITY_WRAPPERS_HPP
