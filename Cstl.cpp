#include "stdafx.h"

#include "Cstl.h"
#include "gcode.h"
#include "UI.h"
#include "math.h"

#include <gl/glu.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#define MIN(A,B) ((A)<(B)? (A):(B))
#define MAX(A,B) ((A)>(B)? (A):(B))

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
STL stl;
extern CubeViewUI *cvui;
/*
// Utility functions
void GLText(Vector3f &pos, char *text)
{
	int len, i;
	glRasterPos3fv((GLfloat*) &pos);
	len = (int) strlen(text);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
}
*/
// STL constructor
STL::STL()
{
}

void STL::Read(string filename)
{
	GCode *code = cvui->code;
	if(code == 0)
		return;

	triangles.clear();

	unsigned int count;
	ifstream infile;
	Min.x = Min.y = Min.z = 99999999.0f;
	Max.x = Max.y = Max.z = -99999999.0f;
	try{
		infile.open(filename.c_str(),  ios::in | ios::binary);
		if(!infile.good())
			return;
		// Set window title
		
		cvui->mainWindow->label(filename.c_str()); 
			
		// Ascii ot binary?
		long header;
		infile.read(reinterpret_cast < char * > (&header), sizeof(long));	// Header
		//Check if the header is "soli"
		if(header == 0x696c6f73)
		{
//		ReadAsciiFile();
		return;
		}
		
		infile.seekg(80, ios_base::beg);
		infile.read(reinterpret_cast < char * > (&count), sizeof(unsigned int));	// N_Triangles
		triangles.reserve(count);

		for(UINT i = 0; i < count; i++)
		{
			float a,b,c;
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f N(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Ax(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Bx(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Cx(a,b,c);
			unsigned short xxx;
			infile.read(reinterpret_cast < char * > (&xxx), sizeof(unsigned short));

			Triangle T(N,Ax,Bx,Cx);

			triangles.push_back(T);

			Min.x = MIN(Ax.x, Min.x);			Min.y = MIN(Ax.y, Min.y);			Min.z = MIN(Ax.z, Min.z);			Max.x = MAX(Ax.x, Max.x);			Max.y = MAX(Ax.y, Max.y);			Max.z = MAX(Ax.z, Max.z);			Min.x = MIN(Bx.x, Min.x);			Min.y = MIN(Bx.y, Min.y);
			Min.z = MIN(Bx.z, Min.z);			Max.x = MAX(Bx.x, Max.x);			Max.y = MAX(Bx.y, Max.y);			Max.z = MAX(Bx.z, Max.z);			Min.x = MIN(Cx.x, Min.x);			Min.y = MIN(Cx.y, Min.y);			Min.z = MIN(Cx.z, Min.z);			Max.x = MAX(Cx.x, Max.x);
			Max.y = MAX(Cx.y, Max.y);			Max.z = MAX(Cx.z, Max.z);
		}

	}
	catch (ifstream::failure e)
	{
		cout << "Exception opening/reading file";
		string error = e.what();
	}
	
	OptimizeRotation();
	CalcBoundingBoxAndZoom(code);
}

void STL::CalcBoundingBoxAndZoom(GCode *code)
{
	code->Min = Min;
	code->Max = Max;

	code->Center = (code->Max + code->Min )/2;

	// Find zoom

	float L=0;
	if(code->Max.x - code->Min.x > L)	L = code->Max.x - code->Min.x;
	if(code->Max.y - code->Min.y > L)	L = code->Max.y - code->Min.y;
	if(code->Max.z - code->Min.z > L)	L = code->Max.z - code->Min.z;

	code->zoom= L;
}


void STL::draw()
{

	// polygons

	if(cvui->DisplayPolygonsButton->value())
	{
		glEnable(GL_CULL_FACE);
//		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //define blending factors
		glBegin(GL_TRIANGLES);
		for(UINT i=0;i<triangles.size();i++)
		{
			switch(triangles[i].axis)
				{
				case NEGX:	glColor4f(1,0,0,0.2f); break;
				case POSX:	glColor4f(0.5f,0,0,0.2f); break;
				case NEGY:	glColor4f(0,1,0,0.2f); break;
				case POSY:	glColor4f(0,0.5f,0,0.2f); break;
				case NEGZ:	glColor4f(0,0,1,0.2f); break;
				case POSZ:	glColor4f(0,0,0.3f,0.2f); break;
				default: glColor4f(0.2f,0.2f,0.2f,0.2f); break;
				}
			glNormal3fv((GLfloat*)&triangles[i].N);
			glVertex3fv((GLfloat*)&triangles[i].A);
			glVertex3fv((GLfloat*)&triangles[i].B);
			glVertex3fv((GLfloat*)&triangles[i].C);
		}
		glEnd();
		glDisable(GL_BLEND);
	}

	// WireFrame
	if(cvui->DisplayWireframeButton->value())
	{
		glColor4f(0,1,0,1);
		for(UINT i=0;i<triangles.size();i++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(triangles[i].A.x, triangles[i].A.y, triangles[i].A.z);
			glVertex3f(triangles[i].B.x, triangles[i].B.y, triangles[i].B.z);
			glVertex3f(triangles[i].C.x, triangles[i].C.y, triangles[i].C.z);
			glEnd();
		}
	}
	// normals
	if(cvui->DisplayNormalsButton->value())
	{
		glColor4f(0,0,1,1);
		glBegin(GL_LINES);
		for(UINT i=0;i<triangles.size();i++)
		{
			Vector3f center = (triangles[i].A+triangles[i].B+triangles[i].C)/3.0f;
			glVertex3fv((GLfloat*)&center);
			Vector3f N = center + (triangles[i].N*10);
			glVertex3fv((GLfloat*)&N);
		}
		glEnd();
	}

	// Endpoints
	if(cvui->DisplayEndpointsButton->value())
	{
		glColor4f(1,0,0,1);
		glPointSize(2);
		glBegin(GL_POINTS);
		for(UINT i=0;i<triangles.size();i++)
		{
			glVertex3f(triangles[i].A.x, triangles[i].A.y, triangles[i].A.z);
			glVertex3f(triangles[i].B.x, triangles[i].B.y, triangles[i].B.z);
			glVertex3f(triangles[i].C.x, triangles[i].C.y, triangles[i].C.z);
		}
		glEnd();
	}

	// Make Layers
	UINT LayerNr = 0;

	float zSize = (Max.z-Min.z);
	float z=cvui->CuttingPlaneSlider->value()*zSize+Min.z;
	float zStep = zSize;

	if(cvui->DisplayAllLayers->value())
		{
		z=Min.z;
		zStep = cvui->LayerThicknessSlider->value()*1000;
		}
	while(z<Max.z)
	{
		{
		CuttingPlane plane;
		CalcCuttingPlane(z, plane);
		plane.Draw(z);
		plane.LinkSegments(z);
		plane.Draw(1);

		// inFill
		vector<Vector2f> infill;
		if(cvui->DisplayinFillButton->value())
			{
			plane.CalcInFill(infill, LayerNr, z);
			glColor4f(1,1,0,1);
			glPointSize(5);
			glBegin(GL_LINES);
			for(UINT i=0;i<infill.size();i+=2)
			{
				if(infill.size() > i+1)
				{
				glVertex3f(infill[i  ].x, infill[i  ].y, z);
				glVertex3f(infill[i+1].x, infill[i+1].y, z);
				}
			}
			glEnd();
			}
		glPointSize(1);
		LayerNr++;
		}
	z+=zStep;
	}
}

void STL::CalcCuttingPlane(float where, CuttingPlane &plane)
{
	// intersect lines with plane

	plane.Min.x = Min.x;
	plane.Min.y = Min.y;
	plane.Max.x = Max.x;
	plane.Max.y = Max.y;

	GCode *code = cvui->code;

	UINT pointNr = 0;
	bool foundOne = false;
	for(UINT i=0;i<triangles.size();i++)
	{
		foundOne=false;
		Segment line(-1,-1);
		Vector3f P1 = triangles[i].A;
		Vector3f P2 = triangles[i].B;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(float)(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			line.start = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			foundOne = true;
		}
		P1 = triangles[i].B;
		P2 = triangles[i].C;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(float)(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			if(foundOne)
				line.end = pointNr++;
			else
				line.start = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			if(foundOne)
			{
				plane.lines.push_back(line);
				continue;// next triangle
			}
			foundOne=true;
		}
		P1 = triangles[i].C;
		P2 = triangles[i].A;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			line.end = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			plane.lines.push_back(line);
		}
	}

}

bool InFillHitCompareFunc(const InFillHit& d1, const InFillHit& d2)
{
  return d1.d < d2.d;
}

vector<InFillHit> HitsBuffer;


void CuttingPlane::CalcInFill(vector<Vector2f> &infill, UINT LayerNr, float z)
{
	int c=0;

	float step = cvui->InfillDistanceSlider->value()*1000;

	bool examine = false;

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;

	float rot = cvui->RotationSlider->value()/180.0f*M_PI;
	rot += (float)LayerNr*(float)cvui->InfillRotationPrLayerSlider->value()/180.0f*M_PI;
	Vector2f InfillDirX(cosf(rot), sinf(rot));
	Vector2f InfillDirY(-InfillDirX.y, InfillDirX.x);
	Vector2f Center = (Max+Min)/2.0f;
	
	for(float x = -Length ; x < Length ; x+=step)
	{
		bool examineThis = false;

		HitsBuffer.clear();

		Vector2f P1 = (InfillDirX * Length)+(InfillDirY*x)+ Center;
		Vector2f P2 = (InfillDirX * -Length)+(InfillDirY*x) + Center;

		if(cvui->DisplayDebuginFillButton->value())
		{
		glBegin(GL_LINES);
		glColor3f(0,0.2f,0);
		glVertex3f(P1.x, P1.y, z);
		glVertex3f(P2.x, P2.y, z);
		glEnd();
		}

		if(cvui->DisplayDebugButton->value())
			if(!examine && ((cvui->ExamineSlider->value()-0.5f)*2 * Length <= x))
				{
				examineThis = examine = true;
				glColor3f(1,1,1);				// Draw the line
				glVertex3f(P1.x, P1.y, z);
				glVertex3f(P2.x, P2.y, z);
				}


		for(UINT i=0;i<lines.size();i++)
		{
			Vector2f P3 = vertices[lines[i].start];
			Vector2f P4 = vertices[lines[i].end];

			Vector3f point;
			InFillHit hit;
			if(IntersectXY(P1,P2,P3,P4,hit))
			{
				if(examineThis)
					int a=0;
				HitsBuffer.push_back(hit);
			}
		}
		
		// Sort hits
	  // Sort the vector using predicate and std::sort
			if(examineThis)
				int a=0;
	  std::sort(HitsBuffer.begin(), HitsBuffer.end(), InFillHitCompareFunc);

	  if(examineThis)
	  {
		  glPointSize(4);
		  glBegin(GL_POINTS);
		  for(UINT i=0;i<HitsBuffer.size();i++)
			  glVertex3f(HitsBuffer[0].p.x, HitsBuffer[0].p.y, z);
		  glEnd();
		  glPointSize(1);
	  }

		// Verify hits intregrety
		// Check if hit extists in table
restart_check:
		for(UINT i=0;i<HitsBuffer.size();i++)
			{
			if(examineThis)
				int a=0;
			bool found = false;

			for(UINT j=i+1;j<HitsBuffer.size();j++)
				if( abs(HitsBuffer[i].d - HitsBuffer[j].d) < 0.0001)
					{
					found = true;
					// Delete both points, and continue
					HitsBuffer.erase(HitsBuffer.begin()+j);
					if(i != 0 && i != HitsBuffer.size()-1)	//If we are "Going IN" to solid material, and there's more points, keep one of the points
						HitsBuffer.erase(HitsBuffer.begin()+i);
					goto restart_check;
					}
			if(found)
				continue;
			}		
		
		
		// Sort hits by distance and transfer to InFill Buffer
		if(HitsBuffer.size() != 0 && HitsBuffer.size() % 2)
			continue;	// There's a uneven number of hits, skip this infill line (U'll live)
		c=0;	// Color counter
		while(HitsBuffer.size())
		{
			infill.push_back(HitsBuffer[0].p);
				if(examineThis)
				{
					switch(c)
					{
					case 0: glColor3f(1,0,0); break;
					case 1: glColor3f(0,1,0); break;
					case 2: glColor3f(0,0,1); break;
					case 3: glColor3f(1,1,0); break;
					case 4: glColor3f(0,1,1); break;
					case 5: glColor3f(1,0,1); break;
					case 6: glColor3f(1,1,1); break;
					case 7: glColor3f(1,0,0); break;
					case 8: glColor3f(0,1,0); break;
					case 9: glColor3f(0,0,1); break;
					case 10: glColor3f(1,1,0); break;
					case 11: glColor3f(0,1,1); break;
					case 12: glColor3f(1,0,1); break;
					case 13: glColor3f(1,1,1); break;
					}
					c++;
					glPointSize(10);
					glBegin(GL_POINTS);
					glVertex3f(HitsBuffer[0].p.x, HitsBuffer[0].p.y, z);
					glEnd();
					glPointSize(1);
				}
			HitsBuffer.erase(HitsBuffer.begin());
		}
	}
}


#define SMALL_NUM  0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product (2D)

//===================================================================

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is not inside S
int inSegment( const Vector2f &P, const Vector2f &p1, const Vector2f &p2)
{
	if (p1.x != p2.x) {    // S is not vertical
		if (p1.x <= P.x && P.x <= p2.x)
			return 1;
		if (p1.x >= P.x && P.x >= p2.x)
			return 1;
	}
	else {    // S is vertical, so test y coordinate
		if (p1.y <= P.y && P.y <= p2.y)
			return 1;
		if (p1.y >= P.y && P.y >= p2.y)
			return 1;
	}
	return 0;
}
// intersect2D_2Segments(): the intersection of 2 finite 2D segments
//    Input:  two finite segments S1 and S2
//    Output: *I0 = intersect point (when it exists)
//            *I1 = endpoint of intersect segment [I0,I1] (when it exists)
//    Return: 0=disjoint (no intersect)
//            1=intersect in unique point I0
//            2=overlap in segment from I0 to I1
int intersect2D_Segments( const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, Vector2f &I0, Vector2f &I1, float &t0, float &t1 )
{
	Vector2f    u = p2 - p1;
	Vector2f    v = p4 - p3;
	Vector2f    w = p1 - p3;
	float    D = perp(u,v);

	// test if they are parallel (includes either being a point)
	if (fabs(D) < SMALL_NUM) {          // S1 and S2 are parallel
		if (perp(u,w) != 0 || perp(v,w) != 0) {
			return 0;                   // they are NOT collinear
		}
		// they are collinear or degenerate
		// check if they are degenerate points
		float du = dot(u,u);
		float dv = dot(v,v);
		if (du==0 && dv==0) {           // both segments are points
			if (p1 != p3)         // they are distinct points
				return 0;
			I0 = p1;                // they are the same point
			return 1;
		}
		if (du==0) {                    // S1 is a single point
			if (inSegment(p1, p3, p4) == 0)  // but is not in S2
				return 0;
			I0 = p1;
			return 1;
		}
		if (dv==0) {                    // S2 a single point
			if (inSegment(p3, p1,p2) == 0)  // but is not in S1
				return 0;
			I0 = p3;
			return 1;
		}
		// they are collinear segments - get overlap (or not)
		//float t0, t1;                   // endpoints of S1 in eqn for S2
		Vector2f w2 = p2 - p3;
		if (v.x != 0) {
			t0 = w.x / v.x;
			t1 = w2.x / v.x;
		}
		else {
			t0 = w.y / v.y;
			t1 = w2.y / v.y;
		}
		if (t0 > t1) {                  // must have t0 smaller than t1
			float t=t0; t0=t1; t1=t;    // swap if not
		}
		if (t0 > 1 || t1 < 0) {
			return 0;     // NO overlap
		}
		t0 = t0<0? 0 : t0;              // clip to min 0
		t1 = t1>1? 1 : t1;              // clip to max 1
		if (t0 == t1) {                 // intersect is a point
			I0 = p3 + v*t0;
			return 1;
		}

		// they overlap in a valid subsegment
		I0 = p3 + v*t0;
		I1 = p3 + v*t1;
		return 2;
	}

	// the segments are skew and may intersect in a point
	// get the intersect parameter for S1
	float     sI = perp(v,w) / D;
	if (sI < 0 || sI > 1)               // no intersect with S1
		return 0;

	// get the intersect parameter for S2
	float     tI = perp(u,w) / D;
	if (tI < 0 || tI > 1)               // no intersect with S2
		return 0;

	I0 = p1 + u * sI;               // compute S1 intersect point
	return 1;
}


// calculates intersection and checks for parallel lines.  
// also checks that the intersection point is actually on  
// the line segment p1-p2
bool CuttingPlane::IntersectXY(const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, InFillHit &hit)
{

	if(abs(p1.x-p3.x) < 0.01 && abs(p1.y - p3.y) < 0.01)
	{
		hit.p = p1;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p2.x-p3.x) < 0.01 && abs(p2.y - p3.y) < 0.01)
	{
		hit.p = p2;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p1.x-p4.x) < 0.01 && abs(p1.y - p4.y) < 0.01)
	{
		hit.p = p1;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p2.x-p4.x) < 0.01 && abs(p2.y - p4.y) < 0.01)
	{
		hit.p = p2;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}

	InFillHit hit2;
	float t0,t1;
	if(intersect2D_Segments(p1,p2,p3,p4,hit.p, hit2.p, t0,t1) == 1)
	{
	hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
	return true;
	}
	return false;
