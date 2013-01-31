/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef MATERIALBASE_H_
#define MATERIALBASE_H_

#include "MeshInfo.h"

template <class M>
class TMaterial
{
public:
	void prepare() 
	{
		static_cast<M*>(this)->prepare();
	}
	void render(const void* geom_info, const SMeshInfo* mesh_info)
	{
		static_cast<M*>(this)->render(geom_info, mesh_info);
	}
protected:
private:
};

#endif