#ifndef REDUCTION_HELPER_HPP
#define REDUCTION_HELPER_HPP

#include <iterator>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <wx/image.h>

#include "color.hpp"

class Image16Bpp
{
    public:
        Image16Bpp(wxImage image);
        const Color16& At(int x, int y) const {return pixels[y * width + x];}
        unsigned int width;
        unsigned int height;
        std::vector<Color16> pixels;
};

/** Represents a set of colors. */
class Palette
{
    public:
        struct PaletteEntryStats
        {
            PaletteEntryStats() : used_count(0), perfect_count(0), error(0) {}
            int used_count;
            int perfect_count;
            unsigned long error;
        };

        struct ColorEntryStats
        {
            ColorEntryStats() : index(0), count(0), error(0) {}
            int index;
            int count;
            unsigned long error;
        };
        Palette(const std::vector<Color16>& _colors = std::vector<Color16>());
        /** Clears the color array */
        void Clear();
        /** Sets array to contain colors passed in */
        void Set(const std::vector<Color16>& _colors);
        /** Sets a color in the array */
        bool Set(unsigned int index, const Color16& color);
        /** Gets color at palette index */
        const Color16& At(int index) const {return colors[index];}
        /** Search palette for color passed in returns the closest palette index that matches the color */
        int Search(const Color16& color) const;
        /** Adds a color to the palette */
        void Add(const Color16& c);
        /** Gets size of this palette */
        unsigned int Size() const {return colors.size();}
        /** Gets colors in palette */
        const std::vector<Color16>& GetColors() const {return colors;}
        mutable std::map<int, PaletteEntryStats> paletteEntryStats;
        /** Cache to speed up repeatedly hit colors */
        mutable std::map<Color16, ColorEntryStats> colorEntryStats;
    protected:
        /** Colors contained in palette with set to prevent duplicates */
        std::vector<Color16> colors;
        std::vector<ColorLAB> labColors;
        std::set<Color16> colorSet;
};

class Image8Bpp
{
    public:
        Image8Bpp() {};
        Image8Bpp(const Image16Bpp& image, std::shared_ptr<Palette> global_palette) {Set(image, global_palette);};
        Image8Bpp(const Image16Bpp& image, int colors);
        void Set(const Image16Bpp& image, std::shared_ptr<Palette> global_palette);
        unsigned int Size() const {return width * height / 2;};
        unsigned int width;
        unsigned int height;
        std::vector<unsigned char> pixels;
        std::shared_ptr<Palette> palette;
};

#endif
