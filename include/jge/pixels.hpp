#ifndef JGE_PIXELS_HPP
#define JGE_PIXELS_HPP

#include <cstdint>
#include <units/base_dimension.h>
#include <units/concepts.h>
#include <units/derived_dimension.h>
#include <units/exp.h>
#include <units/prefix.h>
#include <units/quantity.h>
#include <units/ratio.h>
#include <units/unit.h>

namespace jge
{
// Length.

struct pixel : units::named_unit<pixel, "px", units::no_prefix>
{
};

struct dim_display_length : units::base_dimension<"dL", pixel>
{
};

template <units::Scalar Rep>
using pixels = units::quantity<dim_display_length, pixel, Rep>;

inline namespace literals
{
    consteval auto operator""_px(unsigned long long px)
    {
        return pixels<std::int64_t>(px);
    }

    consteval auto operator""_px(long double px)
    {
        return pixels<long double>(px);
    }

} // namespace literals

// Area.

struct square_pixel : units::unit<square_pixel>
{
};

struct dim_display_area
  : units::derived_dimension<
        dim_display_area, square_pixel, units::exp<dim_display_length, 2>>
{
};

template <std::intmax_t Side>
struct tile
  : units::named_scaled_unit<
        tile<Side>, "tile", units::no_prefix, units::ratio{Side}, square_pixel>
{
};

template <std::intmax_t Side, units::Scalar Rep>
using tiles = units::quantity<dim_display_area, tile<Side>, Rep>;

} // namespace jge

#endif // JGE_PIXELS_HPP
