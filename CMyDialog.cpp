// CMyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Glmfcn.h"
#include "afxdialogex.h"
#include "CMyDialog.h"


// CMyDialog dialog

IMPLEMENT_DYNAMIC(CMyDialog, CDialogEx)

CMyDialog::CMyDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, R(0)
	, G(0)
	, B(0)
	, Size(_T(""))
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER1, R);
	DDX_Slider(pDX, IDC_SLIDER3, G);
	DDX_Slider(pDX, IDC_SLIDER2, B);
	DDX_Text(pDX, IDC_EDIT2, Size);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
END_MESSAGE_MAP()


// CMyDialog message handlers
