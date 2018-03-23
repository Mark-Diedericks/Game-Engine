#include "Test3D.h"

using namespace gebase;
using namespace graphics;
using namespace math;
using namespace events;
using namespace entity;
using namespace component;

#define DEBUG_MENU(name, var, min, max) debug::DebugMenu::Add("Test3D/" ## name, var, min, max)

Test3D::Test3D()
	: Layer3D(genew Scene())
{
	m_MayaCamera = m_Scene->getCamera();
	m_FPSCamera = genew FPSCamera(Matrix4f::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));

	m_Rotation = 0.0f;
	m_SetUniforms[0] = true;
	m_SetUniforms[1] = true;

	Matrix4f result = Matrix4f::Identity() * Matrix4f::Identity();
}

Test3D::~Test3D()
{
}

Vector3f g_CubeTransform(-10, 10, 0);
Vector3f g_DaggerTransform(0, 20, 0);
Vector4f g_SphereColor(0.0f, 0.0f, 0.0f, 1.0f);
Vector3f g_SphereSpecularColor(1.0f, 1.0f, 0.6f);

float g_DaggerGloss = 0.5f;

Shader* shadowPassShader;
FramebufferDepth* g_DepthBuffer;
TextureDepth* g_ShadowMap;

String materialInputs[5] =
{
	"CastIron",
	"WornWood",
	"GunMetal",
	"ABSRed",
	"Custom"
};

enum Materials : uint
{
	CAST_IRON = 0,
	WORN_WOOD,
	GUN_METAL,
	ABS_RED,
	CUSTOM
};

