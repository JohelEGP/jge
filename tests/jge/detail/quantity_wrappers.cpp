#include <cassert>
#include <chrono>
#include <concepts>
#include <functional>
#include <span>
#include <type_traits>
//#include <boost/mp11/utility.hpp>
#include <jge/cartesian.hpp>
#include <jge/pixels.hpp>
#include <units/physical/si/area.h>
#include <units/physical/si/length.h>

using namespace std;
// using namespace boost::mp11;
using namespace jge;

static_assert(regular<width<int>>);
static_assert(regular<width<pixels<int>>>);
static_assert(regular<abscissa<int>>);
static_assert(regular<abscissa<pixels<int>>>);
static_assert(regular<scale<int>>);
static_assert(regular<scale<pixels<int>>>);
static_assert(totally_ordered_with<width<int>, width<long>>);
static_assert(totally_ordered_with<width<pixels<int>>, width<pixels<long>>>);
static_assert(totally_ordered_with<abscissa<int>, abscissa<long>>);
static_assert(
    totally_ordered_with<abscissa<pixels<int>>, abscissa<pixels<long>>>);
static_assert(totally_ordered_with<scale<int>, scale<long>>);
static_assert(totally_ordered_with<scale<pixels<int>>, scale<pixels<long>>>);
// static_assert(!mp_valid<width, span<int>>{});

static_assert(is_trivially_destructible_v</*         */ width<int>>);
static_assert(is_trivially_default_constructible_v</**/ width<int>>);
static_assert(is_trivially_copy_constructible_v</*   */ width<int>>);
static_assert(is_trivially_copy_assignable_v</*      */ width<int>>);
static_assert(is_trivially_move_constructible_v</*   */ width<int>>);
static_assert(is_trivially_move_assignable_v</*      */ width<int>>);
static_assert(is_trivially_destructible_v</*         */ abscissa<int>>);
static_assert(is_trivially_default_constructible_v</**/ abscissa<int>>);
static_assert(is_trivially_copy_constructible_v</*   */ abscissa<int>>);
static_assert(is_trivially_copy_assignable_v</*      */ abscissa<int>>);
static_assert(is_trivially_move_constructible_v</*   */ abscissa<int>>);
static_assert(is_trivially_move_assignable_v</*      */ abscissa<int>>);
static_assert(is_trivially_destructible_v</*         */ scale<int>>);
static_assert(is_trivially_default_constructible_v</**/ scale<int>>);
static_assert(is_trivially_copy_constructible_v</*   */ scale<int>>);
static_assert(is_trivially_copy_assignable_v</*      */ scale<int>>);
static_assert(is_trivially_move_constructible_v</*   */ scale<int>>);
static_assert(is_trivially_move_assignable_v</*      */ scale<int>>);

struct nothrow_rep
{
    nothrow_rep()                   = default;
    nothrow_rep(const nothrow_rep&) = default;
    nothrow_rep& operator=(const nothrow_rep&) = default;
    nothrow_rep(nothrow_rep&&)                 = default;
    nothrow_rep& operator=(nothrow_rep&&)     = default;
    bool operator==(const nothrow_rep&) const = default;
};

struct yesthrow_rep
{
    yesthrow_rep()
    {
    }
    yesthrow_rep(const yesthrow_rep&) noexcept(false) = default;
    yesthrow_rep& operator                            =(const yesthrow_rep&)
    {
        return *this;
    }
    yesthrow_rep(yesthrow_rep&&) noexcept(false) = default;
    yesthrow_rep& operator                       =(yesthrow_rep&&)
    {
        return *this;
    }
    bool operator==(const yesthrow_rep&) const noexcept(false) = default;
};

