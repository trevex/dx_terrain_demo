/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXTEXTURECUBE_H_
#define DXTEXTURECUBE_H_

#include <D3D11.h>
#include <string>

class CDXTextureCube
{
public:
	CDXTextureCube(void);
	~CDXTextureCube(void);

	void load(std::wstring& filename);

	ID3D11ShaderResourceView* get(void) const;
private:
	ID3D11ShaderResourceView* m_texture;
};
#endif