void Test3D::OnInit(Renderer3D& renderer, Scene& scene)
{
	// Enable this to use FPS camera
	// scene.SetCamera(m_FPSCamera);

	String environmentFiles[11] =
	{
		"/pbr/cubemap/CubeMap0.tga",
		"/pbr/cubemap/CubeMap1.tga",
		"/pbr/cubemap/CubeMap2.tga",
		"/pbr/cubemap/CubeMap3.tga",
		"/pbr/cubemap/CubeMap4.tga",
		"/pbr/cubemap/CubeMap5.tga",
		"/pbr/cubemap/CubeMap6.tga",
		"/pbr/cubemap/CubeMap7.tga",
		"/pbr/cubemap/CubeMap8.tga",
		"/pbr/cubemap/CubeMap9.tga",
		"/pbr/cubemap/CubeMap10.tga"
	};

	TextureCube* environment = TextureCube::CreateFromVerticalCross(environmentFiles, 11);

	API::ShaderDeclaration sd;
	sd.name = "Skybox";
	sd.opengl = String("/shaders/Skybox") + ".shader";
	sd.d3d11 = String("/shaders/Skybox") + ".hlsl";

	Shader* skybox = Shader::CreateFromFile(sd);

	Material* skyboxMaterial = genew Material(skybox);
	skyboxMaterial->setRenderFlag(Material::RenderFlags::DISABLE_DEPTH_TEST);
	skybox->Bind();
	m_SkyboxMaterial = genew MaterialInstance(skyboxMaterial);
	m_SkyboxMaterial->setTexture("u_EnvironmentMap", environment);
	Entity* skyboxEntity = genew Entity(MeshFactory::CreateQuad(-1, -1, 2, 2, m_SkyboxMaterial));
	m_Scene->Add(skyboxEntity);

	API::ShaderDeclaration sdpbr;
	sdpbr.name = "AdvancedLighting";
	sdpbr.opengl = String("/shaders/AdvancedLighting") + ".shader";
	sdpbr.d3d11 = String("/shaders/AdvancedLighting") + ".hlsl";

	Shader* pbrShader = Shader::CreateFromFile(sdpbr);

	ShaderManager::Add(pbrShader);
	PBRMaterial* material = genew PBRMaterial(pbrShader);

	PBRMaterial* castIron = genew PBRMaterial(pbrShader);
	castIron->setEnvironmentMap(environment);
	{
		String path = materialInputs[CAST_IRON] + "/" + materialInputs[CAST_IRON];
		castIron->setAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		castIron->setSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		castIron->setGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		castIron->setNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(castIron);

	PBRMaterial* wornWood = genew PBRMaterial(pbrShader);
	wornWood->setEnvironmentMap(environment);
	{
		String path = materialInputs[WORN_WOOD] + "/" + materialInputs[WORN_WOOD];
		wornWood->setAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		wornWood->setSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		wornWood->setGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		wornWood->setNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(wornWood);

	PBRMaterial* gunMetal = genew PBRMaterial(pbrShader);
	gunMetal->setEnvironmentMap(environment);
	{
		String path = materialInputs[GUN_METAL] + "/" + materialInputs[GUN_METAL];
		gunMetal->setAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		gunMetal->setSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		gunMetal->setGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		gunMetal->setNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(gunMetal);


	PBRMaterial* absRed = genew PBRMaterial(pbrShader);
	absRed->setEnvironmentMap(environment);
	{
		String path = materialInputs[ABS_RED] + "/" + materialInputs[ABS_RED];
		absRed->setAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		absRed->setSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		absRed->setGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		absRed->setNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(absRed);

	PBRMaterial* custom = genew PBRMaterial(pbrShader);
	custom->setEnvironmentMap(environment);
	{
		String path = materialInputs[CUSTOM] + "/" + materialInputs[CUSTOM];
		custom->setAlbedoMap(Texture2D::CreateFromFile("/pbr/" + path + "_Albedo.tga"));
		custom->setSpecularMap(Texture2D::CreateFromFile("/pbr/" + path + "_Specular.tga"));
		custom->setGlossMap(Texture2D::CreateFromFile("/pbr/" + path + "_Gloss.tga"));
		custom->setNormalMap(Texture2D::CreateFromFile("/pbr/" + path + "_Normal.tga"));
	}
	m_Materials.push_back(custom);

	// Texture::SetLoadParameters(0);
	m_DaggerMaterial = genew PBRMaterial(pbrShader);
	m_DaggerMaterial->setEnvironmentMap(environment);
	{
		API::TextureLoadOptions options(false, true);
		m_DaggerMaterial->setAlbedoMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Albedo.tga", options));
		m_DaggerMaterial->setSpecularMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Specular.tga", options));
		m_DaggerMaterial->setGlossMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Gloss.tga", options));
		m_DaggerMaterial->setNormalMap(Texture2D::CreateFromFile("res/Dagger/Textures/Dagger_Normals.tga", options));
	}

	Model* daggerModel = genew Model("/models/Dagger.spm", genew MaterialInstance(m_DaggerMaterial));
	m_Dagger = genew Entity(daggerModel->getMesh(), Matrix4f::Translation(g_DaggerTransform));
	m_Scene->Add(m_Dagger);

	PBRMaterial* cubeMaterial = genew PBRMaterial(pbrShader);
	cubeMaterial->setEnvironmentMap(environment);
	Model* cubeModel = genew Model("/models/RoundedCube.spm", genew MaterialInstance(cubeMaterial));
	m_Cube = genew Entity(cubeModel->getMesh(), Matrix4f::Rotate(90.0f, Vector3f(0, 0, 1)) * Matrix4f::Translation(g_CubeTransform));
	m_Scene->Add(m_Cube);

	Model* sphereModel = genew Model("/models/Sphere.spm");

	// Plastics
	for (uint x = 0; x < 10; x++)
	{
		float xx = x * 10.0f;
		float zz = 3.0f;

		float roughness = x / 10.0f;
		Vector3f spec(0.04f, 0.04f, 0.04f);
		Vector4f diffuse(1.0f, 0.0f, 0.0f, 1.0f);

		PBRMaterialInstance* m = genew PBRMaterialInstance(m_Materials[CAST_IRON]);
		m->setAlbedo(diffuse);
		m->setSpecular(spec);
		m->setGloss(1.0f - roughness);
		m->setNormal(false);

		Mesh* mesh = genew Mesh(sphereModel->getMesh());
		mesh->setMaterial(m);

		Entity* sphere = genew Entity(mesh, Matrix4f::Translation(Vector3f(-60 + xx, 2.5f, 90 + zz)) * Matrix4f::Scale(2.0f, 2.0f, 2.0f));
		m_Spheres.push_back(sphere);
		m_Scene->Add(sphere);
	}

	// Metals
	for (uint x = 0; x < 10; x++)
	{
		float xx = x * 10.0f;
		float zz = 0;

		float roughness = x / 10.0f;
		Vector3f spec(1.0f, 1.0f, 1.0f);
		Vector4f diffuse(0.0f, 0.0f, 0.0f, 1.0f);

		PBRMaterialInstance* m = genew PBRMaterialInstance(m_Materials[CAST_IRON]);
		m->setAlbedo(diffuse);
		m->setSpecular(spec);
		m->setGloss(1.0f - roughness);
		m->setNormal(false);

		Mesh* mesh = genew Mesh(sphereModel->getMesh());
		mesh->setMaterial(m);

		Entity* sphere = genew Entity(mesh, Matrix4f::Translation(Vector3f(-60 + xx, 2.5f, 80 + zz)) * Matrix4f::Scale(2.0f, 2.0f, 2.0f));
		m_Spheres.push_back(sphere);
		m_Scene->Add(sphere);
	}

	m_Plane = genew Entity(MeshFactory::CreatePlane(128, 128, Vector3f(0, 1, 0), genew PBRMaterialInstance(m_Materials[CUSTOM])));
	m_Scene->Add(m_Plane);

	LightSetup* lights = genew LightSetup();
	m_Light = genew Light(Vector3f(0.8f, 0.8f, 0.8f));
	lights->Add(m_Light);
	m_Scene->PushLightSetup(lights);

	DEBUG_MENU("Cube", &g_CubeTransform, -100.0f, 100.0f);
	DEBUG_MENU("Light Direction", &lights->getLights()[0]->direction, -1.0f, 1.0f);
	DEBUG_MENU("Light Intensity", &lights->getLights()[0]->intensity, 0, 100);
	DEBUG_MENU("Dagger", &g_DaggerTransform, -50, 50);
	DEBUG_MENU("Dagger Gloss", &g_DaggerGloss, 0.0f, 1.0f);

	// shadowPassShader = Shader::CreateFromFile("Shadow", "shaders/ShadowPass.shader");
	// g_DepthBuffer = FramebufferDepth::Create(2048, 2048);
	// g_ShadowMap = (TextureDepth*)g_DepthBuffer->GetTexture();

	for (uint i = 0; i < 4; i++)
	{
		// m_Materials[i]->SetTexture(g_ShadowMap, 6);
		// m_Materials[i]->SetUniform("u_ShadowMap", 6);
	}
}

void Test3D::OnTick()
{

}

void Test3D::OnUpdate(float delta)
{

 	TransformComponent* cubeTransform = m_Cube->getComponent<TransformComponent>();
 
	Matrix4f transform = Matrix4f::Translate(Vector3f(0, 2.5f, 0)) * Matrix4f::Rotate(m_Rotation, Vector3f(1, 0, 0)) * Matrix4f::Rotate(m_Rotation, Vector3f(0, 1, 0)) * Matrix4f::Rotate(m_Rotation, Vector3f(0, 0, 1));
 	cubeTransform->transform = Matrix4f::Translation(g_CubeTransform) * transform * Matrix4f::Scale(1.4f, 1.4f, 1.4f);

	TransformComponent* dagger = m_Dagger->getComponent<TransformComponent>();
	dagger->transform = Matrix4f::Translation(g_DaggerTransform);
		
 	for (Entity* sphere : m_Spheres)
 	{
		TransformComponent* sphereTransform = sphere->getComponent<TransformComponent>();
	//	sphereTransform->transform = sphereTransform->transform * Matrix4f::Rotate(1.0f, Vector3f(0, 1, 0));
 	}

	// Still OpenGL maths style (column-major)
	Matrix4f vp = m_Scene->getCamera()->getProjectionMatrix() * m_Scene->getCamera()->getViewMatrix();
	m_SkyboxMaterial->setUniform("invViewProjMatrix", math::Matrix4f::Invert(vp));
}

void Test3D::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}

void Test3D::OnEvent(Event& event)
{
	if (event.getEventType() == Event::EventType::KEY_PRESSED)
	{
		KeyPressedEvent* kpe = (KeyPressedEvent*)&event;
		if (kpe->getRepeat() == 0)
		{
			switch (kpe->getKeyCode())
			{
				case GE_KEY_R:
					ShaderManager::Reload("AdvancedLighting");
					break;
				case GE_KEY_C:
					m_Scene->setCamera(m_Scene->getCamera() == m_MayaCamera ? m_FPSCamera : m_MayaCamera);
					break;
			}
		}
	}
	Layer::OnEvent(event);
}
