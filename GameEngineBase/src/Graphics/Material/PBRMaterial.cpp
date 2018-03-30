#include "ge.h"
#include "PBRMaterial.h"

#include "System/Memory.h"
#include "Graphics/Shader/ShaderResource.h"
#include "Utils/LogUtil.h"

namespace gebase { namespace graphics {

	Texture2D* PBRMaterial::s_PreintegratedFG = nullptr;

	PBRMaterial::PBRMaterial(Shader* shader) : Material(shader)
	{
		if (s_PreintegratedFG == nullptr)
			s_PreintegratedFG = Texture2D::CreateFromFile("PreintegratedFG", "res/pbr/PreintegratedFG.bmp");

		setTexture("u_PreintegratedFG", s_PreintegratedFG);
		setUniform("u_UsingNormalMap", 0.0f);

		setUniform("u_AlbedoColor", math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
		setUniform("u_UsingAlbedoMap", 0.0f);

		setUniform("u_SpecularColor", math::Vector3f(1.0f, 1.0f, 1.0f));
		setUniform("u_UsingSpecularMap", 0.0f);

		setUniform("u_GlossColor", 0.8f);
		setUniform("u_UsingGlossMap", 0.0f);

		setUniform("u_DisplacementColor", 0.0f);
		setUniform("u_UsingDisplacementMap", 0.0f);
		
	}

	PBRMaterial::~PBRMaterial()
	{
		gedel m_Shader;
	}

	void PBRMaterial::setEnvironmentMap(TextureCube* texture)
	{
		setTexture("u_EnvironmentMap", texture);
	}

	void PBRMaterial::setAlbedo(const math::Vector4f& color)
	{
		setUniform("u_AlbedoColor", color);
		setUniform("u_UsingAlbedoMap", 0.0f);
	}

	void PBRMaterial::setSpecular(const math::Vector3f& color)
	{
		setUniform("u_SpecularColor", color);
		setUniform("u_UsingSpecularMap", 0.0f);
	}

	void PBRMaterial::setNormal(bool value)
	{
		setUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
	}

	void PBRMaterial::setGloss(float value)
	{
		setUniform("u_GlossColor", value);
		setUniform("u_UsingGlossMap", 0.0f);
	}

	void PBRMaterial::setDisplacement(float value)
	{
		setUniform("u_DisplacementColor", value);
		setUniform("u_UsingDisplacementMap", 0.0f);
	}

	void PBRMaterial::setAlbedoMap(Texture2D* texture)
	{
		setTexture("u_AlbedoMap", texture);
		setUniform("u_UsingAlbedoMap", 1.0f);
	}

	void PBRMaterial::setSpecularMap(Texture2D* texture)
	{
		setTexture("u_SpecularMap", texture);
		setUniform("u_UsingSpecularMap", 1.0f);
	}

	void PBRMaterial::setNormalMap(Texture2D* texture)
	{
		setTexture("u_NormalMap", texture);
		setUniform("u_UsingNormalMap", 1.0f);
	}

	void PBRMaterial::setGlossMap(Texture2D* texture)
	{
		setTexture("u_GlossMap", texture);
		setUniform("u_UsingGlossMap", 1.0f);
	}

	void PBRMaterial::setDisplacementMap(Texture2D* texture)
	{
		setTexture("u_DisplacementMap", texture);
		setUniform("u_UsingDisplacementMap", 1.0f);
	}

	Texture* PBRMaterial::getMap(const String& name)
	{
		ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			utils::LogUtil::WriteLine("ERROR", "[PBRMaterial] getMap() - Declaration is null, " + name);
#ifdef GE_DEBUG0
			__debugbreak();
#endif
			return nullptr;
		}

		uint slot = declaration->getRegister();
		return m_Textures.size() > slot ? m_Textures[slot] : nullptr;
	}

	TextureCube* PBRMaterial::getEnvironmentMap()
	{
		return (TextureCube*)getMap("u_EnvironmentMap");
	}

	Texture2D* PBRMaterial::getAlbedoMap()
	{
		return (Texture2D*)getMap("u_AlbedoMap");
	}

	Texture2D* PBRMaterial::getSpecularMap()
	{
		return (Texture2D*)getMap("u_SpecularMap");
	}

	Texture2D* PBRMaterial::getNormalMap()
	{
		return (Texture2D*)getMap("u_NormalMap");
	}

