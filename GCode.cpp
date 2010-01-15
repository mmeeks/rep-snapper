/* -------------------------------------------------------- *
*
* GCode.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */

#include "stdafx.h"

#include "gcode.h"
#include "FL/FL.h"

#include <iostream>
#include <sstream>

#include "UI.h"

using namespace std;


GCode::GCode()
{
	Min.x = Min.y = Min.z = 99999999.0f;
	Max.x = Max.y = Max.z = -99999999.0f;
	Center.x = Center.y = Center.z = 0.0f;
}

void GCode::Read(string filename)
{
	commands.clear();

	ifstream file;
	file.open(filename.c_str());		//open a file

	if(!file.good())
	{
//		MessageBrowser->add(str(boost::format("Error opening file %s") % Filename).c_str());
		return;
	}

	UINT LineNr = 0;

	string s;

	Vector3f globalPos(0,0,0);
	Min.x = Min.y = Min.z = 99999999.0f;
	Max.x = Max.y = Max.z = -99999999.0f;

	while(getline(file,s))
	{
		istringstream line(s);
		LineNr++;
		string buffer;
//		char charBuffer[1000];
		line >> buffer;	// read something

		if(buffer.find( ";", 0) != string::npos)	// COMMENT
			continue;

		int a=0;

		Command command;

		if( buffer.find( "G21", 0) != string::npos )	//Coordinated Motion
		{
			command.Code = MILLIMETERSASUNITS;
			commands.push_back(command);
		}


		if( buffer.find( "G1", 0) != string::npos )	//string::npos means not defined
		{
			command.Code = COORDINATEDMOTION;
			command.where = globalPos;
			while(line >> buffer)	// read next keyword
			{
				if( buffer.find( "X", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.x = ToFloat(number);
				}
				if( buffer.find( "Y", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.y = ToFloat(number);
				}
				if( buffer.find( "Z", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.z = ToFloat(number);
				}
				if( buffer.find( "E", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.e = ToFloat(number);
				}
				if( buffer.find( "F", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.f = ToFloat(number);
				}
			}
			if(command.where.x < -100)
				continue;
			if(command.where.y < -100)
				continue;
			globalPos = command.where;

			if(command.where.x < Min.x)
				Min.x = command.where.x;
			if(command.where.y < Min.y)
				Min.y = command.where.y;
			if(command.where.z < Min.z)
				Min.z = command.where.z;
			if(command.where.x > Max.x)
				Max.x = command.where.x;
			if(command.where.y > Max.y)
				Max.y = command.where.y;
			if(command.where.z > Max.z)
				Max.z = command.where.z;
			commands.push_back(command);
		}
		else if( buffer.find( "G0", 0) != string::npos )	//Rapid Motion
		{
			command.Code = RAPIDMOTION;
			command.where = globalPos;
			while(line >> buffer)	// read next keyword
			{
				if( buffer.find( "X", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.x = ToFloat(number);
				}
				if( buffer.find( "Y", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.y = ToFloat(number);
				}
				if( buffer.find( "Z", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.z = ToFloat(number);
				}
				if( buffer.find( "E", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.e = ToFloat(number);
				}
				if( buffer.find( "F", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.f = ToFloat(number);
				}
			}
			if(command.where.x < -100)
				continue;
			if(command.where.y < -100)
				continue;
			globalPos = command.where;

			if(command.where.x < Min.x)
				Min.x = command.where.x;
			if(command.where.y < Min.y)
				Min.y = command.where.y;
			if(command.where.z < Min.z)
				Min.z = command.where.z;
			if(command.where.x > Max.x)
				Max.x = command.where.x;
			if(command.where.y > Max.y)
				Max.y = command.where.y;
			if(command.where.z > Max.z)
				Max.z = command.where.z;
			commands.push_back(command);
		}
	}



	Center.x = (Max.x + Min.x )/2;
	Center.y = (Max.y + Min.y )/2;
	Center.z = (Max.z + Min.z )/2;

}

void GCode::draw(const ProcessController &PC)
{
	/*--------------- Drawing -----------------*/

	Vector3f thisPos(0,0,0);
	Vector3f LastColor = Vector3f(0.0f,0.0f,0.0f);
	Vector3f Color = Vector3f(0.0f,0.0f,0.0f);
	float	Distance = 0.0f;
	Vector3f pos(0,0,0);
	UINT start = (UINT)(PC.GCodeDrawStart*(float)(commands.size()));
	UINT end = (UINT)(PC.GCodeDrawEnd*(float)(commands.size()));

	float Er,Eg,Eb;
	HSVtoRGB(PC.GCodeExtrudeHue, PC.GCodeExtrudeSat, PC.GCodeExtrudeVal, Er,Eg,Eb);
	float Mr,Mg,Mb;
	HSVtoRGB(PC.GCodeMoveHue, PC.GCodeMoveSat, PC.GCodeMoveVal, Mr,Mg,Mb);

	float LastE=0.0f;

	for(UINT i=start;i<commands.size() && i < end ;i++)
	{
		switch(commands[i].Code)
		{
		case ZMOVE:
		case COORDINATEDMOTION:
			if(commands[i].e == LastE)
				{
				glLineWidth(3);
				float speed = commands[i].f;
				float luma = speed/PC.MaxPrintSpeedXY*0.5f;
				if(PC.LuminanceShowsSpeed == false)
					luma = 1.0f;
				Color=Vector3f(luma*Mr,luma*Mg,luma*Mb);
				}
			else
				{
				glLineWidth(3);
				float speed = commands[i].f;
				float luma = speed/PC.MaxPrintSpeedXY;
				if(PC.LuminanceShowsSpeed == false)
					luma = 1.0f;
				Color=Vector3f(luma*Er,luma*Eg,luma*Eb);
				}
			if(PC.LuminanceShowsSpeed == false)
				LastColor = Color;
			Distance += (commands[i].where-pos).length();
			glBegin(GL_LINES);
			glColor3fv(&LastColor[0]);
			glVertex3fv((GLfloat*)&pos);
			glColor3fv(&Color[0]);
			glVertex3fv((GLfloat*)&commands[i].where);
			glEnd();
			LastColor = Color;
			LastE=commands[i].e;
			break;
		case RAPIDMOTION:
			glLineWidth(1);
			glBegin(GL_LINES);
			glColor3f(0.75f,0.0f,0.0f);
			Distance += (commands[i].where-pos).length();
			glVertex3fv((GLfloat*)&pos);
			glVertex3fv((GLfloat*)&commands[i].where);
			glEnd();
			break;
		}
		pos = commands[i].where;
	}

	glLineWidth(1);
	std::stringstream oss;

	oss << "Length: "  << Distance/1000.0f << " - " << Distance/200000.0f << " Hour.";
//	std::cout << oss.str();

//	gui->GCodeLengthText->value(oss.str().c_str());	// todo: Fix


	// Draw bbox
	glColor3f(1,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.x, Min.y, Min.z);
	glVertex3f(Max.x, Min.y, Min.z);
	glVertex3f(Max.x, Max.y, Min.z);
	glVertex3f(Min.x, Max.y, Min.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.x, Min.y, Max.z);
	glVertex3f(Max.x, Min.y, Max.z);
	glVertex3f(Max.x, Max.y, Max.z);
	glVertex3f(Min.x, Max.y, Max.z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(Min.x, Min.y, Min.z);
	glVertex3f(Min.x, Min.y, Max.z);
	glVertex3f(Min.x, Max.y, Min.z);
	glVertex3f(Min.x, Max.y, Max.z);
	glVertex3f(Max.x, Max.y, Min.z);
	glVertex3f(Max.x, Max.y, Max.z);
	glVertex3f(Max.x, Min.y, Min.z);
	glVertex3f(Max.x, Min.y, Max.z);
	glEnd();

}

void GCode::MakeText(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, bool UseIncrementalEcode)
{
	Vector3f pos(0,0,0);
	
	float Distance = 0;
	std::stringstream oss;

	Vector3f LastPos(-10,-10,-10);
	float lastE = -10;

	GcodeTxt += GcodeStart + "\n";

	for(UINT i=0;i<commands.size() ;i++)
	{
		oss.str( "" );
		switch(commands[i].Code)
		{
		case SELECTEXTRUDER:
			oss  << "T0\n";
			GcodeTxt += oss.str();
			break;
		case SETSPEED:
			commands[i].where.z = LastPos.z;
			commands[i].e = lastE;
		case ZMOVE:
			commands[i].where.x = LastPos.x;
			commands[i].where.y = LastPos.y;
		case COORDINATEDMOTION:
			oss  << "G1 ";
			if(commands[i].where.x != LastPos.x)
				oss << "X" << commands[i].where.x << " ";
			if(commands[i].where.y != LastPos.y)
				oss << "Y" << commands[i].where.y << " ";
			if(commands[i].where.z != LastPos.z)
				oss << "Z" << commands[i].where.z << " ";
			if(UseIncrementalEcode)	// in incremental mode, the same is nothing
				{
				if(commands[i].e != lastE)
					oss << "E" << commands[i].e << " ";
				}
			else
				{
				if(commands[i].e != 0.0f)
					oss << "E" << commands[i].e << " ";
				}
			oss << " F" << commands[i].f;
			if(commands[i].comment.length() != 0)
				oss << "	;" << commands[i].comment << "\n";
			else
				oss <<  "\n";
			GcodeTxt += oss.str();
			LastPos = commands[i].where;
			lastE = commands[i].e;
			if(commands[i].Code == ZMOVE)
				GcodeTxt += GcodeLayer + "\n";
			break;
		case RAPIDMOTION:
			oss  << "G0 X" << commands[i].where.x << " Y" << commands[i].where.y << " Z" << commands[i].where.z  << " E0\n";
			GcodeTxt += oss.str();
			LastPos = commands[i].where;
			lastE = commands[i].e;
			break;
		}
		pos = commands[i].where;
	}

	GcodeTxt += GcodeEnd + "\n";
}

