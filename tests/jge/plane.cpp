#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <memory>
#include <span>
#include <type_traits>
#include <utility>
#include <jge/cartesian.hpp>
#include <jge/plane.hpp>

template <auto>
struct _require_constant_;

// Remove when direct use of requires-expression formats well.
template <class F, class... Args>
constexpr bool _constant_v_ = requires
{
    typename _require_constant_<(std::invoke(F{}, Args{}()...), 0)>;
};

template <std::semiregular F, std::semiregular... Args>
    requires std::invocable<F, std::invoke_result_t<Args>...>
struct [[nodiscard]] constant : std::bool_constant<_constant_v_<F, Args...>>
{
    constexpr constant(F, Args...)
    {
    }

    constexpr void operator+() const noexcept
    {
        static_assert(
            this->value == true,
            "std::invoke(F{}, Args{}()...) is NOT a constant expression.");
    }

    constexpr void operator!() const noexcept
    {
        static_assert(
            this->value == false,
            "std::invoke(F{}, Args{}()...) IS a constant expression.");
    }
};

static_assert([] {
    +constant([] {});
    +constant([] { return false; });
    !constant([] { assert(false); });
    !constant([] { throw false; });
    return true;
}());

consteval auto operator""_w(unsigned long long w) noexcept
{
    return jge::width{w};
}

consteval auto operator""_h(unsigned long long h) noexcept
{
    return jge::height{h};
}

consteval auto operator""_x(unsigned long long x) noexcept
{
    return jge::abscissa{x};
}

consteval auto operator""_y(unsigned long long y) noexcept
{
    return jge::ordinate{y};
}

struct deleted_spec_mem_fns
{
    deleted_spec_mem_fns()                            = delete;
    deleted_spec_mem_fns(const deleted_spec_mem_fns&) = delete;
    void operator=(const deleted_spec_mem_fns&)  = delete;
    deleted_spec_mem_fns(deleted_spec_mem_fns&&) = delete;
    void operator=(deleted_spec_mem_fns&&) = delete;
};

static_assert(+std::regular</*   */ jge::plane<int>>);
static_assert(!std::totally_ordered<jge::plane<int>>);
static_assert(+std::semiregular</**/ jge::plane<std::span<int>>>);
static_assert(!std::regular</*    */ jge::plane<std::span<int>>>);
static_assert(+std::movable</*       */ jge::plane<std::unique_ptr<int>>>);
static_assert(!std::copyable</*      */ jge::plane<std::unique_ptr<int>>>);
static_assert(+std::equality_comparable<jge::plane<std::unique_ptr<int>>>);
static_assert(+std::movable</*       */ jge::plane<deleted_spec_mem_fns>>);
static_assert(!std::copyable</*      */ jge::plane<deleted_spec_mem_fns>>);
static_assert(!std::equality_comparable<jge::plane<deleted_spec_mem_fns>>);

static_assert(+std::is_nothrow_default_constructible_v</**/ jge::plane<int>>);
static_assert(!std::is_nothrow_copy_constructible_v</*   */ jge::plane<int>>);
static_assert(!std::is_nothrow_copy_assignable_v</*      */ jge::plane<int>>);
static_assert(+std::is_nothrow_move_constructible_v</*   */ jge::plane<int>>);
static_assert(+std::is_nothrow_move_assignable_v</*      */ jge::plane<int>>);
static_assert(!noexcept(jge::plane<int>{0_w + 0_h, jge::default_initialize}));
static_assert(!noexcept(jge::plane<int>{0_w + 0_h, jge::value_initialize}));
static_assert(!noexcept(jge::plane<int>{{0}}));
static_assert(!noexcept(jge::plane<int>{} = {{0}}));
static_assert(!noexcept(jge::plane<int>{{0}, 0_w}));
static_assert(!noexcept(jge::plane<int>{}.assign({0}, 0_w)));
static_assert(!noexcept(jge::plane<int>{"", 0_w}));
static_assert(!noexcept(jge::plane<int>{}.assign("", 0_w)));

static_assert(std::same_as<jge::plane<int>, decltype(jge::plane{{0}})>);
static_assert(std::same_as<jge::plane<int>, decltype(jge::plane{{0}, 0_w})>);
static_assert(std::same_as<jge::plane<char>, decltype(jge::plane{"", 0_w})>);

