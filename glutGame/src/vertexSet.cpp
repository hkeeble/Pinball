#include "vertexSet.h"

#include <iostream>

VertexSet::VertexSet(Vec3* verts, int nVerts)
{
	m_verts = verts;
	m_nverts = nVerts;
}
	
VertexSet::VertexSet(const VertexSet& param)
{
	cpy(param);
}

VertexSet& VertexSet::operator= (const VertexSet& param)
{
	if(this == &param)
		return *this;
	else
	{
		if(m_verts)
			delete [] m_verts;
		cpy(param);
		return *this;
	}
}
	
void VertexSet::cpy(const VertexSet& param)
{
	m_nverts = param.m_nverts;
	m_verts = new Vec3[m_nverts];
	for(int i = 0; i < m_nverts; i++)
		m_verts[i] = param.m_verts[i];
}	

VertexSet::~VertexSet()
{

}

Vec3* VertexSet::GetVerts()
{ 
	return m_verts;
}
	
int VertexSet::NumberOfVerts()
{
	return m_nverts;
}