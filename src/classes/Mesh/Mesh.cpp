#include "Mesh.hpp"

#include <unordered_map>

#include "tiny_obj_loader.h"

#include "VAO/VBLayout.hpp"
#include "Viewer/ShaderProg/ShaderProg.hpp"

using Viewer::ShaderProg;
namespace Assets
{
    Mesh::Mesh(const std::string &pathToObj, ShaderProg &&prog) : prog(std::make_unique<ShaderProg>(std::move(prog)))
    {
		loader = std::async(std::launch::async, &Mesh::Load, this, pathToObj);
    }

    Mesh::Mesh(const std::string &pathToObj, const ShaderProg &prog) : prog(std::make_unique<ShaderProg>(prog))
    {
		loader = std::async(std::launch::async, &Mesh::Load, this, pathToObj);
    }

    Mesh::Mesh(
		const std::string& pathToObj, 
		const std::string& pathToVertShader, 
		const std::string& pathToFragShader
	) : Mesh(pathToObj, ShaderProg(pathToVertShader, pathToFragShader))
    {
    }

    Mesh::Mesh(
		const std::string& pathToObj, 
		const std::string& pathToVertShader, 
		const std::string& pathToFragShader, 
		glm::mat4 &&modelTransform
	) : Mesh(pathToObj, pathToVertShader, pathToFragShader)
    {
		this->modelTransform = modelTransform;
    }

    void Mesh::Wait()
    {
        if(loader.valid())
            loader.wait();

		vbo.SetBufferData(getVerticesSize(), getVertices().data(), GL_STATIC_DRAW);

		VBLayout vbl;
		vbl.Push<float>(3);
		vbl.Push<float>(3);
		vbl.Push<float>(2);
		vbl.Push<float>(4);

		vao.setLayout(vbo, vbl);
		ebo.SetBufferData(getIndeciesSize(), getIndecies().data(), GL_STATIC_DRAW);
    }

    Viewer::ShaderProg& Mesh::GetShaderProg() const
    {
        return *prog;
    }

    void Mesh::Load(const std::string& path)
	{
        tinyobj::ObjReaderConfig reader_config;

		tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(path, reader_config)) 
        {
            throw std::runtime_error(reader.Error() + reader.Warning());
        }

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		auto& attrib = reader.GetAttrib();
		for (const auto& shape : reader.GetShapes())
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				size_t vertInd = 3 * index.vertex_index;
				vertex.position = {
					attrib.vertices[vertInd + 0],
					attrib.vertices[vertInd + 1],
					attrib.vertices[vertInd + 2]
				};

				vertInd = 3 * index.normal_index;
				if (!attrib.normals.empty())
				{
					vertex.normal = {
						attrib.normals[vertInd + 0],
						attrib.normals[vertInd + 1],
						attrib.normals[vertInd + 2]
					};
				}
				
				vertInd = 2 * index.texcoord_index;
				if (!attrib.texcoords.empty() && index.texcoord_index >= 0)
				{
					vertex.texCoords = {
						attrib.texcoords[vertInd + 0],
						1.0f - attrib.texcoords[vertInd + 1]
					};
				}

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}