/*

  float xD1,yD1,xD2,yD2,xD3,yD3;  
  float dot,deg,len1,len2;  
  float segmentLen1,segmentLen2;  
  float ua,ub,div;  
  
  // calculate differences  
  xD1=p2.x-p1.x;  
  xD2=p4.x-p3.x;  
  yD1=p2.y-p1.y;  
  yD2=p4.y-p3.y;  
  xD3=p1.x-p3.x;  
  yD3=p1.y-p3.y;    
  
  // calculate the lengths of the two lines  
  len1=sqrt(xD1*xD1+yD1*yD1);  
  len2=sqrt(xD2*xD2+yD2*yD2);  
  
  // calculate angle between the two lines.  
  dot=(xD1*xD2+yD1*yD2); // dot product  
  deg=dot/(len1*len2);  
  
  // if abs(angle)==1 then the lines are parallell,  
  // so no intersection is possible  
  if(abs(deg)==1)
	  return false;
  
  // find intersection Pt between two lines  
  hit.p=Vector2f (0,0);
  div=yD2*xD1-xD2*yD1;  
  ua=(xD2*yD3-yD2*xD3)/div;  
  ub=(xD1*yD3-yD1*xD3)/div;  
  hit.p.x=p1.x+ua*xD1;  
  hit.p.y=p1.y+ua*yD1;  
  
  // calculate the combined length of the two segments  
  // between Pt-p1 and Pt-p2  
  xD1=hit.p.x-p1.x;  
  xD2=hit.p.x-p2.x;  
  yD1=hit.p.y-p1.y;  
  yD2=hit.p.y-p2.y;  
  segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);  
  
  // calculate the combined length of the two segments  
  // between Pt-p3 and Pt-p4  
  xD1=hit.p.x-p3.x;  
  xD2=hit.p.x-p4.x;  
  yD1=hit.p.y-p3.y;  
  yD2=hit.p.y-p4.y;  
  segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);  
  
  // if the lengths of both sets of segments are the same as  
  // the lenghts of the two lines the point is actually  
  // on the line segment.  
  
  // if the point isn't on the line, return null  
  if(abs(len1-segmentLen1)>0.00 || abs(len2-segmentLen2)>0.00)  
    return false;

  hit.d = segmentLen1-segmentLen2;
  return true;*/
}
/*
int PntOnLine(Vector2f p1, Vector2f p2, Vector2f t)
{
/*
 * given a line through P:(px,py) Q:(qx,qy) and T:(tx,ty)
 * return 0 if T is not on the line through      <--P--Q-->
 *        1 if T is on the open ray ending at P: <--P
 *        2 if T is on the closed interior along:   P--Q
 *        3 if T is on the open ray beginning at Q:    Q-->
 *
 * Example: consider the line P = (3,2), Q = (17,7). A plot
 * of the test points T(x,y) (with 0 mapped onto '.') yields:
 *
 *     8| . . . . . . . . . . . . . . . . . 3 3
 *  Y  7| . . . . . . . . . . . . . . 2 2 Q 3 3    Q = 3
 *     6| . . . . . . . . . . . 2 2 2 2 2 . . .
 *  a  5| . . . . . . . . 2 2 2 2 2 2 . . . . .
 *  x  4| . . . . . 2 2 2 2 2 2 . . . . . . . .
 *  i  3| . . . 2 2 2 2 2 . . . . . . . . . . .
 *  s  2| 1 1 P 2 2 . . . . . . . . . . . . . .    P = 1
 *     1| 1 1 . . . . . . . . . . . . . . . . .
 *      +--------------------------------------
 *        1 2 3 4 5 X-axis 10        15      19
 *
 * Point-Line distance is normalized with the Infinity Norm
 * avoiding square-root code and tightening the test vs the
 * Manhattan Norm. All math is done on the field of integers.
 * The latter replaces the initial ">= MAX(...)" test with
 * "> (ABS(qx-px) + ABS(qy-py))" loosening both inequality
 * and norm, yielding a broader target line for selection.
 * The tightest test is employed here for best discrimination
 * in merging collinear (to grid coordinates) vertex chains
 * into a larger, spanning vectors within the Lemming editor.
 

    if ( abs((p2.y-p1.y)*(t.x-p1.x)-(t.y-p1.y)*(p2.x-p1.x)) >= (MAX(abs(p2.x-p1.x), abs(p2.y-p1.y)))) return(0);
    if (((p2.x<=p1.x)&&(p1.x<=t.x)) || ((p2.y<=p1.y)&&(p1.y<=t.y))) return(1);
    if (((t.x<=p1.x)&&(p1.x<=p2.x)) || ((t.y<=p1.y)&&(p1.y<=p2.y))) return(1);
    if (((p1.x<=p2.x)&&(p2.x<=t.x)) || ((p1.y<=p2.y)&&(p2.y<=t.y))) return(3);
    if (((t.x<=p2.x)&&(p2.x<=p1.x)) || ((t.y<=p2.y)&&(p2.y<=p1.y))) return(3);
    return(2);
}
*/
float dist ( float x1, float y1, float x2, float y2)
{
return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

int PntOnLine(Vector2f p1, Vector2f p2, Vector2f t, float &where)
{

	float A = t.x - p1.x;
	float B = t.y - p1.y;
	float C = p2.x - p1.x;
	float D = p2.y - p1.y;

	where = abs(A * D - C * B) / sqrt(C * C + D * D);

	if(where > 0.01)
		return 0;

	float dot = A * C + B * D;
	float len_sq = C * C + D * D;
	where = dot / len_sq;

/*	float xx,yy;
	xx = p1.x + where * C;
	yy = p1.y + where * D;

	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(1,0,1);
	glVertex2f(xx, yy);
	glEnd();
*/
	if(where <= 0.0f)	// before p1
		{
//		where = param;
		return 1;
/*		xx = p1.x;
		yy = p1.y;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 1;*/
		}
	else if(where >= 1.0f) // after p2
		{
//		where = param;
		return 3;
/*		xx = p2.x;
		yy = p2.y;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 3;*/
		}
	else				// between p1 and p2
		{
//		where = param;
		return 2;			// fast exit, don't need where for this case
/*		xx = p1.x + param * C;
		yy = p1.y + param * D;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 2;*/
		}
}

class OverlapLine{
public:
	OverlapLine(Vector2f start, Vector2f end){s=start;e=end;};
	bool overlaps(Vector2f p1, Vector2f p2)
	{
	int res[2];
	float t1,t2;
	
	if(p1 == s || p2==s)
		return 1;
	if(p1 == e || p2==e)
		return 3;
	
	res[0] = PntOnLine(s,e,p1, t1);	// Is p1 on my line?
	if(res[0] == 0)
		return false;
	res[1] = PntOnLine(s,e,p2, t2);	// Is p2 on my line?
	if(res[1] == 0)
		return false;

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	glVertex2f(s.x, s.y);
	glColor3f(0,1,0);
	glVertex2f(e.x, e.y);
	glEnd();


	if(res[0] != res[1])	// expanding both ends
		{
		Vector2f i1 = s+(e-s)*t1;
		Vector2f i2 = s+(e-s)*t2;

		if(t1 < 0 && t1 < t2)	// Move p1
			s = p1;
		else if(t2 < 0)	// Move p1
			s = p2;
		if(t1 > 1 && t1 > t2)
			e = p1;
		else if(t2 > 1)
//			e = p2;

			glPointSize(5);
			glBegin(GL_POINTS);
			glColor3f(1,0,0);
			glVertex2f(s.x, s.y);
			glColor3f(0,1,0);
			glVertex2f(e.x, e.y);
			glEnd();
		
			return true;
		}

			glPointSize(1);
			glBegin(GL_POINTS);
			glColor3f(0.5,0.5,0.5);
			glVertex2f(s.x, s.y);
			glColor3f(0.5,0.5,0.5);
			glVertex2f(e.x, e.y);
			glEnd();
	return false;
	}
	Vector2f s,e;
};


void CuttingPlane::LinkSegments(float z)
{
	// pick a point.
	
	//Follow it round
	
	float precission = 0.0001;
	
	for(int i=0;i<lines.size();i++)
		{
		for(int j=i;j<lines.size();j++)
			{
			if( (vertices[lines[i].start] - vertices[lines[j].start]).length() < precission)
				{
				lines[j].start = lines[i].start;	// vertices[lines[i].start] may now be unused
				}
			if( (vertices[lines[i].start] - vertices[lines[j].end]).length() < precission)
				{
				lines[j].end = lines[i].start;	// vertices[lines[i].start] may now be unused
				}

			if( (vertices[lines[i].end] - vertices[lines[j].start]).length() < precission)
				{
				lines[j].start = lines[i].end;	// vertices[lines[i].start] may now be unused
				}
			if( (vertices[lines[i].end] - vertices[lines[j].end]).length() < precission)
				{
				lines[j].end = lines[i].end;	// vertices[lines[i].start] may now be unused
				}
			}
		}
		
	// delete unused points
	vector<int> usedlist;
	usedlist.resize(vertices.size());
	for(int i=0;i<usedlist.size();i++)
		usedlist[i] = 0;

	for(int i=0;i<lines.size();i++)
		{
		usedlist[lines[i].start]++;
		usedlist[lines[i].end]++;
		}

	// check that all vertices are used 0 (because of prevous loop) or 2 line segments. - or Bail.
	for(int i=0;i<lines.size();i++)
		{
			if(usedlist[lines[i].start] != 2 && usedlist[lines[i].start] != 0)
				return;
			if(usedlist[lines[i].end] != 2 && usedlist[lines[i].end] != 0)
				return;
		}

	// Make new vertex array
	std::vector<Vector2f> newVertices;
	std::vector<int> oldVertexNumbers;
	std::vector<int> oldVertexNumbersToNew;

	oldVertexNumbersToNew.resize(vertices.size());

	for(int i=0;i<vertices.size();i++)
	{
	if(usedlist[i] != 0)
		{
		newVertices.push_back(vertices[i]);
		oldVertexNumbersToNew[i] = oldVertexNumbers.size();
		oldVertexNumbers.push_back(i);
		}
	}

	// adjust lines to use new vertices
	for(int i=0;i<lines.size();i++)
		{
		lines[i].start = oldVertexNumbersToNew[lines[i].start];
		lines[i].end = oldVertexNumbersToNew[lines[i].end];
		}


	vertices = newVertices;

	// make reverse lines/vertices lookup table
	//vertex[i] is used by (line)5

	vector<Vector2i> vertexUseList;
	vertexUseList.resize(vertices.size());
	for(int i=0;i<vertexUseList.size();i++)
		vertexUseList[i][0] = vertexUseList[i][1] = -1;

	int c=0;
	for(int i=0;i<lines.size();i++)
	{
		int vertexNr = lines[i].start;
		if(vertexUseList[vertexNr][0] == -1)
			vertexUseList[vertexNr][0] = i;
		else
			vertexUseList[vertexNr][1] = i;

		vertexNr = lines[i].end;
		if(vertexUseList[vertexNr][0] == -1)
			vertexUseList[vertexNr][0] = i;
		else
			vertexUseList[vertexNr][1] = i;

	}

	// While (unised line segments)
		{
		// Build polygons
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f,1.0f,1.0f);
		int startLine = cvui->ExamineSlider->value()*(float)(lines.size()-1);
		int startPoint = lines[startLine].start;
		int end = lines[startLine].end;
		glVertex3f(vertices[startPoint].x, vertices[startPoint].y, z);
		glVertex3f(vertices[end].x, vertices[end].y, z);
		
		Poly poly;
		poly.points.push_back(startPoint);
		poly.points.push_back(end);

		vertexUseList[startPoint][0] = -1;// used
		vertexUseList[end][0] = -1;// used

		while(end != startPoint)	// While not closed
		{
			Vector2i linesThatUseThisVertex = vertexUseList[end];

			// Is the vertex we are looking at from line 1 ?
			if(linesThatUseThisVertex[0] != -1 && lines[linesThatUseThisVertex[0]].start == end)	// 22 is used by 4 and 6 - we are looking for 4
			{
				end = lines[linesThatUseThisVertex[0]].end;
				linesThatUseThisVertex[0]= -1;		// Been used
			}
			else if(linesThatUseThisVertex[0] != -1 && lines[linesThatUseThisVertex[0]].end == end)	// 22 is used by 4 and 6 - we are looking for 4
			{
				end = lines[linesThatUseThisVertex[0]].start;
				linesThatUseThisVertex[0] = -1;
			}

			else if(linesThatUseThisVertex[1] != -1 && lines[linesThatUseThisVertex[1]].start == end)	// 22 is used by 4 and 6 - we are looking for 4
			{
				end = lines[linesThatUseThisVertex[1]].end;
				linesThatUseThisVertex[1] = -1;
			}
			else if(linesThatUseThisVertex[1] != -1 && lines[linesThatUseThisVertex[1]].end == end)	// 22 is used by 4 and 6 - we are looking for 4
			{
				end = lines[linesThatUseThisVertex[1]].start;
				linesThatUseThisVertex[1] = -1;
			}

/*			// Find a lines that starts with my end point
			for(int i=0;i<lines.size();i++)
			{
				if(i==startLine)
					continue;				// avoid infinite loop
				if(lines[i].start == end)	// store point
				{
					startLine = i;
					end = lines[startLine].end;
					break;				// done
				}
				else if(lines[i].end == end)	// store point
				{
					startLine = i;
					end = lines[startLine].start;
					break;				// done
				}
				if(end==startPoint)
					break;				// done
			}*/
			glVertex3f(vertices[end].x, vertices[end].y, z);
			glColor3f(0.0f,1.0f,0.0f);
			poly.points.push_back(end);
		}
		glVertex3f(vertices[startPoint].x, vertices[startPoint].y, z);
		glEnd();
		polygons.push_back(poly);
		}

}


