#pragma once

#include "../Libraries/FLU_2.14/FLU/Flu_Tree_Browser.h"

/*
<!--
the values in this file are made-up and are only used to illustrate structure.  
-->
−
<reprap-fab-at-home-build version="0.1">
<!-- a build consists of a variety of objects -->
−
<object name="Corner Bracket">
<!-- each object can contain multiple files. -->
−
<files>
<!-- each file has a location, filetype, and material -->
−
<file location="1.stl" filetype="application/sla" material="HDPE">
−
<!--
you can specify a transform on the file level.  it is applied to this file only. 
-->
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="2.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="2.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="2.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</file>
−
<file location="2.stl" material="Silicone">
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="2.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="2.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="2.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</file>
</files>
−
<!--
you can also supply a transform on the object level that is applied to every file in the object, after they are all loaded. 
-->
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="2.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="2.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="2.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</object>
−
<object name="X Carriage">
−
<files>
−
<file location="http://www.example.com/xcarriage.stl" material="HDPE">
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="2.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="2.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="2.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</file>
−
<file location="3.stl" material="Polyfilla">
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="2.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="2.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="2.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</file>
</files>
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="0.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="0.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="0.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</object>
−
<!--
you can also supply a transform on the build level that is applied to each and every object, after they are all loaded. 
-->
−
<transform3D>
<row m00="1.0" m01="0.0" m02="0.0" m03="0.0"/>
<row m10="0.0" m11="1.0" m12="0.0" m13="0.0"/>
<row m20="0.0" m21="0.0" m22="1.0" m23="0.0"/>
<row m30="0.0" m31="0.0" m32="0.0" m33="1.0"/>
</transform3D>
</reprap-fab-at-home-build>
*/
#pragma once
#include "xml/XML.H"
#include "stl.h"

class RFO_Transform3D
{
public:
	RFO_Transform3D(){identity();}
	void identity(){transform=Matrix4f::IDENTITY;}
	void Read(XMLElement* ch);
	void SaveXML(XMLElement* x);
	Matrix4f transform;
	Flu_Tree_Browser::Node* node;
};

class RFO_File
{
public:
	RFO_File(){}
	void Read(XMLElement* e);
	void Draw();
	RFO_Transform3D transform3D;
	string location;
	string filetype;
	string material;
	STL stl;
	Flu_Tree_Browser::Node* node;
};

class RFO_Object
{
public:
	RFO_Object(){name = "Unnamed object";};
	void Read(XMLElement* e);
	void ReadFiles(XMLElement* e);
	string name;
	RFO_Transform3D transform3D;
	vector<RFO_File> files;
	Flu_Tree_Browser::Node* node;
};


class RFO
{
public:
	RFO(){version=0.1f;}
	void Draw(const ProcessController &PC, float opasity = 1.0f);
	void Load(string path, ProcessController &PC);
	void ReadVersion0x1(XMLElement* e);
	void BuildBrowser(ProcessController &PC);
	void clear(ProcessController &PC);
	void DeleteSelected();
	bool Open(string &filename, ProcessController &PC);
	bool Save(string &filename, ProcessController &PC);
	Matrix4f &SelectedNodeMatrix(Flu_Tree_Browser::Node *node);
	vector<RFO_Object> Objects;
	RFO_Transform3D transform3D;
	float version;
	string m_filename;
	Flu_Tree_Browser::Node* node;
};
