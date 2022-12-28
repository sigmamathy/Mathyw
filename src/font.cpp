#include <Mathyw/font.hpp>
#include <Mathyw/transformation.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mathyw {

Text::Text(Font& font, std::string_view string)
    : font(font)
{
    String(string);
}

void Text::String(std::string_view string)
{
    this->string = string;
    characters.clear();
    characters.reserve(string.size());
    float x = 0.0f, y = 0.0f;
    for (std::size_t i = 0u; i < string.size(); i++)
    {
        Glyph& glyph = font.glyphs.at(string[i]);
        float xpos = x + glyph.bearing[0];
        float ypos = (float)(glyph.bearing[1] - glyph.size[1]);
        float w = (float)glyph.size[0];
        float h = (float)glyph.size[1];

        if (ypos + h > y) y = ypos + h;
        Fmat4 model = Translate(Fvec3(xpos + w / 2.0f, ypos + h / 2.0f, 0.0f)) * Scale(Fvec3(w, -h, 1.0f));
        x += glyph.advance >> 6;
        characters.emplace_back(glyph.texture, model);
    }
    size = Fvec2(x, y);
}

Text& Text::operator=(std::string_view string)
{
    String(string);
    return *this;
}

Font::Font(std::string const& path, int pixel_size)
{
    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_Face face;
    FT_New_Face(ft, path.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 32; c < 128; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        auto& map = face->glyph->bitmap;
        
        std::vector<unsigned char> image;
        image.reserve(map.width * map.rows * 4);

        for (unsigned j = 0; j < map.rows; j++)
            for (unsigned i = 0; i < map.width; i++)
                for (unsigned k = 0; k < 4; k++)
                    image.emplace_back(map.buffer[j * map.width + i]);

        Texture tex(image.data(), Ivec2(map.width, map.rows), 4);
        glyphs.insert(std::make_pair(c, Glyph{
            std::move(tex),
            Ivec2(map.width, map.rows),
            Ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            unsigned(face->glyph->advance.x)
        }));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Glyph& Font::operator[](unsigned char c)
{
    MATHYW_ASSERT(glyphs.count(c), "Character glyph not found at \"Font::operator[]\"");
    return glyphs.at(c);
}

Glyph const& Font::operator[](unsigned char c) const
{
    MATHYW_ASSERT(glyphs.count(c), "Character glyph not found at \"Font::operator[]\"");
    return glyphs.at(c);
}

Text Font::operator[](std::string const& text)
{
    return Text(*this, text);
}

}