void CuttingPlane::Shrink(float distance)
{
	CuttingPlane result;
	if(cvui->DisplayCuttingPlaneButton->value())
	{
		glColor4f(1,1,1,1);
		glBegin(GL_LINES);
		for(UINT i=0;i<lines.size();i++)
		{
			//float angle = atan2(v2.y,v2.x) - atan2(v1.y,v1.x);
//			glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
//			glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
		}
		glEnd();
	}
}

void CuttingPlane::Draw(float z)
{
	if(cvui->DisplayCuttingPlaneButton->value())
		{
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
		for(UINT i=0;i<lines.size();i++)
			{
				if(cvui->DisplayDebugButton->value() && (int)(cvui->ExamineSlider->value()*((float)lines.size()-1)) == i)
				{
				glEnd();
				glColor4f(1,1,0,1);
				glLineWidth(3);
				glBegin(GL_LINES);
				glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
				glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
				glEnd();
				glColor4f(0.5f,0.5f,0.5f,1);
				glLineWidth(1);
				glBegin(GL_LINES);
				glColor4f(1,0,0,1);
				}

			glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
			glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
			}
		glEnd();


	// Endpoints
		glColor4f(1,1,0,1);
		glPointSize(2);
		glBegin(GL_POINTS);
		for(UINT i=0;i<lines.size();i++)
		{
				glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
				glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
		}
		glEnd();
	}
}

