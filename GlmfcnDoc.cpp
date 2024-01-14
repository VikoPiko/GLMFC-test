// GlmfcnDoc.cpp : implementation of the CGlmfcnDoc class
//

#include "stdafx.h"
#include "Glmfcn.h"

#include "GlmfcnDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnDoc

IMPLEMENT_DYNCREATE(CGlmfcnDoc, CDocument)

BEGIN_MESSAGE_MAP(CGlmfcnDoc, CDocument)
	//{{AFX_MSG_MAP(CGlmfcnDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnDoc construction/destruction

CGlmfcnDoc::CGlmfcnDoc()
{

	// TODO: add one-time construction code here

}

CGlmfcnDoc::~CGlmfcnDoc()
{
}

BOOL CGlmfcnDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	radius = 0.5;
	height = 0.7;
	red = 1.0f; green = 1.0f; blue = 1.0f;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_pDC=NULL;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGlmfcnDoc serialization

void CGlmfcnDoc::Serialize(CArchive& ar)
{
	CString oneLine, token;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		ar.ReadString(oneLine);
		radius = _ttof(oneLine);
		ar.ReadString(oneLine);
		height = _ttof(oneLine);
		ar.ReadString(oneLine);
		int pos = 0;
		token = oneLine.Tokenize(" ", pos);
		red = _ttof(token);
		token = oneLine.Tokenize(" ", pos);
		green = _ttof(token);
		token = oneLine.Tokenize(" ", pos);
		blue = _ttof(token);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnDoc diagnostics

#ifdef _DEBUG
void CGlmfcnDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGlmfcnDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGlmfcnDoc commands
