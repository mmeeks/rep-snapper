/* -------------------------------------------------------- *
*
* StdAfx.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */

// stdafx.cpp : source file that includes just the standard includes
// RepSnapper.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

bool InFillHitCompareFunc(const InFillHit& d1, const InFillHit& d2)
{
	return d1.d < d2.d;
}
