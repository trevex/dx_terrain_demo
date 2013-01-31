#ifndef TESTSTATE_H_
#define TESTSTATE_H_

#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Graphics/Geometry.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Light.h"
#include "../Graphics/FontWrapper.h"
#include "../Base/FreeCamera.h"
#include "../Base/PlayerController.h"


class TestState
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
	static CLight* m_light;
	static CLight* m_pointlight;
	static CLight* m_spotlight;
	static CTexture* m_texture;
	static CTexture* m_grass;
	static CTextureCube* m_sky;
	static CubeMesh* m_cube1;
	static CubeMesh* m_cube2;
	static GroundMesh* m_ground;
	static SkyMesh* m_skybox;
	static CFontWrapper* m_font;
	static float m_framedelta;
	//static CFreeCamera* m_camera;
	static Player* m_player;
};

#endif
