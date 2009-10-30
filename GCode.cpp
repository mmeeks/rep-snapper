#include "stdafx.h"

#include "gcode.h"
#include "FL/FL.h"

using namespace std;


GCode::GCode(int x,int y,int w,int h,const char *l) : Fl_Gl_Window(x,y,w,h,l)
{
	Min.T[0] = Min.T[1] = Min.T[2] = 99999999.0f;
	Max.T[0] = Max.T[1] = Max.T[2] = -99999999.0f;
	Center.T[0] = Center.T[1] = Center.T[2] = 0.0f;
	

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth (1.0f);											// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);										// Enable Depth Testing
	glShadeModel (GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate

	ArcBall = new ArcBallT((GLfloat)w, (GLfloat)h);				                // NEW: ArcBall Instance


	Transform.M[0] = 1.0f;	Transform.M[1] = 0.0f; Transform.M[2] = 0.0f;Transform.M[3] = 0.0f,				// NEW: Final Transform
	Transform.M[4] = 0.0f;	Transform.M[5] = 1.0f; Transform.M[6] = 0.0f;Transform.M[7] = 0.0f,				// NEW: Final Transform
	Transform.M[8] = 0.0f;	Transform.M[9] = 0.0f; Transform.M[10] = 1.0f;Transform.M[11] = 0.0f,				// NEW: Final Transform
	Transform.M[12] = 0.0f;	Transform.M[13] = 0.0f; Transform.M[14] = 0.0f;Transform.M[15] = 1.0f,				// NEW: Final Transform

	LastRot.M[0]=1.0f;LastRot.M[1]=0.0f;LastRot.M[2]=0.0f;					// NEW: Last Rotation
	LastRot.M[3]=0.0f;LastRot.M[4]=1.0f;LastRot.M[5]=0.0f;					// NEW: Last Rotation
	LastRot.M[6]=0.0f;LastRot.M[7]=0.0f;LastRot.M[8]=1.0f;					// NEW: Last Rotation
	ThisRot.M[0]=1.0f;ThisRot.M[1]=0.0f;ThisRot.M[2]=0.0f;					// NEW: Last Rotation
	ThisRot.M[3]=0.0f;ThisRot.M[4]=1.0f;ThisRot.M[5]=0.0f;					// NEW: Last Rotation
	ThisRot.M[6]=0.0f;ThisRot.M[7]=0.0f;ThisRot.M[8]=1.0f;					// NEW: Last Rotation


	quadratic=gluNewQuadric();										// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);						// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);							// Create Texture Coords
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

	Vector3fT globalPos;
	Min.T[0] = Min.T[1] = Min.T[2] = 99999999.0f;
	Max.T[0] = Max.T[1] = Max.T[2] = -99999999.0f;

	globalPos.T[0] = globalPos.T[1] = globalPos.T[2] = 0;

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

		if( buffer.find( "G21", 0) != string::npos )	//string::npos means not defined
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
					command.where.s.X = ToFloat(number);
				}
				if( buffer.find( "Y", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.s.Y = ToFloat(number);
				}
				if( buffer.find( "Z", 0) != string::npos )	//string::npos means not defined
				{
					string number = buffer.substr(1,buffer.length()-1);				// 16 characters
					command.where.s.Z = ToFloat(number);
				}
			}
			if(command.where.T[0] < -100)
				continue;
			if(command.where.T[1] < -100)
				continue;
			globalPos = command.where;

			if(command.where.T[0] < Min.T[0])
				Min.T[0] = command.where.T[0];
			if(command.where.T[1] < Min.T[1])
				Min.T[1] = command.where.T[1];
			if(command.where.T[2] < Min.T[2])
				Min.T[2] = command.where.T[2];
			if(command.where.T[0] > Max.T[0])
				Max.T[0] = command.where.T[0];
			if(command.where.T[1] > Max.T[1])
				Max.T[1] = command.where.T[1];
			if(command.where.T[2] > Max.T[2])
				Max.T[2] = command.where.T[2];
			commands.push_back(command);
		}
	}

	Center.T[0] = (Max.T[0] + Min.T[0] )/2;
	Center.T[1] = (Max.T[1] + Min.T[1] )/2;
	Center.T[2] = (Max.T[2] + Min.T[2] )/2;

	// Find zoom

	float L=0;
	if(Max.T[0] - Min.T[0] > L)
		L = Max.T[0] - Min.T[0];
	if(Max.T[1] - Min.T[1] > L)
		L = Max.T[1] - Min.T[1];
	if(Max.T[2] - Min.T[2] > L)
		L = Max.T[2] - Min.T[2];

	zoom= L;

}
void GCode::CenterView()
{
	glTranslatef(-Center.T[0], -Center.T[1], -Center.T[2]);
}

/*

void CubeView::draw() {
    if (!valid()) {
        glLoadIdentity();
        glViewport(0,0,w(),h());
        glOrtho(-10,10,-10,10,-20050,10000);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(xshift, yshift, 0);
    glRotatef(hAng,0,1,0); glRotatef(vAng,1,0,0);
    glScalef(float(size),float(size),float(size));

    drawCube();
    
    glPopMatrix();
}*/

