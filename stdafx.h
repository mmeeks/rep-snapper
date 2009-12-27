/* -------------------------------------------------------- *
*
* StdAfx.h
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <windows.h>											// Header File For Windows
#include <stdio.h>
#include <tchar.h>
#include <fl/Fl.H>
#include <vmmlib/vmmlib.h>
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glaux.h>											// Header File For The GLaux Library
#include "math.h"                                               // Needed for sqrtf
#include "ArcBall.h"
#include "gcode.h"
#include "ModelViewController.h"
#include "Printer.h"
#include "ProcessController.h"
#include "search.h"
#include "stl.h"
#include "UI.h"
#include "xml/XML.h"

void MakeAcceleratedGCodeLine(Vector3f start, Vector3f end, UINT accelerationSteps, float distanceBetweenSpeedSteps, float extrusionFactor, GCode &code, float z, float minSpeedXY, float maxSpeedXY, float minSpeedZ, float maxSpeedZ, bool UseIncrementalECode, float &E);
bool IntersectXY(const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, InFillHit &hit);	// Utilityfunction for CalcInFill
bool InFillHitCompareFunc(const InFillHit& d1, const InFillHit& d2);

#include "ModelViewController.h"

// TODO: reference additional headers your program requires here
