/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "TestState.h"

#include <boost/lexical_cast.hpp>

TestState::CubeMesh* TestState::m_cube1 = 0;
TestState::CubeMesh* TestState::m_cube2 = 0;
TestState::GroundMesh* TestState::m_ground = 0;
//CFreeCamera* TestState::m_camera = 0;
CTexture* TestState::m_texture = 0;
CTexture* TestState::m_grass = 0;
CTextureCube* TestState::m_sky = 0;
CLight* TestState::m_light = 0;
CLight* TestState::m_pointlight = 0;
CLight* TestState::m_spotlight = 0;
TestState::SkyMesh* TestState::m_skybox = 0;
TestState::Player* TestState::m_player = 0;
CFontWrapper* TestState::m_font = 0;
float TestState::m_framedelta = 0.0f;

bool TestState::update(const float &delta)
{
	static float lightMove;
	static bool lightFlag;
	const float lightFactor = 0.01f;
	lightMove += delta * lightFactor;
	if (lightMove > 20.0f)
	{
		lightMove = 0.0f;
		lightFlag = !lightFlag;
	}
	m_framedelta = delta;
	//m_camera->update(delta);
	m_player->update(delta);
	if (lightFlag) m_pointlight->LightPosition.x -= delta * lightFactor;
	else m_pointlight->LightPosition.x += delta * lightFactor;
	return true;
}

bool TestState::enter(void* options) 
{
	m_texture = new CTexture();
	std::wstring boxfile = L"../GameData/Textures/woodcrate01.dds";
	m_texture->load(boxfile);
	m_sky = new CTextureCube();
	std::wstring skyfile = L"../GameData/Textures/skybox.dds";
	m_sky->load(skyfile);
	m_grass = new CTexture();
	std::wstring grassfile = L"../GameData/Textures/grass.dds";
	m_grass->load(grassfile);
	m_skybox = new SkyMesh(new CSkySphereGeometry(10, 3), new CSkyBoxMaterial(m_sky));
	m_cube1 = new CubeMesh(CCubeGeometry::getInstance(), new CBasicMaterial(m_texture));
	m_cube1->translate(glm::vec3(4.95f, 0.0f, 4.95f));
	m_cube2 = new CubeMesh(CCubeGeometry::getInstance(), new CBasicMaterial(m_texture));
	m_cube2->translate(glm::vec3(-2.0f, 0.0f, 3.0f));
	m_ground = new GroundMesh(CQuadGeometry::getInstance(), new CBasicMaterial(m_grass));
	m_ground->translate(glm::vec3(0.0f, -1.0f, 0.0f));
	std::string fontname = "Arial";
	m_font = new CFontWrapper(fontname);
	//m_camera = new CFreeCamera();
	m_player = new Player();
	m_light = new CLight();
	m_pointlight = new CLight();
	m_pointlight->Type = POINTLIGHT;
	m_pointlight->LightPosition = glm::vec3(-4.0f, 8.0f, 2.0f);
	m_pointlight->LightColor = glm::vec3(1.0f, 0.0f, 1.0f);
	m_pointlight->LightRange = glm::vec4(10.0f, 10.0f, 10.0f, 10.0f);
	m_spotlight = new CLight();
	m_spotlight->Type = POINTLIGHT;
	m_spotlight->LightPosition = glm::vec3(4.0f, 3.0f, -2.0f);
	m_spotlight->LightColor = glm::vec3(1.0f, 1.0f, 0.0f);
	m_spotlight->LightRange = glm::vec4(4.0f, 5.0f, 2.0f, 5.0f);
	m_spotlight->LightDirection = glm::vec3(1.0f, -1.0f, 1.0f);
	m_player->attach(m_cube1);

	return true;
}

bool TestState::leave(void)
{
	delete m_pointlight;
	m_pointlight = 0;
	delete m_spotlight;
	m_spotlight = 0;
	delete m_light;
	m_light = 0;
	delete m_cube1;
	m_cube1 = 0;
	delete m_cube2;
	m_cube2 = 0;
	delete m_font;
	m_font = 0;
	//delete m_camera;
	//m_camera = 0;
	delete m_ground;
	m_ground = 0;
	delete m_player;
	m_player = 0;
	return true;
}

void TestState::renderGeometry(void)
{
	m_cube1->render();
	m_cube2->render();
	m_ground->render();
}

void TestState::renderLight(void)
{
	m_light->render();
	m_pointlight->render();
	m_spotlight->render();
}

void TestState::renderForward(void)
{
	m_skybox->render();
}

void TestState::renderPost(void)
{
	m_font->drawString("d: "+boost::lexical_cast<std::string>(m_framedelta), 16.0f, 5.0f, 5.0f, 0xff0000ff);
}