void GCode::draw()
{
    if (!valid())
	{
	glLoadIdentity();
	glViewport (0, 0, w(),h());											// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, w()/h(),1.0f, 1000.0f);						// Calculate The Aspect Ratio Of The Window
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix
	return;
	}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear Screen And Depth Buffer
	glLoadIdentity();												// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-zoom*2);								// Move Left 1.5 Units And Into The Screen 6.0
	
	glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	CenterView();

    glPushMatrix();													// NEW: Prepare Dynamic Transform
	glColor3f(0.75f,0.75f,1.0f);


	/*--------------- Drawing -----------------*/

	glBegin(GL_LINE_STRIP);
	for(UINT i=0;i<commands.size();i++)
	{
		switch(commands[i].Code)
		{
		case COORDINATEDMOTION:
			glVertex3f(commands[i].where.T[0], commands[i].where.T[1], commands[i].where.T[2] );
			break;
		}
	}
	glEnd();


	// Draw bbox
	glColor3f(1,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.T[0], Min.T[1], Min.T[2]);
	glVertex3f(Max.T[0], Min.T[1], Min.T[2]);
	glVertex3f(Max.T[0], Max.T[1], Min.T[2]);
	glVertex3f(Min.T[0], Max.T[1], Min.T[2]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.T[0], Min.T[1], Max.T[2]);
	glVertex3f(Max.T[0], Min.T[1], Max.T[2]);
	glVertex3f(Max.T[0], Max.T[1], Max.T[2]);
	glVertex3f(Min.T[0], Max.T[1], Max.T[2]);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(Min.T[0], Min.T[1], Min.T[2]);
	glVertex3f(Min.T[0], Min.T[1], Max.T[2]);
	glVertex3f(Min.T[0], Max.T[1], Min.T[2]);
	glVertex3f(Min.T[0], Max.T[1], Max.T[2]);
	glVertex3f(Max.T[0], Max.T[1], Min.T[2]);
	glVertex3f(Max.T[0], Max.T[1], Max.T[2]);
	glVertex3f(Max.T[0], Min.T[1], Min.T[2]);
	glVertex3f(Max.T[0], Min.T[1], Max.T[2]);
	glEnd();

	/*--------------- Draw STL ------------------*/

	stl.draw();

	/*--------------- Exit -----------------*/
	glPopMatrix();													// NEW: Unapply Dynamic Transform
	glFlush ();														// Flush The GL Rendering Pipeline
}


int GCode::handle(int event) 
{
	switch(event) {
		case FL_PUSH:	//mouse down event position in Fl::event_x() and Fl::event_y()
			{
				MousePt.T[0] = (GLfloat)Fl::event_x();
				MousePt.T[1] = (GLfloat)Fl::event_y();
				switch(Fl::event_button())	{
				case FL_LEFT_MOUSE:
					ArcBall->click(&MousePt);								// Update Start Vector And Prepare For Dragging
					break;
				case FL_MIDDLE_MOUSE: break;
				case FL_RIGHT_MOUSE:
					Matrix3fSetIdentity(&LastRot);								// Reset Rotation
					Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
					Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
					break;
				}
				LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
				redraw();
				return 1;
			}
		case FL_DRAG:	//mouse moved while down event ...
			MousePt.T[0] = (GLfloat)Fl::event_x();
			MousePt.T[1] = (GLfloat)Fl::event_y();
            Quat4fT     ThisQuat;

            ArcBall->drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
			redraw();
			return 1;
		case FL_RELEASE: //mouse up event ...
			MousePt.T[0] = (GLfloat)Fl::event_x();
			MousePt.T[1] = (GLfloat)Fl::event_y();
			redraw();
			return 1;
		case FL_FOCUS :
		case FL_UNFOCUS : // Return 1 if you want keyboard events, 0 otherwise
			return 0;
		case FL_KEYBOARD: //keypress, key is in Fl::event_key(), ascii in Fl::event_text() .. Return 1 if you understand/use the keyboard event, 0 otherwise...
			return 1;
		case FL_SHORTCUT: // shortcut, key is in Fl::event_key(), ascii in Fl::event_text() ... Return 1 if you understand/use the shortcut event, 0 otherwise...
			return 1;
		default:
			// pass other events to the base class...
			return Fl_Gl_Window::handle(event);
	}
}

void GCode::resize(int x,int y, int width, int height)					// Reshape The Window When It's Moved Or Resized
{
	Fl_Gl_Window::resize(x,y,width,height);
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Reset The Current Viewport
	glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
	glLoadIdentity ();													// Reset The Projection Matrix
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
					1.0f, 1000.0f);		
	glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity ();													// Reset The Modelview Matrix

    ArcBall->setBounds((GLfloat)width, (GLfloat)height);                 //*NEW* Update mouse bounds for arcball
}
