#ifndef SDLUTIL_HPP_INCLUDED
#define SDLUTIL_HPP_INCLUDED
//
// sdl-util.hpp
//
#include "misc/util.hpp"

#include <SDL.h>

#include <algorithm>
#include <exception>
#include <iomanip>
#include <ostream>
#include <sstream>

//
// misc
//
namespace util
{

    inline void destroyTexture(SDL_Texture *& texturePtr)
    {
        if (texturePtr != nullptr)
        {
            SDL_DestroyTexture(texturePtr);
            texturePtr = nullptr;
        }
    }

    inline void freeSurface(SDL_Surface *& surfacePtr)
    {
        if (surfacePtr != nullptr)
        {
            SDL_FreeSurface(surfacePtr);
            surfacePtr = nullptr;
        }
    }

    inline const SDL_Point size(SDL_Texture * const texturePtr)
    {
        SDL_Point size{ 0, 0 };

        if (texturePtr != nullptr)
        {
            const int result = SDL_QueryTexture(texturePtr, nullptr, nullptr, &size.x, &size.y);
            if (result != 0)
            {
                std::ostringstream ss;
                ss << "sdl-util::size() failed: " << SDL_GetError();
                throw std::runtime_error(ss.str());
            }
        }

        return size;
    }
} // namespace util

//
// sdl operators
//

[[nodiscard]] inline bool operator==(const SDL_Color & A, const SDL_Color & B) noexcept
{
    return ((A.r == B.r) && (A.g == B.g) && (A.b == B.b) && (A.a == B.a));
}

[[nodiscard]] inline bool operator!=(const SDL_Color & A, const SDL_Color & B) noexcept
{
    return !(A == B);
}

inline std::ostream & operator<<(std::ostream & os, const SDL_Point & point)
{
    os << '(' << point.x << 'x' << point.y << ')';
    return os;
}

inline std::ostream & operator<<(std::ostream & os, const SDL_Rect & rect)
{
    os << '(' << rect.x << ',' << rect.y << '/' << rect.w << 'x' << rect.h << ')';
    return os;
}

inline std::ostream & operator<<(std::ostream & os, const SDL_Color & color)
{
    os << '(' << static_cast<int>(color.r) << ',' << static_cast<int>(color.g) << ','
       << static_cast<int>(color.b) << ';' << static_cast<int>(color.a) << ')';
    return os;
}

inline std::ostream & operator<<(std::ostream & os, const SDL_DisplayMode & mode)
{
    os << '(' << mode.w << 'x' << mode.h << ':' << SDL_BITSPERPIXEL(mode.format) << '-'
       << mode.refresh_rate << "Hz_" << SDL_GetPixelFormatName(mode.format) << ')';

    return os;
}

inline std::ostream & operator<<(std::ostream & os, const SDL_RendererInfo & info)
{
    os << '(' << info.name << '-' << info.max_texture_width << 'x' << info.max_texture_height
       << '(';

    if (info.flags & SDL_RENDERER_SOFTWARE)
    {
        os << "soft|";
    }

    if (info.flags & SDL_RENDERER_ACCELERATED)
    {
        os << "acc|";
    }

    if (info.flags & SDL_RENDERER_PRESENTVSYNC)
    {
        os << "vsyn|";
    }

    if (info.flags & SDL_RENDERER_TARGETTEXTURE)
    {
        os << "text|";
    }

    os << ")[";
    for (unsigned t(0); t < info.num_texture_formats; ++t)
    {
        os << SDL_GetPixelFormatName(info.texture_formats[t]) << ',';
    }

    os << ']';

    return os;
}

[[nodiscard]] inline bool operator==(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return ((left.x == right.x) && (left.y == right.y));
}

[[nodiscard]] inline bool operator<(const SDL_Point & left, const SDL_Point & right) noexcept
{
    if (left.x != right.x)
    {
        return (left.x < right.x);
    }
    else
    {
        return (left.y < right.y);
    }
}

[[nodiscard]] inline bool operator<=(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return ((left == right) || (left < right));
}

[[nodiscard]] inline const SDL_Point
    operator+(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return { (left.x + right.x), (left.y + right.y) };
}

[[nodiscard]] inline const SDL_Point
    operator-(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return { (left.x - right.x), (left.y - right.y) };
}

[[nodiscard]] inline const SDL_Point
    operator*(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return { (left.x * right.x), (left.y * right.y) };
}

[[nodiscard]] inline const SDL_Point operator*(const SDL_Point & left, const int right) noexcept
{
    return { (left.x * right), (left.y * right) };
}

[[nodiscard]] inline const SDL_Point
    operator/(const SDL_Point & numerator, const SDL_Point & denominator) noexcept
{
    return { (numerator.x / denominator.x), (numerator.y / denominator.y) };
}

[[nodiscard]] inline const SDL_Point
    operator/(const SDL_Point & numerator, const int & denominator) noexcept
{
    return { (numerator.x / denominator), (numerator.y / denominator) };
}

//

