#include "Stdafx.h"
#include "Printer.h"

// Draw grid
void Printer::Draw()
{
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
	for(UINT x=0;x<m_fVolume.x;x+=10)
		{
		glVertex3f(x, 0.0f, 0.0f);
		glVertex3f(x, m_fVolume.y, 0.0f);
		}
	glVertex3f(m_fVolume.x, 0.0f, 0.0f);
	glVertex3f(m_fVolume.x, m_fVolume.y, 0.0f);
	
	for(UINT y=0;y<m_fVolume.y;y+=10)
	{
	glVertex3f(0.0f, y, 0.0f);
	glVertex3f(m_fVolume.x, y, 0.0f);
	}
	glVertex3f(0.0f, m_fVolume.y, 0.0f);
	glVertex3f(m_fVolume.x, m_fVolume.y, 0.0f);

	glEnd();
}