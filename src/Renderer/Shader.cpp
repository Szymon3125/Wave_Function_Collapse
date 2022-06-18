#include "pch.h"

#include "Renderer/Shader.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type) {
	if (type == "vertex") {
		return GL_VERTEX_SHADER;
	}
	if (type == "geometry") {
		return GL_GEOMETRY_SHADER;
	}
	if (type == "fragment" || type == "pixel") {
		return GL_FRAGMENT_SHADER;
	}

	ASSERT(false, "Unknown shader type!");
	return 0;
}

Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : _name(name) {
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	Compile(sources);
}

Shader::Shader(const std::string& filepath) {
	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);

	// Extract name from filepath
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	_name = filepath.substr(lastSlash, count);
}

Shader::~Shader() {
	glDeleteProgram(_rendererId);
}

void Shader::Bind() const {
	glUseProgram(_rendererId);
}

void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::UploadUniformInt(const std::string& name, int value) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniform1i(location, value);
}

void Shader::UploadUniformFloat(const std::string& name, float value) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniform1f(location, value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
	GLint location = glGetUniformLocation(_rendererId, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string Shader::ReadFile(const std::string& filepath) {
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else {
		LOG_ERROR("Could not open file '{0}'", filepath);
	}
	return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source) {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while(pos != std::string::npos) {
		size_t eol = source.find_first_of("\r\n", pos);
		ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1  :nextLinePos));
	}
	return shaderSources;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
	GLuint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs;
	glShaderIDs.reserve(shaderSources.size());
	for (auto kv : shaderSources) {
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);
		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			LOG_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs.push_back(shader);
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		for (auto id : glShaderIDs) {
			glDeleteShader(id);
		}

		LOG_ERROR("{0}", infoLog.data());
		ASSERT(false, "Shader link failure!");
		return;
	}

	for (auto id : glShaderIDs) {
		glDeleteShader(id);
	}

	_rendererId = program;
}

// ----- Shader Library -----

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader) {
	auto& name = shader->GetName();
	Add(shader, name);
}

void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader, const std::string& name) {
	ASSERT(!Exists(name), "Shader alredy exists!");
	_shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(filepath);
	Add(shader);
	return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath, const std::string& name) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(filepath);
	Add(shader, name);
	return shader;
}
 
std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name) {
	ASSERT(Exists(name), "Shader not found!");
	return _shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
	return _shaders.find(name) != _shaders.end();
}