[[nodiscard]] inline bool operator==(const SDL_Rect & r1, const SDL_Rect & r2) noexcept
{
    return (std::tie(r1.x, r1.y, r1.w, r1.h) == std::tie(r2.x, r2.y, r2.w, r2.h));
}

[[nodiscard]] inline bool operator<(const SDL_Rect & r1, const SDL_Rect & r2) noexcept
{
    return (std::tie(r1.x, r1.y, r1.w, r1.h) < std::tie(r2.x, r2.y, r2.w, r2.h));
}

//
// rect helpers
//

namespace util
{

    [[nodiscard]] inline const SDL_Point position(const SDL_Rect & rect)
    {
        return { rect.x, rect.y };
    }

    [[nodiscard]] inline int right(const SDL_Rect & rect) { return (rect.x + rect.w); }

    [[nodiscard]] inline int bottom(const SDL_Rect & rect) { return (rect.y + rect.h); }

    [[nodiscard]] inline const SDL_Point size(const SDL_Rect & rect) { return { rect.w, rect.h }; }

    [[nodiscard]] inline SDL_Point center(const SDL_Rect & rect)
    {
        return (position(rect) + (size(rect) / 2));
    }

    [[nodiscard]] inline const SDL_Rect
        makeRect(const SDL_Point & pos, const SDL_Point & size) noexcept
    {
        return { pos.x, pos.y, size.x, size.y };
    }

    // sfml utils to re-size (scale) any sf::FloatRect without moving it
    inline void scaleRectInPlace(SDL_Rect & rect, const float scaleX, const float scaleY) noexcept
    {
        const int widthChange(static_cast<int>(static_cast<float>(rect.w) * scaleX) - rect.w);
        rect.w += widthChange;
        rect.x -= (widthChange / 2);

        const int heightChange(static_cast<int>(static_cast<float>(rect.h) * scaleY) - rect.h);
        rect.h += heightChange;
        rect.y -= (heightChange / 2);
    }

    [[nodiscard]] inline const SDL_Rect scaleRectInPlaceCopy(
        const SDL_Rect & before, const float scaleX, const float scaleY) noexcept
    {
        SDL_Rect after{ before };
        scaleRectInPlace(after, scaleX, scaleY);
        return after;
    }

    inline void scaleRectInPlace(SDL_Rect & rect, const float scale) noexcept
    {
        scaleRectInPlace(rect, scale, scale);
    }

    [[nodiscard]] inline const SDL_Rect
        scaleRectInPlaceCopy(const SDL_Rect & before, const float scale) noexcept
    {
        SDL_Rect after{ before };
        scaleRectInPlace(after, scale);
        return after;
    }

    // without changing the shape
    inline void fit(SDL_Rect & rect, const int width, const int height)
    {
        // skip if empty to avoid dividing by zero below
        const SDL_Point sizeOrig{ rect.w, rect.h };
        if ((sizeOrig.x <= 0) || (sizeOrig.y <= 0))
        {
            return;
        }

        const float scaleHoriz{ static_cast<float>(width) / static_cast<float>(sizeOrig.x) };
        scaleRectInPlace(rect, scaleHoriz, scaleHoriz);

        if (rect.h > height)
        {
            const float scaleVert{ static_cast<float>(height) / static_cast<float>(sizeOrig.y) };
            scaleRectInPlace(rect, scaleVert, scaleVert);
        }
    }

    // without changing the shape
    [[nodiscard]] inline const SDL_Rect
        fitCopy(const SDL_Rect & before, const int width, const int height)
    {
        SDL_Rect after{ before };
        fit(after, width, height);
        return after;
    }

    inline void fit(SDL_Rect & rect, const int dimm) { fit(rect, dimm, dimm); }

    inline const SDL_Rect fitCopy(const SDL_Rect & before, const int dimm)
    {
        return fitCopy(before, dimm, dimm);
    }

} // namespace util

//
// vetor and euclidian math
//

[[nodiscard]] inline float dotProduct(const SDL_Point & left, const SDL_Point & right) noexcept
{
    return static_cast<float>((left.x * right.x) + (left.y * right.y));
}

[[nodiscard]] inline const SDL_Point
    difference(const SDL_Point & from, const SDL_Point & to) noexcept
{
    return (to - from);
}

[[nodiscard]] inline float magnitude(const SDL_Point & vec) noexcept
{
    return static_cast<float>(std::sqrt((vec.x * vec.x) + (vec.y * vec.y)));
}

[[nodiscard]] inline float distance(const SDL_Point & from, const SDL_Point & to) noexcept
{
    return magnitude(to - from);
}

[[nodiscard]] inline const SDL_Point
    normalize(const SDL_Point & vec, const SDL_Point & returnOnError = { 0, 0 }) noexcept
{
    const float mag{ magnitude(vec) };

    if (mag < util::tiny)
    {
        return returnOnError;
    }

    const float normX = static_cast<float>(vec.x) / mag;
    const float normY = static_cast<float>(vec.y) / mag;

    return { static_cast<int>(normX), static_cast<int>(normY) };
}