static_assert(is_nothrow_default_constructible_v</**/ width<nothrow_rep>>);
static_assert(is_nothrow_copy_constructible_v</*   */ width<nothrow_rep>>);
static_assert(is_nothrow_copy_assignable_v</*      */ width<nothrow_rep>>);
static_assert(is_nothrow_move_constructible_v</*   */ width<nothrow_rep>>);
static_assert(is_nothrow_move_assignable_v</*      */ width<nothrow_rep>>);
static_assert(is_nothrow_default_constructible_v</**/ abscissa<nothrow_rep>>);
static_assert(is_nothrow_copy_constructible_v</*   */ abscissa<nothrow_rep>>);
static_assert(is_nothrow_copy_assignable_v</*      */ abscissa<nothrow_rep>>);
static_assert(is_nothrow_move_constructible_v</*   */ abscissa<nothrow_rep>>);
static_assert(is_nothrow_move_assignable_v</*      */ abscissa<nothrow_rep>>);
static_assert(is_nothrow_default_constructible_v</**/ scale<nothrow_rep>>);
static_assert(is_nothrow_copy_constructible_v</*   */ scale<nothrow_rep>>);
static_assert(is_nothrow_copy_assignable_v</*      */ scale<nothrow_rep>>);
static_assert(is_nothrow_move_constructible_v</*   */ scale<nothrow_rep>>);
static_assert(is_nothrow_move_assignable_v</*      */ scale<nothrow_rep>>);

static_assert(!is_nothrow_default_constructible_v</**/ width<yesthrow_rep>>);
static_assert(!is_nothrow_copy_constructible_v</*   */ width<yesthrow_rep>>);
static_assert(!is_nothrow_copy_assignable_v</*      */ width<yesthrow_rep>>);
static_assert(!is_nothrow_move_constructible_v</*   */ width<yesthrow_rep>>);
static_assert(!is_nothrow_move_assignable_v</*      */ width<yesthrow_rep>>);
static_assert(!is_nothrow_default_constructible_v</**/ abscissa<yesthrow_rep>>);
static_assert(!is_nothrow_copy_constructible_v</*   */ abscissa<yesthrow_rep>>);
static_assert(!is_nothrow_copy_assignable_v</*      */ abscissa<yesthrow_rep>>);
static_assert(!is_nothrow_move_constructible_v</*   */ abscissa<yesthrow_rep>>);
static_assert(!is_nothrow_move_assignable_v</*      */ abscissa<yesthrow_rep>>);
static_assert(!is_nothrow_default_constructible_v</**/ scale<yesthrow_rep>>);
static_assert(!is_nothrow_copy_constructible_v</*   */ scale<yesthrow_rep>>);
static_assert(!is_nothrow_copy_assignable_v</*      */ scale<yesthrow_rep>>);
static_assert(!is_nothrow_move_constructible_v</*   */ scale<yesthrow_rep>>);
static_assert(!is_nothrow_move_assignable_v</*      */ scale<yesthrow_rep>>);

static_assert(is_nothrow_constructible_v<width<int>, int>);
static_assert(is_nothrow_constructible_v<width<int>, long>);
static_assert(is_nothrow_constructible_v<width<int>, width<long>>);
static_assert(is_nothrow_constructible_v<width<nothrow_rep>, nothrow_rep>);
static_assert(!is_nothrow_constructible_v<width<yesthrow_rep>, yesthrow_rep>);
static_assert(is_nothrow_constructible_v<abscissa<int>, int>);
static_assert(is_nothrow_constructible_v<abscissa<int>, long>);
static_assert(is_nothrow_constructible_v<abscissa<int>, abscissa<long>>);
static_assert(is_nothrow_constructible_v<abscissa<nothrow_rep>, nothrow_rep>);
static_assert(
    !is_nothrow_constructible_v<abscissa<yesthrow_rep>, yesthrow_rep>);
static_assert(is_nothrow_constructible_v<scale<int>, int>);
static_assert(is_nothrow_constructible_v<scale<int>, long>);
static_assert(is_nothrow_constructible_v<scale<int>, scale<long>>);
static_assert(is_nothrow_constructible_v<scale<nothrow_rep>, nothrow_rep>);
static_assert(!is_nothrow_constructible_v<scale<yesthrow_rep>, yesthrow_rep>);

