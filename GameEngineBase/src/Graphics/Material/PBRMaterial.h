#pragma once

#include "Material.h"

namespace gebase { namespace graphics {

	class GE_API PBRMaterial : public Material
	{
	private:
		static Texture2D* s_PreintegratedFG;

		Texture* getMap(const String& name);
	public:
		PBRMaterial(Shader* shader);
		~PBRMaterial();

		bool EmployRenderAPI(RenderAPI api) override;

		void setEnvironmentMap(TextureCube* texture);
		TextureCube* getEnvironmentMap();

		//STATICS
		void setAlbedo(const math::Vector4f& color);
		void setSpecular(const math::Vector3f& color);
		void setNormal(bool value);
		void setGloss(float value);
		void setDisplacement(float value);

		//MAPS
		void setAlbedoMap(Texture2D* texture);
		void setSpecularMap(Texture2D* texture);
		void setNormalMap(Texture2D* texture);
		void setGlossMap(Texture2D* texture);
		void setDisplacementMap(Texture2D* texture);

		//GET MAPS
		Texture2D* getAlbedoMap();
		Texture2D* getSpecularMap();
		Texture2D* getNormalMap();
		Texture2D* getGlossMap();
		Texture2D* getDisplacementMap();
	};

	class GE_API PBRMaterialInstance : public MaterialInstance
	{
	public:
		PBRMaterialInstance(PBRMaterial* material);

		bool EmployRenderAPI(RenderAPI api) override;

		void setEnvironmentMap(TextureCube* texture);

		//STATICS
		void setAlbedo(const math::Vector4f& color);
		void setSpecular(const math::Vector3f& color);
		void setNormal(bool value);
		void setGloss(float value);

		//MAPS
		void setAlbedoMap(Texture2D* texture);
		void setSpecularMap(Texture2D* texture);
		void setNormalMap(Texture2D* texture);
		void setGlossMap(Texture2D* texture);
	};
	
} }