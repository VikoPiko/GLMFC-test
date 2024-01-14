// GlmfcnView.cpp : implementation of the CGlmfcnView class
//

#include "stdafx.h"
#include "Glmfcn.h"

#include "GlmfcnDoc.h"
#include "GlmfcnView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "glaux.h"

#include "CMyDialog.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

float sphereSize = 0.7f;
float r = 1.0f, g = 1.0f, b = 1.0f;
bool useCylinder = true;

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView

IMPLEMENT_DYNCREATE(CGlmfcnView, CView)

BEGIN_MESSAGE_MAP(CGlmfcnView, CView)
	//{{AFX_MSG_MAP(CGlmfcnView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_VIEW_DIALOG, &CGlmfcnView::OnViewDialog)
    ON_COMMAND(ID_VIEW_SWAP, &CGlmfcnView::OnViewSwap)
END_MESSAGE_MAP()

float spin=0.0;

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView construction/destruction

CGlmfcnView::CGlmfcnView()
{
	// TODO: add construction code here

}

CGlmfcnView::~CGlmfcnView()
{
}

BOOL CGlmfcnView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

BOOL CGlmfcnView::InitializeOpenGL()
{
	CGlmfcnDoc* pDoc = GetDocument();
    pDoc->m_pDC = new CClientDC(this);
	m_hDC = pDoc->m_pDC->GetSafeHdc();

    if ( NULL == pDoc->m_pDC ) // failure to get DC
        {
		::AfxMessageBox("Couldn't get a valid DC.");
        return FALSE;
        }

    if ( !SetupPixelFormat() )
        {
		::AfxMessageBox("SetupPixelFormat failed.\n");
        return FALSE;
        }
	m_hRC=wglGetCurrentContext();
	if (m_hRC==NULL)
    if ( 0 == (m_hRC = ::wglCreateContext( m_hDC ) ) )
        {
		::AfxMessageBox("wglCreateContext failed.");
        return FALSE;
        }


    return makeCurrent();
}


BOOL CGlmfcnView::SetupPixelFormat()
{
    // default pixel format for a single-buffered,
    // OpenGL-supporting, hardware-accelerated, 
    // RGBA-mode format. Pass in a pointer to a different
    // pixel format if you want something else
	CGlmfcnDoc* pDoc = GetDocument();
    PIXELFORMATDESCRIPTOR pfd = 
        {
        sizeof(PIXELFORMATDESCRIPTOR),// size of this pfd
        1,                      // version number
        PFD_DRAW_TO_WINDOW |    // support window
		PFD_DOUBLEBUFFER |		// doubble buffer
        PFD_SUPPORT_OPENGL,		// support OpenGL
        PFD_TYPE_RGBA,          // RGBA type
        24,                     // 24-bit color depth
        0, 0, 0, 0, 0, 0,       // color bits ignored
        0,                      // no alpha buffer
        0,                      // shift bit ignored
        0,                      // no accumulation buffer
        0, 0, 0, 0,             // accum bits ignored
        16,                     // 16-bit z-buffer
        0,                      // no stencil buffer
        0,                      // no auxiliary buffer
        PFD_MAIN_PLANE,         // main layer
        0,                      // reserved
        0, 0, 0                 // layer masks ignored
        };

    int pixelformat;
    PIXELFORMATDESCRIPTOR* pPFDtoUse;

    // let the user override the default pixel format
//    pPFDtoUse = (0 == pPFD)? &pfd : pPFD; 
    pPFDtoUse =  &pfd ; 

    if ( 0 == (pixelformat = 
        ::ChoosePixelFormat( m_hDC, pPFDtoUse )) )
        {
    ::AfxMessageBox("ChoosePixelFormat failed.");
    return FALSE;
        }

    if ( FALSE == ::SetPixelFormat( m_hDC,
        pixelformat, pPFDtoUse ) )
        {
    ::AfxMessageBox("SetPixelFormat failed.");
        return FALSE;
        }

    return TRUE;
}

