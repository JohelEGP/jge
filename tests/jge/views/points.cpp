#include <algorithm>
#include <array>
#include <cassert>
#include <ranges>
#include <utility>
#include <jge/pixels.hpp>
#include <jge/views/points.hpp>

constexpr int test()
{
    using namespace jge;
    using namespace std::ranges;

    static_assert(view<points_view<int>>);
    static_assert(random_access_range<points_view<int>>);
    static_assert(sized_range<points_view<int>>);

    assert(empty(points_view{size2d{width{0}, height{0}}}));
    assert(empty(points_view{size2d{width{1}, height{0}}}));
    assert(empty(points_view{size2d{width{0}, height{1}}}));
    [] {
        const points_view pts{size2d{width{1}, height{1}}};
        const std::array elems{point2d{abscissa{0}, ordinate{0}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{2}, height{1}}};
        const std::array elems{
            point2d{abscissa{0}, ordinate{0}},
            point2d{abscissa{1}, ordinate{0}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{1}, height{2}}};
        const std::array elems{
            point2d{abscissa{0}, ordinate{0}},
            point2d{abscissa{0}, ordinate{1}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{2}, height{2}}};
        const std::array elems{
            point2d{abscissa{0}, ordinate{0}},
            point2d{abscissa{1}, ordinate{0}},
            point2d{abscissa{0}, ordinate{1}},
            point2d{abscissa{1}, ordinate{1}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{3}, height{2}}};
        const std::array elems{point2d{abscissa{0}, ordinate{0}},
                               point2d{abscissa{1}, ordinate{0}},
                               point2d{abscissa{2}, ordinate{0}},
                               point2d{abscissa{0}, ordinate{1}},
                               point2d{abscissa{1}, ordinate{1}},
                               point2d{abscissa{2}, ordinate{1}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{2}, height{3}}};
        const std::array elems{point2d{abscissa{0}, ordinate{0}},
                               point2d{abscissa{1}, ordinate{0}},
                               point2d{abscissa{0}, ordinate{1}},
                               point2d{abscissa{1}, ordinate{1}},
                               point2d{abscissa{0}, ordinate{2}},
                               point2d{abscissa{1}, ordinate{2}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();
    [] {
        const points_view pts{size2d{width{2_px}, height{3_px}}};
        const std::array elems{
            point2d{abscissa{0_px}, ordinate{0_px}},
            point2d{abscissa{1_px}, ordinate{0_px}},
            point2d{abscissa{0_px}, ordinate{1_px}},
            point2d{abscissa{1_px}, ordinate{1_px}},
            point2d{abscissa{0_px}, ordinate{2_px}},
            point2d{abscissa{1_px}, ordinate{2_px}}};
        assert(std::cmp_equal(size(pts), elems.size()));
        assert(equal(pts, elems));
        assert(equal(reverse_view{pts}, reverse_view{elems}));
    }();

    return 0;
}

int main()
{
    constexpr int ret{test()};
    return ret;
}