void STL::OptimizeRotation()
{
	// Find the axis that has the largest surface
	// Rotate to point this face towards -Z

	// if dist center <|> 0.1 && Normal points towards, add area

	Vector3f AXIS_VECTORS[3];
	AXIS_VECTORS[0] = Vector3f(1,0,0);
	AXIS_VECTORS[1] = Vector3f(0,1,0);
	AXIS_VECTORS[2] = Vector3f(0,0,1);

	float area[6];
	for(UINT i=0;i<6;i++)
		area[i] = 0.0f;

	for(UINT i=0;i<triangles.size();i++)
		{
		triangles[i].axis = NOT_ALIGNED;				
		for(UINT triangleAxis=0;triangleAxis<3;triangleAxis++)
			{
			if (  triangles[i].N.cross(AXIS_VECTORS[triangleAxis]).length() < 0.1)
				{
				int positive=0;
				if(triangles[i].N[triangleAxis] > 0)// positive
					positive=1;
				AXIS axisNr = (AXIS)(triangleAxis*2+positive);
				triangles[i].axis = axisNr;
				if( ! (abs(Min[triangleAxis]-triangles[i].A[triangleAxis]) < 1.1 || abs(Max[triangleAxis]-triangles[i].A[triangleAxis]) < 1.1) )	// not close to boundingbox edges?
					{
					triangles[i].axis = NOT_ALIGNED;	// Not close to bounding box
					break;
					}
				area[axisNr] += triangles[i].area();
				break;
				}
			}
		}


	AXIS down = NOT_ALIGNED;
	float LargestArea = 0;
	for(UINT i=0;i<6;i++)
	{
	if(area[i] > LargestArea)
		{
		LargestArea = area[i];
		down = (AXIS)i;
		}
	}

	switch(down)
	{
	case NEGX: RotateObject(Vector3f(0,-1,0), M_PI/2.0f); break;
	case POSX: RotateObject(Vector3f(0,1,0), M_PI/2.0f); break;
	case NEGY: RotateObject(Vector3f(1,0,0), M_PI/2.0f); break;
	case POSY: RotateObject(Vector3f(-1,0,0), M_PI/2.0f); break;
	case POSZ: RotateObject(Vector3f(1,0,0), M_PI); break;
	}

}

