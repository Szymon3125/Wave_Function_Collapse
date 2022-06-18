#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {

public:
	Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	const std::string& GetName() { return _name; }

	void UploadUniformInt(const std::string& name, int value);

	void UploadUniformFloat(const std::string& name, float value);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

	void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	std::string _name;

	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	uint32_t _rendererId;

};

class ShaderLibrary {

public:
	void Add(const std::shared_ptr<Shader>& shader);
	void Add(const std::shared_ptr<Shader>& shader, const std::string& name);
	std::shared_ptr<Shader> Load(const std::string& filepath);
	std::shared_ptr<Shader> Load(const std::string& filepath, const std::string& name);
	
	std::shared_ptr<Shader> Get(const std::string& name);
	bool Exists(const std::string& name) const;

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders;

};