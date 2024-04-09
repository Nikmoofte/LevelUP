#ifndef MESH_HPP
#define MESH_HPP

#include <future>
#include <vector>
#include <string>
#include <memory>

#include <Vertex.hpp>
#include <VBO/VBO.hpp>
#include <VAO/VAO.hpp>
#include <EBO/EBO.hpp>

#include <glm/mat4x4.hpp>

namespace Viewer
{
	class ShaderProg;
}
namespace Assets 
{
    class Mesh 
    {
        public:
		Mesh(const std::string& pathToObj, Viewer::ShaderProg&& prog);
		Mesh(const std::string& pathToObj, const Viewer::ShaderProg& prog);
		Mesh(const std::string& pathToObj, const std::string& pathToVertShader, const std::string& pathToFragShader);
		Mesh(const std::string& path, const std::string& pathToVertShader, const std::string& pathToFragShader, glm::mat4&& modelTransform);

		void Wait();
		
		std::vector<Vertex>& getVertices() { return vertices; }
		uint32_t getVerticesSize() const { return vertices.size() * sizeof(Vertex); }
		std::vector<uint32_t>& getIndecies() { return indices; }
		uint32_t getIndeciesSize() const { return indices.size(); }

		Viewer::ShaderProg& GetShaderProg() const;
		const glm::mat4& getModelMat() const { return modelTransform; }
		const glm::mat4& setModelMat(const glm::mat4& model) { return modelTransform = model; }
		
		friend class Renderer;
	private:
		glm::mat4 modelTransform = glm::mat4(1.f);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::future<void> loader{};

		VAO vao;
		VBO vbo;
		EBO ebo;
		std::unique_ptr<Viewer::ShaderProg> prog;

		void Load(const std::string& path);
    };
}

#endif