void CGlmfcnView::myReshape()
{
	CGlmfcnDoc* pDoc = GetDocument();
    // compute the aspect ratio
    // this will keep all dimension scales equal
    pDoc->m_AspectRatio = (GLdouble)pDoc->m_WindowWidth/(GLdouble)pDoc->m_WindowHeight;

    // Now, set up the viewing area-select the full client area
    ::glViewport(0, 0, pDoc->m_WindowWidth, pDoc->m_WindowHeight);

    if ( GL_NO_ERROR != ::glGetError() )
    {
    ::AfxMessageBox("Error while trying to set viewport.");
    }

    // select the projection matrix as the recipient of
    // matrix operations (there's three to choose from)
    ::glMatrixMode(GL_PROJECTION);
    // initialize the projection matrix to a pristine state
    ::glLoadIdentity();

    // select the viewing volume. You do it after you
    // get the aspect ratio and set the viewport
//    ::gluPerspective( 45.0f, pDoc->m_AspectRatio, 0.1f, 20.0f );
    float r=1.0/pDoc->m_AspectRatio;

    if (pDoc->m_WindowWidth <= pDoc->m_WindowHeight) 
        gluPerspective(45.0f, pDoc->m_AspectRatio, 0.1f, 20.0f);
    else 
        glOrtho (0, 2.0/r, 0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);

    // NOTE: Other commands you could have here are
    // glFrustum, which gives you much more control over
    // the perspective view, or glOrtho which is used for
    // parallel projections. No matter what you use, check
    // the error status when you set the viewing frustum!

    if ( GL_NO_ERROR != ::glGetError() )
    {
		::AfxMessageBox("Error while trying to set viewing frustum.");
		return ;
    }


    // now select the modelview matrix and clear it
    // this is the mode we do most of our calculations in
    // so we leave it as the default mode.
    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();
  
    // now perform a default viewing transformations
//    ::glTranslatef( 0.0f, 0.0f, -5.0f );
//    ::glRotatef( 20.0f, 1.0f, 0.0f, 0.0f );

}

BOOL CGlmfcnView::makeCurrent()
{
	CGlmfcnDoc* pDoc = GetDocument();
//	HGLRC hglrc = 	wglGetCurrentContext();
//	if (hglrc!=pDoc->m_hRC )
    if ( FALSE == ::wglMakeCurrent(m_hDC, m_hRC ) )
        {
		::AfxMessageBox("wglMakeCurrent failed.");
        return FALSE;
        }

    // specify black as clear color
    ::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    // specify the back of the buffer as clear depth
    ::glClearDepth( 1.0f );
    // enable depth testing
    ::glEnable( GL_DEPTH_TEST );
    
return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView drawing

void CGlmfcnView::OnDraw(CDC* pDC)
{
	CGlmfcnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
//	glClearColor(0.1, 0.1, 0.1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
    glTranslatef(1, 0.5, 0);
    glRotatef(30, 0, 1, 0);
    glColor3f(pDoc->red, pDoc->green, pDoc->blue);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_dif[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 30.0 }; 
    GLfloat light_position[] = { 0, 1.0, 0.5, 0.0 }; // x y z w positions

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    //GL_LIGHT0();
    
    if (useCylinder)
        auxSolidCylinder(pDoc->radius, pDoc->height);
    else
        //auxSolidSphere(pDoc->radius);
        auxWireCylinder(pDoc->radius, pDoc->height);
    // auxWireTeapot()

	BOOL ok=SwapBuffers(m_hDC);
}

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView diagnostics

#ifdef _DEBUG
void CGlmfcnView::AssertValid() const
{
	CView::AssertValid();
}

void CGlmfcnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGlmfcnDoc* CGlmfcnView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGlmfcnDoc)));
	return (CGlmfcnDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnView message handlers

int CGlmfcnView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
     InitializeOpenGL();
	
	return 0;
}


void CGlmfcnView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CGlmfcnDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
    if ( 0 >= cx || 0 >= cy )
        {
        return;
        }

    // save the width and height of the current window
    pDoc->m_WindowWidth = cx;
    pDoc->m_WindowHeight = cy;

	myReshape();
}


BOOL CGlmfcnView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CView::OnEraseBkgnd(pDC);
}



void CGlmfcnView::OnViewDialog()
{
    // TODO: Add your command handler code here
    CMyDialog dialog;
   
    //dialog.Size = sphereSize;
    dialog.R = r * 100.0f;
    dialog.G = g * 100.0f;
    dialog.B = b * 100.0f;

    dialog.Size = std::to_string(sphereSize).c_str();
   
    auto modalState = dialog.DoModal();

    if (modalState == IDOK)
    {
        r = dialog.R / 100.f;
        g = dialog.G / 100.f;
        b = dialog.B / 100.f;
        sphereSize = std::stof(dialog.Size.GetBuffer());

        Invalidate();
    }

}


void CGlmfcnView::OnViewSwap()
{
    // TODO: Add your command handler code here
    useCylinder = !useCylinder; // ?

    Invalidate();
}
