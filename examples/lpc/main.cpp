#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <jge/cartesian.hpp>
#include <jge/pixels.hpp>
#include <jge/plane.hpp>
#include <jge/views/points.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>

constexpr std::integral auto integral(const auto& x)
{
    auto impl = []<class Rep>(const Rep& y) { return y / Rep{1}; };
    if constexpr (requires { x(); })
        return impl(x());
    else
        return impl(x);
}

using namespace jge::literals;

using pixels_size    = jge::size2d<jge::pixels<unsigned>>;
using pixel_point    = jge::point2d<jge::pixels<unsigned>>;
using pixel_subplane = jge::subplane<jge::pixels<int>>;

class image_view;

class [[nodiscard]] image
{
    sf::Image img;

public:
    image() = default;

    explicit image(const pixels_size sz)
    {
        img.create(sz.w().count(), sz.h().count());
    }

    [[nodiscard]] static std::optional<image>
    load_from_file(const std::string& filename)
    {
        image res;
        if (res.img.loadFromFile(filename))
            return res;
        return {};
    }

    void copy(image_view, pixel_point top_left);

    [[nodiscard]] sf::Image& underlying() noexcept
    {
        return img;
    }

    [[nodiscard]] const sf::Image& underlying() const noexcept
    {
        return img;
    }

    pixels_size size() const noexcept
    {
        auto [w, h]{img.getSize()};
        return {
            jge::width{jge::pixels<unsigned>{w}},
            jge::height{jge::pixels<unsigned>{h}}};
    }

    [[nodiscard]] std::span<const sf::Uint8> pixels() const noexcept
    {
        return std::span{img.getPixelsPtr(), to1d(size()).count()};
    }

    [[nodiscard]] bool operator==(const image& r) const noexcept
    {
        return size() == r.size() && std::ranges::equal(pixels(), r.pixels());
    }

    operator image_view() const noexcept;
};

class [[nodiscard]] image_view
{
    const image* img_{};
    pixel_subplane plane_{};

public:
    image_view() = default;

    image_view(const image& img, const pixel_subplane plane) noexcept
      : img_{&img}, plane_{plane}
    {
        assert(contains<jge::pixels<unsigned>>(img.size(), plane));
    }

    void img(const image& img) noexcept
    {
        assert(contains<jge::pixels<unsigned>>(img.size(), plane_));
        img_ = &img;
    }

    void plane(const pixel_subplane s) noexcept
    {
        assert(contains<jge::pixels<unsigned>>(img_->size(), s));
        plane_ = s;
    }

    [[nodiscard]] const image& img() const noexcept
    {
        assert(img_);
        return *img_;
    }

    pixel_subplane plane() const noexcept
    {
        return plane_;
    }
};

image::operator image_view() const noexcept
{
    return {*this, {{}, size()}};
}

void image::copy(const image_view v, const pixel_point top_left)
{
    assert(contains<jge::pixels<unsigned>>(size(), {top_left, v.plane().size}));
    img.copy(
        v.img().underlying(), integral(top_left.x), integral(top_left.y),
        {integral(v.plane().top_left.x), integral(v.plane().top_left.y),
         integral(v.plane().size.w), integral(v.plane().size.h)});
}

constexpr jge::pixels<unsigned> tile_side{32_px};

template <units::Scalar Rep>
using tiles = jge::tiles<tile_side.count(), Rep>;

constexpr jge::size2d tile_size{jge::width{tile_side}, jge::height{tile_side}};

class [[nodiscard]] tile_set
{
    image img;

public:
    tile_set() = default;

    using size_type  = jge::size2d<tiles<unsigned>>;
    using point_type = jge::point2d<tiles<unsigned>>;

    explicit tile_set(image img) noexcept : img{std::move(img)}
    {
        assert(img.size().w() % tile_side == 0_px);
        assert(img.size().h() % tile_side == 0_px);
    }

    image_view operator[](const point_type pt) const noexcept
    {
        assert(contains(size(), pt));
        return {
            img,
            {pt / jge::scale{tiles<unsigned>{1}} * jge::scale{tile_side},
             tile_size}};
    }

    size_type size() const noexcept
    {
        return {
            jge::width{tiles<unsigned>{img.size().w() / tile_side}},
            jge::height{tiles<unsigned>{img.size().h() / tile_side}}};
    }
};

class [[nodiscard]] layer
{
    image img_;

public:
    layer() = default;

    layer(
        const tile_set& tset,
        const jge::plane<tile_set::point_type>& tset_tiles)
      : img_{tset_tiles.size() * tile_side}
    {
        for (auto [layer_tile, tset_tile] : ranges::zip_view(
                 jge::views::points(tset_tiles.size()), to1d(tset_tiles)))
            img_.copy(tset[tset_tile], layer_tile * jge::scale{tile_side});
    }

    [[nodiscard]] const image& img() const noexcept
    {
        return img_;
    }
};

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    // Load a sprite to display
    tile_set tset{image::load_from_file("grass.png").value()};
    const jge::plane background{
        {15, 16, 17, 10, 17, 16},
        {16, 17, 10, 10, 10, 17},
        {17, 10, 10, 17, 10, 10}};
    const layer lyr{
        tset,
        {to1d(background) |
             ranges::views::transform([&](const std::size_t tile1d) {
                 return to2d(
                            tile1d,
                            jge::width<std::size_t>{tset.size().w().count()}) *
                        jge::scale{tiles<unsigned>{1}};
             }),
         background.size().w}};
    sf::Texture texture;
    texture.loadFromImage(lyr.img().underlying());
    sf::Sprite sprite(texture);
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Update the window
        window.display();
    }
}
