/**********************************************************************************
*\file         RenderResourceManager.cpp
*\brief        RenderResourceManager.cpp
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "RenderResourceManager.h"

#include "rapidjson/document.h"

RenderResourceManager::RenderResourceManager()
{
	m_Models["Quad"] = MeshBuilder::Build2DMesh();
	m_Models["Plane"] = m_Models["Quad"];
	m_Models["Screen"] = MeshBuilder::BuildScreenMesh();

	const std::vector<glm::vec3> skyboxVerts = {
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },

		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },

		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },

		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },

		{ -1.0f,  1.0f, -1.0f },
		{ 1.0f,  1.0f, -1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f },

		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f }
	};

	// Create vbo for debug lines
	glCreateBuffers(1, &m_SkyboxVBO);
	glNamedBufferStorage(m_SkyboxVBO, sizeof(glm::vec3) * skyboxVerts.size(), skyboxVerts.data(), GL_DYNAMIC_STORAGE_BIT);
}

RenderResourceManager::~RenderResourceManager()
{
	// Clean up shaders
	for (std::pair<std::string, ShaderProgram> shader : m_Shaders)
		shader.second.DeleteShader();

	m_Shaders.clear();

	glDeleteTextures(1, &m_Skybox);
	glDeleteBuffers(1, &m_SkyboxVBO);

	// Clean up fonts
	UnloadAllFonts();

	// Clean up textures
	UnloadAllTextures();

	// Clean up materials
	UnloadAllMaterials();

	// Clean up models
	UnloadAllMeshes();
}

void RenderResourceManager::LoadShader(const std::string& Name, const std::string& Vert, const std::string& Frag)
{
	if (m_Shaders.find(Name) == m_Shaders.end())
		m_Shaders[Name] = ShaderBuilder::CreateShader(Vert, Frag);
}

void RenderResourceManager::UnloadAllFonts()
{
	for (auto& pair : m_Fonts)
		glDeleteTextures(1, &pair.second.GetTexture());
}

void RenderResourceManager::UnloadAllTextures()
{
	for (auto& pair : m_Textures)
		glDeleteTextures(1, &pair.second);

	m_Textures.clear();
}

void RenderResourceManager::UnloadAllMeshes()
{
	for (auto& model : m_Models)
		model.second.RemoveAllSubMesh();

	m_Models.clear();
}

void RenderResourceManager::UnloadAllMaterials()
{
	m_Materials.clear();
}

void RenderResourceManager::LoadFonts(const std::string& Name, const std::string& File)
{
	GLuint texture = TextureLoader::LoadDDSAtlas(File + ".dds");

	std::ifstream ifs(File + ".json");

	if (ifs.is_open())
	{
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		rapidjson::Document file;
		file.Parse(buffer.str().c_str());

		std::map<char, Font::Letter> font;

		char id;
		float width, height;
		float xoffset, yoffset;
		int xadvance;
		int x, y;

		const rapidjson::Value& letters = file["chars"];

		for (const auto& letter : letters.GetArray())
		{
			id = static_cast<char>(letter["id"].GetInt());
			width = letter["width"].GetFloat();
			height = letter["height"].GetFloat();
			xoffset = letter["xoffset"].GetFloat();
			yoffset = letter["yoffset"].GetFloat();
			xadvance = letter["xadvance"].GetInt();
			x = letter["x"].GetInt();
			y = letter["y"].GetInt();

			font[id] = Font::Letter{ glm::vec2{x, y}, glm::vec2{width, height}, glm::vec2{xoffset, yoffset}, xadvance };
		}

		m_Fonts[Name] = Font{ 512, texture, font };
	}
}

std::string RenderResourceManager::LoadTextures(const std::string& Texture, const std::string& File, const bool& GammaCorrect)
{
	if (m_Textures.find(Texture) == m_Textures.end())
	{
		GLuint id = TextureLoader::LoadDDSTexture(File, GammaCorrect);

		if (id != 0)
			m_Textures[Texture] = id;
	}

	return Texture;
}

void RenderResourceManager::UnloadTextures(const std::string& Mesh)
{
	glDeleteTextures(1, &m_Textures[Mesh]);
	m_Textures.erase(Mesh);
}

void RenderResourceManager::LoadSkyboxTexture(const std::vector<std::string>& Textures)
{
	if (m_Skybox == 0)
		m_Skybox = TextureLoader::LoadDDSSkyboxTexture(Textures);
}

void RenderResourceManager::Load3DMeshNUI(const std::string& Mesh, const std::string& File)
{
	if (m_Models.find(Mesh) == m_Models.end())
		m_Models[Mesh] = MeshBuilder::BuildMeshFromNUI(File);
}

void RenderResourceManager::LoadMaterialNUI(const std::string& material_name, NUILoader::TempMaterial& material_data)
{
	if (m_Materials.find(material_name) == m_Materials.end())
	{
		Model::Material mat;

		mat.m_Diffuse = LoadTextures(material_data.m_Diffuse.first, material_data.m_Diffuse.second, true);
		mat.m_Ambient = LoadTextures(material_data.m_Ambient.first, material_data.m_Ambient.second, true);
		mat.m_Specular = LoadTextures(material_data.m_Specular.first, material_data.m_Specular.second, false);
		mat.m_Normal = LoadTextures(material_data.m_Normal.first, material_data.m_Normal.second, false);

		m_Materials[material_name] = mat;
	}
}

void RenderResourceManager::Unload3DMesh(const std::string& Mesh)
{
	if (m_Models.find(Mesh) != m_Models.end())
		m_Models.erase(Mesh);
}

RenderResourceManager& RenderResourceManager::GetInstanced()
{
	static RenderResourceManager rrm;

	return rrm;
}