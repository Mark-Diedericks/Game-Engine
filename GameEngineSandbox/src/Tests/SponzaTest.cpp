#include "SponzaTest.h"

using namespace gebase;
using namespace debug;
using namespace graphics;
using namespace math;
using namespace events;
using namespace entity;
using namespace component;

SponzaTest::SponzaTest()
	: Layer3D(genew Scene())
{
	m_MayaCamera = m_Scene->getCamera();
	m_FPSCamera = genew FPSCamera(Matrix4f::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
}

SponzaTest::~SponzaTest()
{

}

void SponzaTest::OnInit(Renderer3D& renderer, Scene& scene)
{
	String environmentFiles[11] =
	{
		"res/pbr/cubemap/CubeMap0.tga",
		"res/pbr/cubemap/CubeMap1.tga",
		"res/pbr/cubemap/CubeMap2.tga",
		"res/pbr/cubemap/CubeMap3.tga",
		"res/pbr/cubemap/CubeMap4.tga",
		"res/pbr/cubemap/CubeMap5.tga",
		"res/pbr/cubemap/CubeMap6.tga",
		"res/pbr/cubemap/CubeMap7.tga",
		"res/pbr/cubemap/CubeMap8.tga",
		"res/pbr/cubemap/CubeMap9.tga",
		"res/pbr/cubemap/CubeMap10.tga"
	};
	//ENVIRONMENT MAP
	TextureCube* environment = TextureCube::CreateFromVerticalCross(environmentFiles, 11);

	//SKYPLANE
	ShaderDeclaration sd;
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

	//PBR SHADER	
	ShaderDeclaration sdpbr;
	sdpbr.name = "AdvancedLighting";
	sdpbr.opengl = String("/shaders/AdvancedLighting") + ".shader";
	sdpbr.d3d11 = String("/shaders/AdvancedLighting") + ".hlsl";

	Shader* pbrShader = Shader::CreateFromFile(sdpbr);
	ShaderManager::Add(pbrShader);


	//Models parts
	std::vector<std::string> models = { "arch", "backplate", "walls1", "walls2", "walls3", "ceiling", "column1", "column2", "column3", "curtain_blue", "curtain_green", "curtain_red", "details",
										"fabric_blue", "fabric_green", "fabric_red", "floor", "floor2", "chain", "lion", "pole", "roof", "vase", "vase_round", "vase_hanging","flower", "flower2" };
	//Set texture Wrap to work on OpenGL (or there are texture glitches when texture is repeating)
	Texture::setWrap(TextureWrap::REPEAT);
	//Loop
	for (uint i = 0; i < models.size(); i++) {
		PBRMaterial* material = genew PBRMaterial(pbrShader);
		material->setEnvironmentMap(environment);
		{
			TextureLoadOptions options(false, true);
			material->setAlbedoMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/albedo.tga", options));
			material->setSpecularMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/specular.tga", options));
			material->setGlossMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/gloss.tga", options));
			material->setNormalMap(Texture2D::CreateFromFile("res/Sponza/" + models[i] + "/normal.tga", options));
		}

		Model* model = genew Model("res/Sponza/" + models[i] + "/model.spm", genew MaterialInstance(material));
		Entity* entity = genew Entity(model->getMesh(), Matrix4f::Translation(Vector3f(0, 0, 0)) * Matrix4f::Rotate(-90.0f, Vector3f(1.0f, 0.0f, 0.0f)));
		m_Scene->Add(entity);
	}
	//Reset it
	Texture::setWrap(TextureWrap::CLAMP);

	//LIGHT(S)
	LightSetup* lights = genew LightSetup();
	lights->Add(genew Light(Vector3f(0.8f, 0.8f, 0.8f)));
	m_Scene->PushLightSetup(lights);

	DebugMenu::Add("SponzaTest/Light Direction", &lights->getLights()[0]->direction, -1.0f, 1.0f);
	DebugMenu::Add("SponzaTest/Light Intensity", &lights->getLights()[0]->intensity, 0, 100);
}

void SponzaTest::OnTick()
{
	
}

void SponzaTest::OnUpdate(float delta)
{
	// Still OpenGL maths style (column-major)
	Matrix4f vp = m_Scene->getCamera()->getProjectionMatrix() * m_Scene->getCamera()->getViewMatrix();
	m_SkyboxMaterial->setUniform("invViewProjMatrix", math::Matrix4f::Invert(vp));
}

void SponzaTest::OnEvent(gebase::events::Event& event)
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

void SponzaTest::OnRender(Renderer3D& renderer)
{
	Layer3D::OnRender(renderer);
}
