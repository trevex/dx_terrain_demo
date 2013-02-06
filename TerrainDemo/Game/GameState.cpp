/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "GameState.h"

#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include "../Graphics/GeometryData.h"


#define LIGHT_COUNT 10

CLight*						GameState::m_dirLight = 0;
CLight*						GameState::m_pointLights = 0;
CTexture*					GameState::m_crate = 0;
CTexture*					GameState::m_grass = 0;
CTextureCube*				GameState::m_sky = 0;
GameState::CubeMesh*		GameState::m_playerCube = 0;
GameState::CubeMesh*		GameState::m_sceneCube = 0;
GameState::GroundMesh*		GameState::m_ground = 0;
GameState::SkyMesh*			GameState::m_skybox = 0;
CFontWrapper*				GameState::m_font = 0;
GameState::Player*			GameState::m_player = 0;
float						GameState::m_frameDelta = 0.0f;
glm::vec3*					GameState::m_pointLightDirs = 0;

bool GameState::update(const float &delta)
{
	static float lightMove;
	static bool lightFlag;
	const float lightFactor = 0.005f;
	lightMove += delta * lightFactor;
	if (lightMove > 20.0f)
	{
		lightMove = 0.0f;
		lightFlag = !lightFlag;
	}
	if (lightFlag) 
	{
		for (int i = 0; i < LIGHT_COUNT; i++) m_pointLights[i].LightPosition -= (delta * lightFactor * m_pointLightDirs[i]);
	}
	else
	{
		for (int i = 0; i < LIGHT_COUNT; i++) m_pointLights[i].LightPosition += (delta * lightFactor * m_pointLightDirs[i]);
	}

	m_player->update(delta);
	m_frameDelta = delta;

	return true;
}

bool GameState::enter(void* options) 
{
	m_crate = new CTexture();
	std::wstring crateFile = L"../GameData/Textures/woodcrate01.dds";
	m_crate->load(crateFile);

	m_sky = new CTextureCube();
	std::wstring skyFile = L"../GameData/Textures/skybox.dds";
	m_sky->load(skyFile);

	m_grass = new CTexture();
	std::wstring grassFile = L"../GameData/Textures/grass.dds";
	m_grass->load(grassFile);

	m_skybox = new SkyMesh(new CSkySphereGeometry(10, 3), new CSkyBoxMaterial(m_sky));
	m_playerCube = new CubeMesh(CCubeGeometry::getInstance(), new CBasicMaterial(m_crate));
	m_playerCube->translate(glm::vec3(4.95f, 0.0f, 4.95f));
	m_sceneCube = new CubeMesh(CCubeGeometry::getInstance(), new CBasicMaterial(m_crate));
	m_sceneCube->translate(glm::vec3(-2.0f, 0.0f, 3.0f));
	m_ground = new GroundMesh(CQuadGeometry::getInstance(), new CBasicMaterial(m_grass));
	m_ground->translate(glm::vec3(0.0f, -1.0f, 0.0f));

	std::string fontname = "Arial";
	m_font = new CFontWrapper(fontname);

	m_player = new Player();
	m_dirLight = new CLight();

	m_pointLights = new CLight[LIGHT_COUNT];
	m_pointLightDirs = new glm::vec3[LIGHT_COUNT];
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> pos_dist(-20.0f, 20.0f);
	boost::random::uniform_real_distribution<> color_dist(0.2f, 1.0f);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		m_pointLights[i].Type = POINTLIGHT;
		m_pointLights[i].LightPosition = glm::vec3(pos_dist(gen), 4.0f, pos_dist(gen));
		m_pointLights[i].LightColor = glm::vec3(color_dist(gen), color_dist(gen), color_dist(gen));
		m_pointLights[i].LightRange = glm::vec4(8.0f, 8.0f, 8.0f, 1.0f);
		m_pointLightDirs[i] = glm::normalize(glm::vec3(pos_dist(gen), 0.0f, pos_dist(gen)));
	}
	m_player->attach(m_playerCube);

	CGeometryData test_data;
	test_data.getPositionVector()->push_back(glm::vec3(1.879789f, 1.23123f, 0.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.122789f, 1.21223f, 3.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.879789f, 1.23123f, 0.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.123789f, 1.23123f, 0.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.879789f, 1.12323f, 0.43431231f));
	test_data.getPositionVector()->push_back(glm::vec3(0.879789f, 1.23123f, 2.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(0.129789f, 1.23123f, 2.123231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.879789f, 4.23123f, 2.1231231f));
	test_data.getPositionVector()->push_back(glm::vec3(1.879789f, 4.23123f, 2.1231231f));
	test_data.removeDuplicates();

	return true;
}

bool GameState::leave(void)
{
	delete m_dirLight;  
	delete[] m_pointLights;
	delete m_crate;
	delete m_grass;
	delete m_sky;
	delete m_playerCube;
	delete m_sceneCube;
	delete m_ground;
	delete m_skybox;
	delete m_font;
	delete m_player;
	return true;
}

void GameState::renderGeometry(void)
{
	m_playerCube->render();
	m_sceneCube->render();
	m_ground->render();
}

void GameState::renderLight(void)
{
	m_dirLight->render();
	for (int i = 0; i < LIGHT_COUNT; i++) m_pointLights[i].render();
}

void GameState::renderForward(void)
{
	m_skybox->render();
}

void GameState::renderPost(void)
{
	m_font->drawString("d: "+boost::lexical_cast<std::string>(m_frameDelta), 16.0f, 5.0f, 5.0f, 0xff0000ff);
}