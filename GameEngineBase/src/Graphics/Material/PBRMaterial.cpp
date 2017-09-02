#include "ge.h"
#include "PBRMaterial.h"

#include "System/Memory.h"
#include "Backend/API/APIShaderResource.h"

namespace gebase { namespace graphics {

	Texture2D* PBRMaterial::s_PreintegratedFG = nullptr;

	PBRMaterial::PBRMaterial(Shader* shader) : Material(shader)
	{
		if (s_PreintegratedFG == nullptr)
			s_PreintegratedFG = Texture2D::CreateFromFile("PreintegratedFG", "res/pbr/PreintegratedFG.bmp");

		setUniform("u_UsingNormalMap", 0.0f);
		setTexture("u_PreintegratedFG", s_PreintegratedFG);

		setUniform("u_UsingAlbedoMap", 0.0f);
		setUniform("u_AlbedoColor", math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

		setUniform("u_UsingSpecularMap", 0.0f);
		setUniform("u_SpecularColor", math::Vector3f(1.0f, 1.0f, 1.0f));

		setUniform("u_UsingGlossMap", 0.0f);
		setUniform("u_GlossColor", 0.8f);
	}

	PBRMaterial::~PBRMaterial()
	{
		gedel m_Shader;
	}

	bool PBRMaterial::EmployRenderAPI(RenderAPI api)
	{
		if (!Material::EmployRenderAPI(api))
			return false;

		if (!s_PreintegratedFG->EmployRenderAPI(api))
			return false;

		if (getAlbedoMap())
			if (!getAlbedoMap()->EmployRenderAPI(api))
				return false;

		if (getNormalMap())
			if (!getNormalMap()->EmployRenderAPI(api))
				return false;

		if (getSpecularMap())
			if (!getSpecularMap()->EmployRenderAPI(api))
				return false;

		if (getGlossMap())
			if (!getGlossMap()->EmployRenderAPI(api))
				return false;

		if (getEnvironmentMap())
			if (!getEnvironmentMap()->EmployRenderAPI(api))
				return false;

		return true;
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

	Texture* PBRMaterial::getMap(const String& name)
	{
		API::ShaderResourceDeclaration* declaration = FindResourceDeclaration(name);

		if (!declaration)
		{
			std::cout << "[PBRMaterial] getMap() - Declaration is null, " << name.c_str() << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
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

	//PBR MATERIAL INSTANCE START

	PBRMaterialInstance::PBRMaterialInstance(PBRMaterial* material) : MaterialInstance(material)
	{

	}

	bool PBRMaterialInstance::EmployRenderAPI(RenderAPI api)
	{
		return MaterialInstance::EmployRenderAPI(api);
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

} }