void STL::RotateObject(Vector3f axis, float angle)
{
	Vector3f min,max;

	min.x = min.y = min.z = 99999999.0f;
	max.x = max.y = max.z = -99999999.0f;

	for(UINT i=0; i<triangles.size() ; i++)
	{
	triangles[i].N = triangles[i].N.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].A = triangles[i].A.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].B = triangles[i].B.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].C = triangles[i].C.rotate(angle, axis.x, axis.y, axis.z);
	min.x = MIN(min.x, triangles[i].A.x);
	min.y = MIN(min.y, triangles[i].A.y);
	min.z = MIN(min.z, triangles[i].A.z);
	max.x = MAX(max.x, triangles[i].A.x);
	max.y = MAX(max.y, triangles[i].A.y);
	max.z = MAX(max.z, triangles[i].A.z);
	min.x = MIN(min.x, triangles[i].B.x);
	min.y = MIN(min.y, triangles[i].B.y);
	min.z = MIN(min.z, triangles[i].B.z);
	max.x = MAX(max.x, triangles[i].B.x);
	max.y = MAX(max.y, triangles[i].B.y);
	max.z = MAX(max.z, triangles[i].B.z);
	min.x = MIN(min.x, triangles[i].C.x);
	min.y = MIN(min.y, triangles[i].C.y);
	min.z = MIN(min.z, triangles[i].C.z);
	max.x = MAX(max.x, triangles[i].C.x);
	max.y = MAX(max.y, triangles[i].C.y);
	max.z = MAX(max.z, triangles[i].C.z);
	}
	Min = min;
	Max = max;
}

float Triangle::area()
{
	return ( ((C-A).cross(B-A)).length() );
}