[[nodiscard]] inline SDL_Point diffNormal(
    const SDL_Point & from,
    const SDL_Point & to,
    const SDL_Point & returnOnError = { 0, 0 }) noexcept
{
    return normalize(difference(from, to), returnOnError);
}

namespace util
{

    namespace color
    {

        using Diff_t = std::int_fast32_t;

        // color value diffs

        template <typename T = Diff_t>
        [[nodiscard]] constexpr T diff(const Uint8 left, const Uint8 right) noexcept
        {
            return (static_cast<T>(left) - static_cast<T>(right));
        }

        template <typename T = Diff_t>
        [[nodiscard]] constexpr T diffAbs(const Uint8 left, const Uint8 right) noexcept
        {
            return util::abs(diff<T>(left, right));
        }

        [[nodiscard]] inline constexpr float diffRatio(const Uint8 left, const Uint8 right) noexcept
        {
            return (diffAbs<float>(left, right) / 255.0f);
        }

        // whole color diffs

        [[nodiscard]] inline Diff_t
            diffMagnitudeCount(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            return (
                diffAbs(left.r, right.r) + diffAbs(left.g, right.g) + diffAbs(left.b, right.b) +
                diffAbs(left.a, right.a));
        }

        [[nodiscard]] inline Diff_t
            diffMagnitudeCountOpaque(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            return (diffAbs(left.r, right.r) + diffAbs(left.g, right.g) + diffAbs(left.b, right.b));
        }

        [[nodiscard]] inline float
            diffMagnitude(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            return (
                static_cast<float>(diffMagnitudeCount(left, right)) / static_cast<float>(255 * 4));
        }

        [[nodiscard]] inline float
            diffMagnitudeOpaque(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            return (
                static_cast<float>(diffMagnitudeCountOpaque(left, right)) /
                static_cast<float>(255 * 3));
        }

        [[nodiscard]] inline float
            diffEuclid(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            const Diff_t diffRed{ diffAbs(left.r, right.r) };
            const Diff_t diffGrn{ diffAbs(left.g, right.g) };
            const Diff_t diffBlu{ diffAbs(left.b, right.b) };
            const Diff_t diffAlp{ diffAbs(left.a, right.a) };

            const double diff{ std::sqrt(static_cast<double>(
                (diffRed * diffRed) + (diffGrn * diffGrn) + (diffBlu * diffBlu) +
                (diffAlp * diffAlp))) };

            const double ratioCorrection{ std::sqrt(static_cast<double>(255 * 255 * 4)) };

            return static_cast<float>(diff / ratioCorrection);
        }

        [[nodiscard]] inline float diffEuclidOpaque(SDL_Color left, SDL_Color right) noexcept
        {
            const Diff_t diffRed{ diffAbs(left.r, right.r) };
            const Diff_t diffGrn{ diffAbs(left.g, right.g) };
            const Diff_t diffBlu{ diffAbs(left.b, right.b) };

            const double diff{ std::sqrt(static_cast<double>(
                (diffRed * diffRed) + (diffGrn * diffGrn) + (diffBlu * diffBlu))) };

            const double ratioCorrection{ std::sqrt(static_cast<double>(255 * 255 * 3)) };

            return static_cast<float>(diff / ratioCorrection);
        }

        // whole color diffs (that attempt to look correct to the human eye)

        // from https://www.compuphase.com/cmetric.htm
        [[nodiscard]] inline float
            diffWeightedEuclidOpaque(const SDL_Color & left, const SDL_Color & right) noexcept
        {
            const Diff_t avgRed{ (static_cast<Diff_t>(left.r) + static_cast<Diff_t>(right.r)) / 2 };

            const Diff_t diffRed{ diff(left.r, right.r) };
            const Diff_t diffGrn{ diff(left.g, right.g) };
            const Diff_t diffBlu{ diff(left.b, right.b) };

            const Diff_t bitsRed{ ((512 + avgRed) * diffRed * diffRed) >> 8 };
            const Diff_t bitsGreen{ 4 * diffGrn * diffGrn };
            const Diff_t bitsBlue{ ((767 - avgRed) * diffBlu * diffBlu) >> 8 };

            const double diff{ std::sqrt(static_cast<double>(bitsRed + bitsGreen + bitsBlue)) };
            const double ratioCorrection{ std::sqrt(584970.0) };

            return static_cast<float>(diff / ratioCorrection);
        }

        // blends

        [[nodiscard]] inline constexpr Uint8
            blend(const float ratio, const Uint8 from, const Uint8 to) noexcept
        {
            return static_cast<Uint8>(
                util::mapRatioTo(ratio, static_cast<float>(from), static_cast<float>(to)));
        }

        [[nodiscard]] inline SDL_Color
            blend(const float ratio, const SDL_Color & from, const SDL_Color & to) noexcept
        {
            return SDL_Color{ blend(ratio, from.r, to.r),
                              blend(ratio, from.g, to.g),
                              blend(ratio, from.b, to.b),
                              blend(ratio, from.a, to.a) };
        }
    } // namespace color

} // namespace util

#endif // SDLUTIL_HPP_INCLUDED
