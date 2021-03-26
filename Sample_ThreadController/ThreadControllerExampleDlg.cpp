
// MFCApplication6Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ThreadControllerExample.h"
#include "ThreadControllerExampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ThreadControllerExampleDlg::ThreadControllerExampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void ThreadControllerExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ThreadControllerExampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &ThreadControllerExampleDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ThreadControllerExampleDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCApplication6Dlg message handlers

BOOL ThreadControllerExampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ThreadControllerExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ThreadControllerExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ThreadControllerExampleDlg::OnBnClickedButton1()
{
	adsWorker.start(std::bind(&ThreadControllerExampleDlg::worker, this));
}

void ThreadControllerExampleDlg::OnBnClickedButton2()
{
	if (adsWorker.isWorking() == true)
	{
		adsWorker.pause();
		GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("Resume"));

	}
	else
	{
		adsWorker.resume();
		GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("Pause"));

	}
}

void ThreadControllerExampleDlg::worker()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);

	str.Append(_T("\r\n") + CString(std::to_wstring(count++).c_str()));

	GetDlgItem(IDC_EDIT1)->SetWindowText(str);

}
