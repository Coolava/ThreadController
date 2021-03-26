
// MFCApplication6Dlg.h : header file
//

#pragma once
#include <string>
#include <thread>
#include "ThreadController.h"
// CMFCApplication6Dlg dialog
class CMFCApplication6Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication6Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION6_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedButton2();

	int count = 0;
	void worker();
	ThreadController adsWorker;
};