static_assert(same_as<width<int>, decltype(width{0})>);
static_assert(same_as<abscissa<int>, decltype(abscissa{0})>);
static_assert(same_as<scale<int>, decltype(scale{0})>);

static_assert(same_as<common_type_t<width<int>, width<long>>, width<long>>);
static_assert(
    same_as<common_type_t<abscissa<int>, abscissa<long>>, abscissa<long>>);
static_assert(same_as<common_type_t<scale<int>, scale<long>>, scale<long>>);

constexpr void test()
{
    using namespace chrono;
    using namespace units::physical::si;

    []<class... Wrappers>(Wrappers...)
    {
        (..., []<class Wrapper>(Wrapper) {
            using rep = Wrapper::rep;
            static_assert(!is_constructible_v<rep, Wrapper>);
            static_assert([]<template <class> class... IsXable>() {
                return (... && (IsXable<Wrapper>{} == IsXable<rep>{}));
            }
                              .template operator()<
                                  std::is_trivially_destructible,
                                  std::is_trivially_default_constructible,
                                  std::is_trivially_copy_constructible,
                                  std::is_trivially_copy_assignable,
                                  std::is_trivially_move_constructible,
                                  std::is_trivially_move_assignable,
                                  std::is_nothrow_destructible,
                                  std::is_nothrow_default_constructible,
                                  std::is_nothrow_copy_constructible,
                                  std::is_nothrow_copy_assignable,
                                  std::is_nothrow_move_constructible,
                                  std::is_nothrow_move_assignable,
                                  std::is_standard_layout>());
        }(Wrappers{}));
    }
    (width{0}, width{0s}, width{0q_m}, abscissa{0}, abscissa{0s},
     abscissa{0q_m});

    []<template <class> class... Wrappers>()
    {
        (..., []<template <class> class Wrapper>() {
            static_assert(regular<Wrapper<int>>);
            static_assert(regular<Wrapper<seconds>>);
            static_assert(totally_ordered_with<Wrapper<int>, Wrapper<long>>);
            static_assert(
                totally_ordered_with<Wrapper<seconds>, Wrapper<minutes>>);
            static_assert(
                !equality_comparable_with<Wrapper<seconds>, Wrapper<int>>);
            static_assert(!equality_comparable_with<
                          Wrapper<length<metre>>, Wrapper<area<square_metre>>>);
        }.template operator()<Wrappers>());
    }
    .template operator()<width, abscissa>();

    static_assert(!is_constructible_v<width<int>, abscissa<int>>);
    static_assert(!is_constructible_v<abscissa<int>, width<int>>);

#define NOTHROW(...)                                                           \
    [&] {                                                                      \
        static_assert(noexcept(__VA_ARGS__));                                  \
        return __VA_ARGS__;                                                    \
    }()

    static_assert(NOTHROW(width<int>{'\0'}) == width{0});
    static_assert(NOTHROW(width<int>{42}) == width{42});
    static_assert(NOTHROW(width<int>{1729ull}) == width{1729});
    static_assert(!constructible_from<width<seconds>, int>);
    static_assert(width<seconds>{0s} == width{0s});
    static_assert(width<seconds>{42min} == width{42min});
    static_assert(!constructible_from<width<seconds>, milliseconds>);
    static_assert(!constructible_from<width<seconds>, duration<double>>);

    constexpr auto nothrow_convertible_to = []<class T, class U>(T, U) {
        return std::convertible_to<T, U> &&
               std::is_nothrow_convertible_v<T, U> &&
               std::is_nothrow_constructible_v<U, T>;
    };

    static_assert(nothrow_convertible_to(width{'c'}, width{0}));
    static_assert(nothrow_convertible_to(width{0ll}, width{0}));
    static_assert(std::convertible_to<width<seconds>, width<duration<double>>>);
    static_assert(!constructible_from<width<seconds>, width<duration<double>>>);
    static_assert(width<duration<double>>{width{20s}} == width{20s});

    constexpr auto equal_and_same = []<class T>(T l, T r) { return l == r; };

    static_assert(equal_and_same(NOTHROW(+width{'\0'}), width{0}));
    static_assert(equal_and_same(NOTHROW(-width{'\0'}), width{0}));

    width w{0};
    abscissa x{0};

    assert(NOTHROW(++w) == width{1});
    assert(NOTHROW(--w) == width{0});
    assert(NOTHROW(w++) == width{0} && w == width{1});
    assert(NOTHROW(w--) == width{1} && w == width{0});

    assert(NOTHROW(w += width{2u}) == width{2});
    assert(NOTHROW(x += width{2u}) == abscissa{2});
    assert(NOTHROW(w -= width{1l}) == width{1});
    assert(NOTHROW(x -= width{1l}) == abscissa{1});

    assert(NOTHROW(w *= 4u) == width{4});
    assert(NOTHROW(x *= scale{4u}) == abscissa{4});
    assert(NOTHROW(w /= 2l) == width{2});
    assert(NOTHROW(x /= scale{2l}) == abscissa{2});
    assert(NOTHROW(w %= 42) == width{2});
    assert(NOTHROW(w %= width{2}) == width{0});

    static_assert(equal_and_same(NOTHROW(width{2} + width{3u}), width{5u}));
    static_assert(equal_and_same(abscissa{2} + width{3u}, abscissa{5u}));
    static_assert(equal_and_same(width{2} + abscissa{3u}, abscissa{5u}));
    static_assert(equal_and_same(NOTHROW(width{5} - width{3u}), width{2u}));
    static_assert(equal_and_same(abscissa{5} - width{3u}, abscissa{2u}));
    static_assert(equal_and_same(abscissa{5} - abscissa{3u}, width{2u}));
    static_assert(!invocable<plus<>, abscissa<int>, abscissa<int>>);
    static_assert(!invocable<minus<>, width<int>, abscissa<int>>);

    static_assert(equal_and_same(NOTHROW(width{2} * 3u), width{6u}));
    static_assert(equal_and_same(NOTHROW(abscissa{2} * scale{3u})(), 6u));
    static_assert(equal_and_same(NOTHROW(2 * width{3u}), width{6u}));
    static_assert(equal_and_same(NOTHROW(scale{2} * abscissa{3u})(), 6u));
    static_assert(equal_and_same(NOTHROW(width{6} / 3u), width{2u}));
    static_assert(equal_and_same(NOTHROW(abscissa{6} / scale{3u})(), 2u));
    static_assert(equal_and_same(NOTHROW(width{6} / width{3u}), 2u));
    static_assert(equal_and_same(NOTHROW(width{5} % 3u), width{2u}));
    static_assert(equal_and_same(NOTHROW(width{5} % width{3u}), width{2u}));
    static_assert(
        !invocable<multiplies<>, width<length<metre>>, width<length<metre>>>);
    static_assert(!invocable<
                  multiplies<>, abscissa<length<metre>>, scale<length<metre>>>);
    static_assert(!invocable<
                  multiplies<>, scale<length<metre>>, abscissa<length<metre>>>);
    static_assert(
        !invocable<divides<>, width<area<square_metre>>, width<length<metre>>>);
    static_assert(
        !invocable<
            divides<>, abscissa<area<square_metre>>, scale<length<metre>>>);
    static_assert(
        !invocable<modulus<>, width<area<square_metre>>, width<length<metre>>>);
    static_assert(!invocable<modulus<>, abscissa<int>, abscissa<int>>);

    static_assert(equal_and_same(NOTHROW(width{0} == width{0}), true));
    static_assert(equal_and_same(NOTHROW(width{0} <= width{0}), true));
}

consteval auto const_invoke(auto f)
{
    return f();
}

auto run_invoke(auto f)
{
    return f();
}

int main()
{
    const_invoke(test);
    run_invoke(test);
}
