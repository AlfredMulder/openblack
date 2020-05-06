/*****************************************************************************
 * Copyright (c) 2018-2020 openblack developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/openblack/openblack
 *
 * openblack is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "Util/Hash.h"

#include <memory>
#include <string>
#include <typeindex>
#include <vector>

namespace openblack
{

namespace graphics {
	class Texture2D;
}

class FileStream;
class FileSystem;

using codepoint_t = char16_t;

/**
 * A font in Black & White consists of two files, a .fnt and a .met.
 * .met contains all the meta data about the font and it's glyphs.
 * Where a .fnt contains run length encoded 1 bit bitmap data.
 */
class Font
{
public:
	struct Glyph
	{
		codepoint_t codepoint;
		uint16_t width;
		int16_t xOffset;
		uint16_t padding;
		float fXoffset;
		float fWidth;
		float unk2;
		uint32_t data_offset;
		uint32_t data_size;

		float U0;
		float U1;
		float V0;
		float V1;
	};

public:
	void LoadFromFile(FileSystem& filesystem, const std::string& name);

	std::string GetName() const { return _name; }
	uint32_t GetSize() const { return _size; }
	const std::vector<Glyph>& GetGlyphs() const { return _glyphs; }
	const graphics::Texture2D& GetAtlasTexture() const { return *_atlasTexture; }

	const Glyph* FindGlyphNoFallback(codepoint_t glyph) const;

private:
	uint32_t _size;
	std::string _name;
	std::vector<Glyph> _glyphs;
	std::vector<codepoint_t> _glyphLookup;

	std::unique_ptr<graphics::Texture2D> _atlasTexture;

	void buildFontAtlas(uint8_t* fontData);
	void renderGlyph(uint8_t* glyphData, size_t size, uint16_t width, uint8_t* dest, size_t stride);
};

// WIP stuff:

/*struct Glyph {
	codepoint_t codepoint;
	unsigned int width;
	unsigned int height;
	unsigned int height_offset; // idk
	float unk0;
	float unk1;
	float unk2;
};

class Font
{
	friend class FontAtlas;

public:
	Font(const std::string& filename);
	~Font() = default;

	std::string GetName() const;
	unsigned int GetSize() const;

protected:

	// Load a particular glyph's info and retrieves the glyph's bitmap data. (Decompresses)
	std::unique_ptr<uint8_t[]> LoadGlyph(codepoint_t codepoint, Glyph& glyph) const;


private:
	std::string _name;
	unsigned int _size;
	std::unique_ptr<FileStream> _fntFile;

	std::vector<Glyph> _glyphs;
	std::vector<codepoint_t> _glyphLookup;
};*/
} // namespace openblack

namespace std {
template<>
struct hash<openblack::Font> {
	size_t operator()(const openblack::Font& font) const {
		size_t hash = std::hash<std::type_index>()(std::type_index(typeid(openblack::Font)));
		openblack::hash_combine(hash, font.GetName());
		openblack::hash_combine(hash, font.GetSize());
		return hash;
	}
};
}
