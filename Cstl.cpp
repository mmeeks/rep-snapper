#include "stdafx.h"

#include "Cstl.h"
#include "gcode.h"
#include "UI.h"
#include "math.h"

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
		infile.read(reinterpret_cast < char * > (&header), sizeof(long));	// N_Triangles
		if(header == 0x696c6f73)
		{
//		ReadAsciiFile();
		return;
		}
		//soli
			
			
		infile.seekg(80, ios_base::beg);
		infile.read(reinterpret_cast < char * > (&count), sizeof(unsigned int));	// N_Triangles

		triangles.resize(count);	// pre-allocate array

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
/*
	Min.x = MIN(code->Min.T[0], Min.x);
	Min.y = MIN(code->Min.T[1], Min.y);
	Min.z = MIN(code->Min.T[2], Min.z);
	Max.x = MAX(code->Max.T[0], Max.x);
	Max.y = MAX(code->Max.T[2], Max.y);
	Max.z = MAX(code->Max.T[0], Max.z);
*/
	code->Min.T[0] = Min.x;
	code->Min.T[1] = Min.y;
	code->Min.T[2] = Min.z;
	code->Max.T[0] = Max.x;
	code->Max.T[1] = Max.y;
	code->Max.T[2] = Max.z;

	code->Center.T[0] = (code->Max.T[0] + code->Min.T[0] )/2;
	code->Center.T[1] = (code->Max.T[1] + code->Min.T[1] )/2;
	code->Center.T[2] = (code->Max.T[2] + code->Min.T[2] )/2;

	// Find zoom

	float L=0;
	if(code->Max.T[0] - code->Min.T[0] > L)	L = code->Max.T[0] - code->Min.T[0];
	if(code->Max.T[1] - code->Min.T[1] > L)	L = code->Max.T[1] - code->Min.T[1];
	if(code->Max.T[2] - code->Min.T[2] > L)	L = code->Max.T[2] - code->Min.T[2];

	code->zoom= L;

}
void STL::draw()
{

	// polygons

	if(cvui->DisplayPolygonsButton->value())
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //define blending factors
		glColor4f(0,1,0,0.5f);
		glBegin(GL_TRIANGLES);
		for(UINT i=0;i<triangles.size();i++)
		{
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

//	CalcCuttingPlane(cvui->CuttingPlaneSlider->value(), cuttingPlane);

	// Make Layers
	UINT LayerNr = 0;

	float ySize = (Max.y-Min.y);
	fprintf(stderr,"ySize %f\n",ySize);
	fprintf(stderr,"Max.y %f\n",Max.y);
	fprintf(stderr,"Min.y %f\n",Min.y);
	float y=cvui->CuttingPlaneSlider->value()*ySize+Min.y;

//	for(float y=Min.y ; y<Max.y; y+=cvui->LayerThicknessSlider->value())
		{
		vector<Vector3f> cuttingPlane;
		CalcCuttingPlane(y, cuttingPlane);

		fprintf(stderr,"cuttingPlane size : %d\n",cuttingPlane.size());

		// CuttingPlane
		if(cvui->DisplayCuttingPlaneButton->value())
			{
			glColor4f(1,1,1,1);
			glBegin(GL_LINES);
			for(UINT i=0;i<cuttingPlane.size();i+=2)
				{
				glVertex3fv((GLfloat*)&cuttingPlane[i]);
				glVertex3fv((GLfloat*)&cuttingPlane[i+1]);
				}
			glEnd();
			}

		// inFill
		vector<Vector3f> infill;
//		CalcInFill(y, cuttingPlane, infill, LayerNr);

		if(cvui->DisplayinFillButton->value())
			{
				glColor4f(1,1,0,1);
				glPointSize(5);
				glBegin(GL_LINES);
				for(UINT i=0;i<infill.size();i+=2)
				{
					if(infill.size() > i+1)
					{
					glVertex3fv((GLfloat*)&infill[i]);
					glVertex3fv((GLfloat*)&infill[i+1]);
					}
				}
				glEnd();
			}
		glPointSize(1);
		LayerNr++;
		}
}

void STL::CalcCuttingPlane(float where, vector<Vector3f> &points)
{
	// intersect lines with plane

	GCode *code = cvui->code;

	for(UINT i=0;i<triangles.size();i++)
	{
		Vector3f P1 = triangles[i].A;
		Vector3f P2 = triangles[i].B;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(P2.z-P1.z);
			points.push_back(P1+((P2-P1)*t));
		}
		P1 = triangles[i].B;
		P2 = triangles[i].C;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(P2.z-P1.z);
			points.push_back(P1+((P2-P1)*t));
		}
		P1 = triangles[i].C;
		P2 = triangles[i].A;
		if(where < P1.z != where < P2.z)
		{
			float t = (where-P1.z)/(P2.z-P1.z);
			points.push_back(P1+((P2-P1)*t));
		}
	}

}

