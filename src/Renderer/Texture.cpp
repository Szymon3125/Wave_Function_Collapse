#include "pch.h"
 
#include "Renderer/Texture.h"

#include "stb/stb_image.h"
#include <glad/glad.h>

Texture::Texture(uint32_t width, uint32_t height) : _width(width), _height(height) {
	_internalFormat = GL_RGBA8;
	_dataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &_rendererID);
	glTextureStorage2D(_rendererID, 1, _internalFormat, _width, _height);

	glTextureParameteri(_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string& path, TextureType type) : _name(path), _path(path), _type(type) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	ASSERT(data, "Faild to load image!");

	_isLoaded = true;
	_width = width;
	_height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	_internalFormat = internalFormat;
	_dataFormat = dataFormat;

	ASSERT(internalFormat & dataFormat, "Format not supported!");

	glGenTextures(1, &_rendererID);
	glBindTexture(GL_TEXTURE_2D, _rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &_rendererID);
}

void Texture::SetData(void* data, uint32_t size) {
	uint32_t bpp = _dataFormat == GL_RGBA ? 4 : 3;
	ASSERT(size == _width * _height * bpp, "Data must be entire texture!");
	glTextureSubImage2D(_rendererID, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(uint32_t slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _rendererID);
}

// ----- Texture Library -----

TextureLibrary::TextureLibrary() {
	_textures = std::unordered_map<std::string, std::shared_ptr<Texture>>();
}

void TextureLibrary::Add(const std::shared_ptr<Texture>& texture) {
	auto& name = texture->GetName();
	Add(texture, name);
}

void TextureLibrary::Add(const std::shared_ptr<Texture>& texture, const std::string& name) {
	ASSERT(!Exists(name), "Texture alredy exists!");
	_textures[name] = texture;
}

std::shared_ptr<Texture> TextureLibrary::Load(const std::string& filepath, TextureType type) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(filepath, type);
	Add(texture);
	return(texture);
}

std::shared_ptr<Texture> TextureLibrary::Load(const std::string& filepath, TextureType type, const std::string& name) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(filepath, type);
	Add(texture, name);
	return texture;
}

std::shared_ptr<Texture> TextureLibrary::Get(const std::string& name) {
	ASSERT(Exists(name), "Texture not found!");
	return _textures[name];
}

bool TextureLibrary::Exists(const std::string& name) const {
	return _textures.find(name) != _textures.end();
}
