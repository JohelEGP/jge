#ifndef JGE_DETAIL_MEMORY_HPP
#define JGE_DETAIL_MEMORY_HPP

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace jge::detail
{
inline constexpr bool default_allocator_is_nothrow{false};

inline constexpr auto uninitialized_value_construct =
    []<class R>(R && r) -> std::ranges::borrowed_iterator_t<R>
    requires requires
    {
        std::ranges::uninitialized_value_construct(std::forward<R>(r));
    }
{
    auto first{std::ranges::begin(r)};
    auto last{std::ranges::end(r)};
    for (; first != last; ++first)
        std::construct_at(std::to_address(first));
    return first;
};

inline constexpr auto uninitialized_copy =
    []<class IR, class OR>(IR && in_range, OR&& out_range)
    -> std::ranges::uninitialized_copy_result<
        std::ranges::borrowed_iterator_t<IR>,
        std::ranges::borrowed_iterator_t<OR>>
    requires requires
    {
        std::ranges::uninitialized_copy(
            std::forward<IR>(in_range), std::forward<OR>(out_range));
    }
{
    auto ifirst{std::ranges::begin(in_range)};
    auto ilast{std::ranges::end(in_range)};
    auto ofirst{std::ranges::begin(out_range)};
    auto olast{std::ranges::end(out_range)};
    for (; ifirst != ilast && ofirst != olast; ++ofirst, (void)++ifirst)
        std::construct_at(std::to_address(ofirst), *ifirst);
    return {std::move(ifirst), ofirst};
};

} // namespace jge::detail

#endif // JGE_DETAIL_MEMORY_HPP