	Texture2D* PBRMaterial::getGlossMap()
	{
		return (Texture2D*)getMap("u_GlossMap");
	}

	Texture2D* PBRMaterial::getDisplacementMap()
	{
		return (Texture2D*)getMap("u_DisplacementMap");
	}

	bool PBRMaterial::EmployRenderAPIShader(RenderAPI api)
	{
		if (m_Shader)
			m_Shader = Shader::ConvertRenderAPI(api, m_Shader);

		if (!Material::EmployRenderAPIShader(api))
			return false;		

		return true;
	}

	bool PBRMaterial::EmployRenderAPITexture2D(RenderAPI api)
	{
		m_Shader->Bind();

		if (!Material::EmployRenderAPITexture2D(api))
			return false;

		m_Shader->Bind();

		s_PreintegratedFG = Texture2D::ConvertRenderAPI(api, s_PreintegratedFG);
		setTexture("u_PreintegratedFG", s_PreintegratedFG);

		return true;
	}

	bool PBRMaterial::EmployRenderAPITextureCube(RenderAPI api)
	{
		if (!Material::EmployRenderAPITextureCube(api))
			return false;

		return true;
	}

	bool PBRMaterial::EmployRenderAPITextureDepth(RenderAPI api)
	{
		if (!Material::EmployRenderAPITextureDepth(api))
			return false;

		return true;
	}

	bool PBRMaterial::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		if (!Material::EmployRenderAPIFramebuffer2D(api))
			return false;

		return true;
	}

	bool PBRMaterial::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		if (!Material::EmployRenderAPIFramebufferDepth(api))
			return false;

		return true;
	}

	bool PBRMaterial::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		if (!Material::EmployRenderAPIIndexBuffer(api))
			return false;

		return true;
	}

	bool PBRMaterial::EmployRenderAPIVertexArray(RenderAPI api)
	{
		if (!Material::EmployRenderAPIVertexArray(api))
			return false;

		return true;
	}


	//PBR MATERIAL INSTANCE START

	PBRMaterialInstance::PBRMaterialInstance(PBRMaterial* material) : MaterialInstance(material)
	{

	}

	void PBRMaterialInstance::setEnvironmentMap(TextureCube* texture)
	{
		setTexture("u_EnvironmentMap", texture);
	}

	void PBRMaterialInstance::setAlbedo(const math::Vector4f& color)
	{
		setUniform("u_AlbedoColor", color);
		setUniform("u_UsingAlbedoMap", 0.0f);
	}

	void PBRMaterialInstance::setSpecular(const math::Vector3f& color)
	{
		setUniform("u_SpecularColor", color);
		setUniform("u_UsingSpecularMap", 0.0f);
	}

	void PBRMaterialInstance::setNormal(bool value)
	{
		setUniform("u_UsingNormalMap", value ? 1.0f : 0.0f);
	}

	void PBRMaterialInstance::setGloss(float value)
	{
		setUniform("u_GlossColor", value);
		setUniform("u_UsingGlossMap", 0.0f);
	}

	void PBRMaterialInstance::setAlbedoMap(Texture2D* texture)
	{
		setTexture("u_AlbedoMap", texture);
		setUniform("u_UsingAlbedoMap", 1.0f);
	}

	void PBRMaterialInstance::setSpecularMap(Texture2D* texture)
	{
		setTexture("u_SpecularMap", texture);
		setUniform("u_UsingSpecularMap", 1.0f);
	}

	void PBRMaterialInstance::setNormalMap(Texture2D* texture)
	{
		setTexture("u_NormalMap", texture);
		setUniform("u_UsingNormalMap", 1.0f);
	}

	void PBRMaterialInstance::setGlossMap(Texture2D* texture)
	{
		setTexture("u_GlossMap", texture);
		setUniform("u_UsingGlossMap", 1.0f);
	}

	bool PBRMaterialInstance::EmployRenderAPIShader(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPIShader(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPITexture2D(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPITexture2D(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPITextureCube(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPITextureCube(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPITextureDepth(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPITextureDepth(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPIFramebuffer2D(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPIFramebuffer2D(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPIFramebufferDepth(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPIFramebufferDepth(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPIIndexBuffer(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPIIndexBuffer(api))
			return false;

		return true;
	}

	bool PBRMaterialInstance::EmployRenderAPIVertexArray(RenderAPI api)
	{
		if (!MaterialInstance::EmployRenderAPIVertexArray(api))
			return false;

		return true;
	}


} }