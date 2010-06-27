/* -------------------------------------------------------- *
*
* unittest.cpp
*
* Copyright 2010+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */



/*************************************************************************

	TODO LIST:

Logick: Add more Unittest :)


*************************************************************************/

#include "stdafx.h"
#include "ModelViewController.h"
#include "gcode.h"
#include "UI.h"

#if !defined(WIN32) || defined (UNITTEST)

GUI *gui;

#include <boost/thread.hpp>

using namespace std;

#include <Polygon2f.h>

#define BOOST_TEST_MODULE RepSnapperTest
#define BOOST_TEST_MAIN 1
#define BOOST_TEST_DYN_LINK 1
#include <boost/test/unit_test.hpp>

using namespace PolyLib;

    //// seven ways to detect and report the same error:
    //BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error

    //BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error

    //if( add( 2,2 ) != 4 )
    //  BOOST_ERROR( "Ouch..." );            // #3 continues on error

    //if( add( 2,2 ) != 4 )
    //  BOOST_FAIL( "Ouch..." );             // #4 throws on error

    //if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error

    //BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
    //                     "add(..) result: " << add( 2,2 ) );

    //BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error

BOOST_AUTO_TEST_CASE( Logick_Basic_Shrink_Test )
{
	Polygon2f p;

	p.vertices.push_back(Vector2f(10,10));
	p.vertices.push_back(Vector2f(10,110));
	p.vertices.push_back(Vector2f(110,110));
	p.vertices.push_back(Vector2f(110,10));

	list<Polygon2f*> parent;
	list<Polygon2f*> res;

	p.Shrink(1, parent, res);
	BOOST_CHECK( p.vertices.size() == res.front()->vertices.size() );
	BOOST_CHECK( res.front()->vertices.front() == Vector2f(11,11) );
	BOOST_CHECK( res.front()->vertices.back() == Vector2f(109,11) );

	delete res.front();
}

BOOST_AUTO_TEST_CASE( Logick_Advanced_Shrink_Test )
{
	Polygon2f p;

	p.vertices.push_back(Vector2f(10,10));
	p.vertices.push_back(Vector2f(10,110));
	p.vertices.push_back(Vector2f(110,110));
	p.vertices.push_back(Vector2f(120,110));
	p.vertices.push_back(Vector2f(110,109));
	p.vertices.push_back(Vector2f(110,10));

	list<Polygon2f*> parent;
	list<Polygon2f*> res;

	p.Shrink(2, parent, res);
	BOOST_CHECK( p.vertices.size()-2 == res.front()->vertices.size() );
	BOOST_CHECK( res.front()->vertices.front() == Vector2f(12,12) );
	BOOST_CHECK( res.front()->vertices.back() == Vector2f(108,12) );

	delete res.front();
}

BOOST_AUTO_TEST_CASE( Logick_Advanced_Polygon_Split_Tests )
{
	Polygon2f p;

	p.vertices.push_back(Vector2f(10,10));
	p.vertices.push_back(Vector2f(59,60));
	p.vertices.push_back(Vector2f(10,110));
	p.vertices.push_back(Vector2f(110,110));
	p.vertices.push_back(Vector2f(61,60));
	p.vertices.push_back(Vector2f(110,10));

	list<Polygon2f*> parent;
	list<Polygon2f*> res;

	p.Shrink(2, parent, res);
	BOOST_CHECK( res.size() == 2 );

	for(list<Polygon2f*>::iterator pIt = res.begin(); pIt != res.end(); pIt++)
	{
		delete *pIt;
	}
}

BOOST_AUTO_TEST_CASE( Slicing_PointHash )
{
	PointHash h;
	float x = 10.0, y = 7.0;
	float d = PointHash::float_epsilon;
}

#endif // !defined(WIN32) || defined (UNITTEST)
