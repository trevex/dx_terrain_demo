/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "DXShaderHelper.h"

void LoadPrecompiledShader(std::wstring& filename, ID3DX11Effect*& fx)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, static_cast<SDXRenderInfo*>(g_RenderInfo)->device, &fx));
}