bool InFillHitCompareFunc(const InFillHit& d1, const InFillHit& d2)
{
  return d1.d < d2.d;
}

vector<InFillHit> HitsBuffer;

void STL::CalcInFill(float where, vector<Vector3f> &CuttingPlane, vector<Vector3f> &infill, UINT layerNr)
{
	int c=0;

	float step = cvui->InfillDistanceSlider->value();

	float Z = where;//*(Max.z-Min.z)+Min.z;

	bool examine = false;

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;

	float rot = cvui->RotationSlider->value()/180.0f*M_PI;
	rot += layerNr*cvui->InfillRotationPrLayerSlider->value()/180.0f*M_PI;
	Vector3f InfillDirX(cosf(rot), sinf(rot), 0.0f);	// Length = 1
	Vector3f InfillDirY(-InfillDirX.y, InfillDirX.x, 0.0f);	// Length = 1
	Vector3f Center = (Max+Min)/2.0f;
	Center.z = where*(Max.z+Min.z);
	
	glBegin(GL_LINES);
	for(float x = -Length ; x < Length ; x+=step)
	{
		bool examineThis = false;

		HitsBuffer.clear();

		Vector3f P1 = (InfillDirX * Length)+(InfillDirY*x)+ Center + Vector3f(0,0,Z);
		Vector3f P2 = (InfillDirX * -Length)+(InfillDirY*x) + Center + Vector3f(0,0,Z);

		if(cvui->DisplayDebuginFillButton->value())
		{
		glColor3f(0,0.5f,0);
		glVertex3fv((GLfloat*)&P1);
		glVertex3fv((GLfloat*)&P2);
		}

		if(cvui->DisplayDebugButton->value())
			if(!examine && ((cvui->ExamineSlider->value()-0.5f)*2 * Length <= x))
				{
				examineThis = examine = true;
				glColor3f(1,1,1);
				glVertex3fv((GLfloat*)&P1);
				glVertex3fv((GLfloat*)&P2);
				}


		for(UINT i=0;i<CuttingPlane.size();i+=2)
		{
			Vector3f P3 = CuttingPlane[i];
			Vector3f P4 = CuttingPlane[i+1];

			glVertex3fv((GLfloat*)&P3);
			glVertex3fv((GLfloat*)&P4);

			Vector3f point;
			InFillHit hit;
			P1.z = P2.z = P3.z = P4.z = Z;
			if(IntersectXY(P1,P2,P3,P4,hit))
			{
				hit.p.z = P1.z;
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
		
		// Verify hits intregrety
		// Check if hit extists in table
restart_check:
		for(UINT i=0;i<HitsBuffer.size();i++)
			{
			if(examineThis)
				int a=0;
			bool found = false;
			fprintf(stderr, "Hitsbuffer.size: %d\n",HitsBuffer.size());
			for(UINT j=i+1;j<HitsBuffer.size();j++)
				if( abs(HitsBuffer[i].d - HitsBuffer[j].d) < 0.0001)// && abs( (HitsBuffer[i].p - HitsBuffer[j].d).length()) < 0.0001 )
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
					glEnd();
					glPointSize(10);
					glBegin(GL_POINTS);
					glVertex3fv((GLfloat *)&HitsBuffer[0].p);
					glEnd();
					glPointSize(1);
					glColor3f(1,1,1);
					glBegin(GL_LINES);
				}
			HitsBuffer.erase(HitsBuffer.begin());
		}
	}

	glEnd();

}


#define SMALL_NUM  0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product (2D)

//===================================================================

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is not inside S
int inSegment( const Vector3f &P, const Vector3f &p1, const Vector3f &p2)
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
int intersect2D_Segments( const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, const Vector3f &p4, Vector3f &I0, Vector3f &I1 )
{
	Vector3f    u = p2 - p1;
	Vector3f    v = p4 - p3;
	Vector3f    w = p1 - p3;
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
		float t0, t1;                   // endpoints of S1 in eqn for S2
		Vector3f w2 = p2 - p3;
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
bool STL::IntersectXY(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, const Vector3f &p4, InFillHit &hit)
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
	if(intersect2D_Segments(p1,p2,p3,p4,hit.p, hit2.p) == 1)
	{
	hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
	return true;
	}
	return false;


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
  hit.p=Vector3f (0,0,0);
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
  return true;
}
