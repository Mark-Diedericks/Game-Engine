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

		//GET MAPS
		Texture* getAlbedoMap();
		Texture* getSpecularMap();
		Texture* getNormalMap();
		Texture* getGlossMap();
	};

	class GE_API PBRMaterialInstance : public MaterialInstance
	{
	public:
		PBRMaterialInstance(PBRMaterial* material);

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