#ifndef _VERTEX_SET_H_
#define _VERTEX_SET_H_

#include "globals.h"

struct VertexSet
{
private:
	Vec3* m_verts;
	int m_nverts;
public:
	VertexSet(Vec3* verts, int nVerts);
	VertexSet(const VertexSet& param);
	virtual VertexSet& operator= (const VertexSet& param);
	virtual ~VertexSet();
	
	void cpy(const VertexSet& param);
	
	Vec3* GetVerts();
	int NumberOfVerts();
};

#endif // _VERTEX_SET_H_