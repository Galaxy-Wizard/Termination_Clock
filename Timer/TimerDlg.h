// TimerDlg.h: файл заголовка
//

#pragma once

#include <afxdialogex.h>
#include <afxmt.h>
#include "CStaticExtended.h"

// Диалоговое окно CTimerDlg
class CTimerDlg : public CDialogEx
{
// Создание
public:
	CTimerDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CTimeSpan InitialEventTime;
	CTimeSpan CurrentEventTime;
	CStaticExtended TimeLeft;
	bool Stop;
	COLORREF CurrentColor;
	CCriticalSection StopCriticalSection;
	afx_msg void OnBnClickedButton2();
	CDateTimeCtrl TerminationDateTime;
	afx_msg void OnBnClickedButton3();
};
