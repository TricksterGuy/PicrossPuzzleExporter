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
        Image16Bpp(wxImage image, const std::string& _name);
        void GetColors(std::vector<Color>& colors) const;
        void GetColors(std::vector<Color>::iterator& color_ptr) const;
        unsigned int Size() const {return width * height;};
        unsigned int width;
        unsigned int height;
        std::string name;
        std::vector<unsigned short> pixels;
};

class Palette
{
    public:
        Palette() {}
        Palette(const std::vector<Color>& _colors, const std::string& _name);
        void Set(const std::vector<Color>& _colors);
        Color At(int index) const {return colors[index];}
        int Search(const Color& color) const;
        int Search(unsigned short color) const;
        unsigned int Size() const {return colors.size();};
        std::vector<Color> colors;
        std::string name;
    private:
        mutable std::map<Color, int> colorIndexCache;
};

class Image8Bpp
{
    public:
        Image8Bpp() {};
        Image8Bpp(const Image16Bpp& image, std::shared_ptr<Palette> global_palette) {Set(image, global_palette);};
        void Set(const Image16Bpp& image, std::shared_ptr<Palette> global_palette);
        unsigned int Size() const {return width * height / 2;};
        unsigned int width;
        unsigned int height;
        std::string name;
        std::vector<unsigned char> pixels;
        std::shared_ptr<Palette> palette;
};

#endif
