/* A minimal wrappining of OpenGL and related functions. */

#include <stdio.h>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glut.h>		// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "avm.h"

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	//glEnable(GL_LIGHTING);
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(-1.5f,0.0f,-6.0f);
	glBegin(GL_TRIANGLES);						// Drawing Using Triangles
		glVertex3f( 0.0f, 1.0f, 0.0f);				// Top
		glVertex3f(-1.0f,-1.0f, 0.0f);				// Bottom Left
		glVertex3f( 1.0f,-1.0f, 0.0f);				// Bottom Right
	glEnd();
	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","Azure GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

_declspec (dllexport) void acreateglwin(struct _avm* vm)
{
	char* caption = (char*)getdata(vm->hp.heap, getarg(vm,0)->entity.ival);
	int x = (int)getarg(vm,1)->entity.ival;
	int y = (int)getarg(vm,2)->entity.ival;
	int colordepth = (int)getarg(vm,3)->entity.ival;
	int full = (int)getarg(vm,4)->entity.ival;
	CreateGLWindow(caption, x, y, colordepth, (bool)full);
}

_declspec (dllexport) void akillglwin(struct _avm* vm)
{
	KillGLWindow();
}

_declspec (dllexport) void ahandlemsg(struct _avm* vm)
{
	MSG msg;
	int done = 0;
	word w;
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
	{
		if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			done=1;							// If So done=TRUE
		else									// If Not, Deal With Window Messages
		{
			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
	}
	seti(&w, done);
	returnv(vm, &w);
}

_declspec (dllexport) void agetinput(struct _avm* vm)
{
	word w;
	int key = 0;
	int i;
	if (active)								// Program Active?
		for (i=1; i<256; keys[i]=0,i++)
			if (keys[i])
				key=i;
	seti(&w, key);
	returnv(vm, &w);
}

_declspec (dllexport) void adrawscene(struct _avm* vm)
{
	SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
}

_declspec (dllexport) void aglClear(struct _avm* vm)
{
	int p = (int)getarg(vm, 0)->entity.ival;
	glClear(p);
}

_declspec (dllexport) void aglLoadIdentity(struct _avm* vm)
{
	glLoadIdentity();
}

_declspec (dllexport) void aglTranslatef(struct _avm* vm)
{
	float f1 = (float)getarg(vm, 0)->entity.fval;
	float f2 = (float)getarg(vm, 1)->entity.fval;
	float f3 = (float)getarg(vm, 2)->entity.fval;
	glTranslatef(f1,f2,f3);
}

_declspec (dllexport) void aglRotatef(struct _avm* vm)
{
	float f1 = (float)getarg(vm, 0)->entity.fval;
	float f2 = (float)getarg(vm, 1)->entity.fval;
	float f3 = (float)getarg(vm, 2)->entity.fval;
	float f4 = (float)getarg(vm, 3)->entity.fval;
	glRotatef(f1,f2,f3,f4);
}

_declspec (dllexport) void aglBegin(struct _avm* vm)
{
	int p = (int)getarg(vm, 0)->entity.ival;
	glBegin(p);
}

_declspec (dllexport) void aglEnd(struct _avm* vm)
{
	glEnd();
}

_declspec (dllexport) void aglEnable(struct _avm* vm)
{
	int attr = (int)getarg(vm, 0)->entity.ival;
	glEnable(attr);
}

_declspec (dllexport) void aglDisable(struct _avm* vm)
{
	int attr = (int)getarg(vm, 0)->entity.ival;
	glDisable(attr);
}

_declspec (dllexport) void aglShadeModel(struct _avm* vm)
{
	int sm = (int)getarg(vm, 0)->entity.ival;
	glShadeModel(sm);
}

/* Pack a table containing floats into a native float array. */
float* packf(ctable* table)
{
	cnode *node, *nodes = (cnode*)&(table->nodes);
	float* arr = (float*)calloc(table->tablesize, sizeof(float));
	int i;
	for (i=0; i<table->tablesize; i++)
	{
		node = &nodes[i];
		arr[i] = node->value.entity.fval;
	}	
	return arr;
}

_declspec (dllexport) void aglLightfv(struct _avm* vm)
{
	int light = (int)getarg(vm, 0)->entity.ival;
	int pname = (int)getarg(vm, 1)->entity.ival;
	long toff = vm->tot.table[getarg(vm, 2)->entity.ival].offset;
	ctable* table = (ctable*)getdata(vm->hp.heap, toff);
	float* arr = packf(table);
	glLightfv(light, pname, arr);
	free(arr);
}

_declspec (dllexport) void aglClearColor(struct _avm* vm)
{
	float r = (float)getarg(vm, 0)->entity.fval;
	float g = (float)getarg(vm, 1)->entity.fval;
	float b = (float)getarg(vm, 2)->entity.fval;
	float a = (float)getarg(vm, 3)->entity.fval;
	glClearColor(r,g,b,a);
}

_declspec (dllexport) void aglClearDepth(struct _avm* vm)
{
	int depth = (int)getarg(vm, 0)->entity.ival;
	glClearDepth(depth);
}

_declspec (dllexport) void aglColor3f(struct _avm* vm)
{
	float f1 = (float)getarg(vm, 0)->entity.fval;
	float f2 = (float)getarg(vm, 1)->entity.fval;
	float f3 = (float)getarg(vm, 2)->entity.fval;
	glColor3f(f1,f2,f3);
}

_declspec (dllexport) void aglVertex3f(struct _avm* vm)
{
	float f1 = (float)getarg(vm, 0)->entity.fval;
	float f2 = (float)getarg(vm, 1)->entity.fval;
	float f3 = (float)getarg(vm, 2)->entity.fval;
	glVertex3f(f1,f2,f3);
}

_declspec (dllexport) void aglPolygonMode(struct _avm* vm)
{
	int face = (int)getarg(vm, 0)->entity.ival;
	int mode = (int)getarg(vm, 1)->entity.ival;
	glPolygonMode(face, mode);
}

_declspec (dllexport) void aglTexCoord2f(struct _avm* vm)
{
	float s = (float)getarg(vm, 0)->entity.ival;
	float t = (float)getarg(vm, 1)->entity.ival;
	glTexCoord2f(s,t);
}

_declspec (dllexport) void aglMatrixMode(struct _avm* vm)
{
	int mode = (int)getarg(vm, 0)->entity.ival;
	glMatrixMode(mode);
}

_declspec (dllexport) void aglFrustum(struct _avm* vm)
{
	float left = (float)getarg(vm, 0)->entity.fval;
	float right = (float)getarg(vm, 1)->entity.fval;
	float bottom = (float)getarg(vm, 2)->entity.fval;
	float top = (float)getarg(vm, 3)->entity.fval;
	float znear = (float)getarg(vm, 4)->entity.fval;
	float zfar = (float)getarg(vm, 5)->entity.fval;
	glFrustum(left, right, bottom, top, znear, zfar);
}

_declspec (dllexport) void aglCullFace(struct _avm* vm)
{
	int mode = (int)getarg(vm, 0)->entity.ival;
	glCullFace(mode);
}

_declspec (dllexport) void aglFrontFace(struct _avm* vm)
{
	int mode = (int)getarg(vm, 0)->entity.ival;
	glFrontFace(mode);
}

_declspec (dllexport) void aglDepthFunc(struct _avm* vm)
{
	int mode = (int)getarg(vm, 0)->entity.ival;
	glDepthFunc(mode);
}

_declspec (dllexport) void aglHint(struct _avm* vm)
{
	int target = (int)getarg(vm, 0)->entity.ival;
	int mode = (int)getarg(vm, 1)->entity.ival;
	glHint(target, mode);
}

_declspec (dllexport) void aglLoadTex(struct _avm* vm)
{
	char* tex = (char*)getdata(vm->hp.heap, getarg(vm,0)->entity.ival);

}

_declspec (dllexport) void agluSphere(struct _avm* vm)
{
	float f = (float)getarg(vm, 0)->entity.fval;
	int i1 = (int)getarg(vm, 1)->entity.ival;
	int i2 = (int)getarg(vm, 2)->entity.ival;
	gluSphere(gluNewQuadric(), f,i1,i2);
}

_declspec (dllexport) void agluCylinder(struct _avm* vm)
{
	float f1 = (float)getarg(vm, 0)->entity.fval;
	float f2 = (float)getarg(vm, 1)->entity.fval;
	float f3 = (float)getarg(vm, 2)->entity.fval;
	int i1 = (int)getarg(vm, 3)->entity.ival;
	int i2 = (int)getarg(vm, 4)->entity.ival;
	gluCylinder(gluNewQuadric(), f1,f2,f3,i1,i2);
}

_declspec (dllexport) void aglutSolidTeapot(struct _avm* vm)
{
	float f = (float)getarg(vm, 0)->entity.fval;
	glutSolidTeapot(f);
}

_declspec (dllexport) void aglutWireTeapot(struct _avm* vm)
{
	float f = (float)getarg(vm, 0)->entity.fval;
	glutWireTeapot(f);
}

_declspec (dllexport) void aGetTickCount(struct _avm* vm)
{
	word w;
	seti(&w, GetTickCount());
	returnv(vm, &w);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