constexpr void test()
{
    {
        jge::plane<int> p;
        const auto& cp{p};
        static_assert(std::same_as<decltype(p[0_x + 0_y]), int&>);
        static_assert(std::same_as<decltype(cp[0_x + 0_y]), const int&>);
        static_assert(noexcept(p[0_x + 0_y]));
        static_assert(noexcept(cp[0_x + 0_y]));
        static_assert(noexcept(cp.size()));
        static_assert(noexcept(cp == cp));
        static_assert(noexcept(to1d(p)));
        static_assert(noexcept(to1d(cp)));
    }

    {
        auto plane = [] {
            jge::plane<int> p;
            return p;
        };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane<int>{}; };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] {
            return jge::plane<int>{0_w + 0_h, jge::default_initialize};
        };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] {
            return jge::plane<int>{0_w + 0_h, jge::value_initialize};
        };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane<int>{{}}; };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane<int>{{}, {}}; };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane<int>{{}, 0_w}; };
        auto p{plane()};
        const auto& cp{p};
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(p)));
        assert([](auto s) { return s.empty() && !s.data(); }(to1d(cp)));
        assert(cp.size() == 0_w + 0_h);
        !constant([](/* */ auto p) { (void)p[0_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 0_y]; }, plane);
    }

    {
        auto plane = [] { return jge::plane{{0}}; };
        auto p{plane()};
        const auto& cp{p};
        assert(std::ranges::equal(std::array{0}, to1d(p)));
        assert(std::ranges::equal(std::array{0}, to1d(cp)));
        assert(cp.size() == 1_w + 1_h);
        assert(cp[0_x + 0_y] == 0);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert((p[0_x + 0_y] = 1) == 1);
        !constant([](/* */ auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 1_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 1_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane{{0, 1}}; };
        auto p{plane()};
        const auto& cp{p};
        assert(std::ranges::equal(std::array{0, 1}, to1d(p)));
        assert(std::ranges::equal(std::array{0, 1}, to1d(cp)));
        assert(cp.size() == 2_w + 1_h);
        assert(cp[0_x + 0_y] == 0);
        assert(cp[1_x + 0_y] == 1);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert(&p[1_x + 0_y] == &cp[1_x + 0_y]);
        assert((p[0_x + 0_y] = 2) == 2);
        assert((p[1_x + 0_y] = 3) == 3);
        !constant([](/* */ auto p) { (void)p[2_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[2_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 1_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 1_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane{{0}, {1}}; };
        auto p{plane()};
        const auto& cp{p};
        assert(std::ranges::equal(std::array{0, 1}, to1d(p)));
        assert(std::ranges::equal(std::array{0, 1}, to1d(cp)));
        assert(cp.size() == 1_w + 2_h);
        assert(cp[0_x + 0_y] == 0);
        assert(cp[0_x + 1_y] == 1);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert(&p[0_x + 1_y] == &cp[0_x + 1_y]);
        assert((p[0_x + 0_y] = 2) == 2);
        assert((p[0_x + 1_y] = 3) == 3);
        !constant([](/* */ auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 2_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 2_y]; }, plane);
    }
    {
        auto plane = [] { return jge::plane{{0, 1}, {2, 3}}; };
        auto p{plane()};
        const auto& cp{p};
        assert(std::ranges::equal(std::array{0, 1, 2, 3}, to1d(p)));
        assert(std::ranges::equal(std::array{0, 1, 2, 3}, to1d(cp)));
        assert(cp.size() == 2_w + 2_h);
        assert(cp[0_x + 0_y] == 0);
        assert(cp[1_x + 0_y] == 1);
        assert(cp[0_x + 1_y] == 2);
        assert(cp[1_x + 1_y] == 3);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert(&p[1_x + 0_y] == &cp[1_x + 0_y]);
        assert(&p[0_x + 1_y] == &cp[0_x + 1_y]);
        assert(&p[1_x + 1_y] == &cp[1_x + 1_y]);
        assert((p[0_x + 0_y] = 4) == 4);
        assert((p[1_x + 0_y] = 5) == 5);
        assert((p[0_x + 1_y] = 6) == 6);
        assert((p[1_x + 1_y] = 7) == 7);
        !constant([](/* */ auto p) { (void)p[2_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[2_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 2_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 2_y]; }, plane);
    }
    {
        !constant([] { (void)jge::plane{{0}, {}}; });
        !constant([] { (void)jge::plane{{}, {0}}; });
        !constant([] { (void)jge::plane{{0, 1}, {2}}; });
        !constant([] { (void)jge::plane{{0}, {1, 2}}; });
    }

    {
        using jge::plane;
        assert((jge::plane{{0}, 0000000001_w} == jge::plane{{0}}));
        assert((jge::plane{{0, 1}, 0000001_w} == jge::plane{{0}, {1}}));
        assert((jge::plane{{0, 1}, 0000002_w} == jge::plane{{0, 1}}));
        assert((jge::plane{{0, 1, 2}, 0001_w} == jge::plane{{0}, {1}, {2}}));
        assert((jge::plane{{0, 1, 2}, 0003_w} == jge::plane{{0, 1, 2}}));
        assert((jge::plane{{0, 1, 2, 3}, 1_w} == plane{{0}, {1}, {2}, {3}}));
        assert((jge::plane{{0, 1, 2, 3}, 2_w} == jge::plane{{0, 1}, {2, 3}}));
        assert((jge::plane{{0, 1, 2, 3}, 4_w} == jge::plane{{0, 1, 2, 3}}));
        !constant([] { (void)jge::plane<int>{{}, 1_w}; });
        !constant([] { (void)jge::plane<int>{{}, 2_w}; });
        !constant([] { (void)jge::plane{{0}, 0_w}; });
        !constant([] { (void)jge::plane{{0}, 2_w}; });
        !constant([] { (void)jge::plane{{0, 1}, 0_w}; });
        !constant([] { (void)jge::plane{{0, 1}, 3_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2}, 0_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2}, 2_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2}, 4_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2, 3}, 0_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2, 3}, 3_w}; });
        !constant([] { (void)jge::plane{{0, 1, 2, 3}, 5_w}; });
    }

    {
        auto plane = [] {
            return jge::plane<int>{1_w + 1_h, jge::default_initialize};
        };
        auto p{plane()};
        const auto& cp{p};
        assert(1 == to1d(p).size());
        assert(1 == to1d(cp).size());
        assert(cp.size() == 1_w + 1_h);
        !constant([](const auto p) { void(p[0_x + 0_y] == 0); }, plane);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert((p[0_x + 0_y] = 1) == 1);
        !constant([](/* */ auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 1_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 1_y]; }, plane);
    }
    {
        auto plane = [] {
            return jge::plane<int>{1_w + 1_h, jge::value_initialize};
        };
        auto p{plane()};
        const auto& cp{p};
        assert(std::ranges::equal(std::array{0}, to1d(p)));
        assert(std::ranges::equal(std::array{0}, to1d(cp)));
        assert(cp.size() == 1_w + 1_h);
        assert(cp[0_x + 0_y] == 0);
        assert(&p[0_x + 0_y] == &cp[0_x + 0_y]);
        assert((p[0_x + 0_y] = 1) == 1);
        !constant([](/* */ auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](const auto p) { (void)p[1_x + 0_y]; }, plane);
        !constant([](/* */ auto p) { (void)p[0_x + 1_y]; }, plane);
        !constant([](const auto p) { (void)p[0_x + 1_y]; }, plane);
    }
    {
        using namespace jge;
        !constant([] { (void)jge::plane<int>(1_w + 0_h, default_initialize); });
        !constant([] { (void)jge::plane<int>(0_w + 1_h, default_initialize); });
        !constant([] { (void)jge::plane<int>(1_w + 0_h, value_initialize); });
        !constant([] { (void)jge::plane<int>(0_w + 1_h, value_initialize); });
    }

    {
        constexpr jge::value_initialize_t vi;
        assert(jge::plane<int>(1_w + 1_h, vi) == jge::plane({{0}}));
        assert(jge::plane<int>(2_w + 1_h, vi) == jge::plane({{0, 0}}));
        assert(jge::plane<int>(1_w + 2_h, vi) == jge::plane({{0}, {0}}));
        assert(jge::plane<int>(2_w + 2_h, vi) == jge::plane({{0, 0}, {0, 0}}));
    }

    {
        const jge::plane p1{{0, 1}, {2, 3}};
        const jge::plane p2{{0, 1, 2, 3}};
        assert(p1 == p1 && p2 == p2);
        assert(p1 != p2);
        assert(std::ranges::equal(to1d(p1), to1d(p2)));
    }

    {
        const jge::plane<int> p1;
        const jge::plane p2{p1};
        assert(p1 == p2);
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        const jge::plane p2{p1};
        assert(p1 == p2);
    }
    {
        !constant([] {
            const jge::plane<int> p1{1_w + 1_h, jge::default_initialize};
            const jge::plane p2{p1};
        });
    }
    {
        const jge::plane<int> p1;
        jge::plane<int> p2;
        assert(p1 == p2);
        assert(&(p2 = p1) == &p2);
        assert(p1 == p2);
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        jge::plane<int> p2;
        assert(p1 != p2);
        assert(&(p2 = p1) == &p2);
        assert(p1 == p2);
        p2[0_x + 0_y] = 4;
        assert(p1 != p2);
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        jge::plane<int> p2{p1};
        const std::span s0{to1d(p2)};
        assert(p1 == p2);
        assert(&(p2 = p1) == &p2);
        assert(p1 == p2);
        const std::span s1{to1d(p2)};
        assert(s0.data() == s1.data() && s0.size() == s1.size());
    }
    {
        !constant([] {
            const jge::plane<int> p1{1_w + 1_h, jge::default_initialize};
            jge::plane<int> p2;
            p2 = p1;
        });
    }
    {
        const jge::plane<int> p1;
        jge::plane p3{p1};
        jge::plane p2{std::move(p3)};
        assert(p1 == p2 && p2 == p3);
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        jge::plane p3{p1};
        jge::plane p2{std::move(p3)};
        assert(p1 == p2);
        assert(p3 == jge::plane<int>{});
    }
    {
        const jge::plane<int> p1;
        jge::plane p3{p1};
        jge::plane<int> p2;
        p2 = std::move(p3);
        assert(p1 == p2 && p2 == p3);
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        jge::plane p3{p1};
        jge::plane<int> p2;
        p2 = std::move(p3);
        assert(p1 == p2);
        assert(p3 == jge::plane<int>{});
    }
    {
        const jge::plane p1{{0, 1}, {2, 3}};
        jge::plane p3{p1};
        jge::plane<int> p2{{4, 5}};
        p2 = std::move(p3);
        assert(p1 == p2);
        assert(p3 == jge::plane({{4, 5}}));
    }
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
