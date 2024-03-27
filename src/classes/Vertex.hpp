#ifndef VERTEX_HPP
#define VERTEX_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

extern const glm::vec3 modelColor;

namespace Assets
{
    class Vertex 
    {
    public:
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 texCoords{};
		glm::vec4 color{modelColor, 1.0f};

		bool operator==(const Vertex& other) const
		{
			return position == other.position &&
				   normal == other.normal &&
				   texCoords == other.texCoords;
		}
    };
}

namespace std
{
	template <>
	struct hash<Assets::Vertex>
	{
		size_t operator()(Assets::Vertex const& vertex) const
		{
			auto temp = ((hash<glm::vec3>()(vertex.position) ^
					(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoords) << 1); 
			return temp;
				
		}
	};
}

#endif
