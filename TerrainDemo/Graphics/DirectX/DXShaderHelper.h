/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXSHADERHELPER_H_
#define DXSHADERHELPER_H_

#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"

#include <D3DX11.h>
#include <d3dx11effect.h>
#include <string>
#include <fstream>
#include <vector>

void LoadPrecompiledShader(std::wstring& filename, ID3DX11Effect*& fx);

#endif