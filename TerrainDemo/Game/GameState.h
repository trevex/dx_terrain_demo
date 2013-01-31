/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Graphics/Geometry.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Light.h"
#include "../Graphics/FontWrapper.h"
#include "../Base/PlayerController.h"


class GameState
{
public:
	static bool update(const float &delta);
	static bool enter(void* options);
	static bool leave(void);
	static void renderGeometry(void);
	static void renderLight(void);
	static void renderForward(void);
	static void renderPost(void);
protected:
private:
	typedef CMesh<CCubeGeometry, CBasicMaterial> CubeMesh;
	typedef CMesh<CSkySphereGeometry, CSkyBoxMaterial> SkyMesh;
	typedef CMesh<CQuadGeometry, CBasicMaterial> GroundMesh;
	typedef CPlayerController<CubeMesh> Player;
	static CLight*			m_dirLight;
	static CTexture*		m_crate;
	static CTexture*		m_grass;
	static CTextureCube*	m_sky;
	static CubeMesh*		m_playerCube;
	static CubeMesh*		m_sceneCube;
	static GroundMesh*		m_ground;
	static SkyMesh*			m_skybox;
	static CFontWrapper*	m_font;
	static Player*			m_player;
	static float			m_frameDelta;

	static CLight*			m_pointLights;
	static glm::vec3*		m_pointLightDirs;
	static std::map<unsigned int, CubeMesh*> m_netPlayers;
};

#endif
