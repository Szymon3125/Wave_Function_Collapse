#pragma once

#include <string>
#include <glad/glad.h>	

enum TextureType {
	// None = 0,
	Diffuse,
	Specular
};

class Texture {

public:
	Texture(uint32_t width, uint32_t height);
	Texture(const std::string& path, TextureType type);
	virtual ~Texture();

	const std::string& GetName() { return _name; }
	TextureType GetType() { return _type; }

	uint32_t GetWidth() const { return _width; }
	uint32_t GetHeight() const { return _height; }
	uint32_t GetRendererID() const { return _rendererID; }

	void SetData(void* data, uint32_t size);
	void Bind(uint32_t slot = 0) const;

	bool IsLoaded() const { return _isLoaded; }

	bool operator==(const Texture& other) const {
		return _rendererID == other._rendererID;
	}

private:
	std::string _name;
	TextureType _type;
	std::string _path;

	bool _isLoaded = false;
	uint32_t _width, _height;
	GLenum _internalFormat, _dataFormat;

	uint32_t _rendererID;

};

class TextureLibrary {

public:
	TextureLibrary();

	void Add(const std::shared_ptr<Texture>& texture);
	void Add(const std::shared_ptr<Texture>& texture, const std::string& name);
	std::shared_ptr<Texture> Load(const std::string& filepath, TextureType type);
	std::shared_ptr<Texture> Load(const std::string& filepath, TextureType type, const std::string& name);

	std::shared_ptr<Texture> Get(const std::string& name);
	bool Exists(const std::string& name) const;

private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;

};