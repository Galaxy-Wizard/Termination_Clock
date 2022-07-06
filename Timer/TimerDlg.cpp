// TimerDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Timer.h"
#include "TimerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ==========================================================================
// system shutdown
// nSDType: 0 - Shutdown the system
//          1 - Shutdown the system and turn off the power (if supported)
//          2 - Shutdown the system and then restart the system
void SystemShutdown(UINT nSDType)
{
	HANDLE           hToken;
	TOKEN_PRIVILEGES tkp;

	::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1; // set 1 privilege
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// get the shutdown privilege for this process
	::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	switch (nSDType)
	{
	case 0: ::InitiateSystemShutdownExW(NULL, L"Termination_Clock application shutdown by user timer in 30 seconds", 30, FALSE, FALSE, SHTDN_REASON_FLAG_PLANNED); break;
	case 1: ::InitiateSystemShutdownExW(NULL, L"Termination_Clock application shutdown by user timer in 30 seconds", 30, FALSE, FALSE, SHTDN_REASON_FLAG_PLANNED); break;
	case 2: ::InitiateSystemShutdownExW(NULL, L"Termination_Clock application shutdown by user timer in 30 seconds", 30, FALSE, TRUE, SHTDN_REASON_FLAG_PLANNED); break;
	}
}

// Диалоговое окно CTimerDlg

const UINT_PTR EventNumber = 2000;
const UINT EventTimeOneSecond = 1000;
const COLORREF RedColor = RGB(255, 0, 0);
const COLORREF GrayColor = RGB(127, 127, 127);

CTimerDlg::CTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMER_DIALOG, pParent), Stop(true), CurrentColor(GrayColor)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, TimeLeft);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, TerminationDateTime);
}

BEGIN_MESSAGE_MAP(CTimerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTimerDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CTimerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTimerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// Обработчики сообщений CTimerDlg

BOOL CTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	CTime local_date_time;

	TerminationDateTime.SetFormat(L"hh:mm:ss          ddMMMMyyyy");

	TerminationDateTime.GetTime(local_date_time);

	CTimeSpan local_time_span = local_date_time - CTime();

	InitialEventTime = local_time_span;

	CurrentEventTime = CTime::GetCurrentTime() - CTime();

	CTimeSpan local_difference_span = InitialEventTime - CurrentEventTime;
	
	Stop = true;

	SetTimer(EventNumber, EventTimeOneSecond, nullptr);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTimerDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений

	CTime local_date_time;

	TerminationDateTime.GetTime(local_date_time);

	CTimeSpan local_time_span = local_date_time - CTime();

	InitialEventTime = local_time_span;

	CurrentEventTime = CTime::GetCurrentTime() - CTime();

	CTimeSpan local_difference_span = InitialEventTime - CurrentEventTime;

	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = false;
	CurrentColor = GrayColor;

	SingleLock.Unlock();
}


void CTimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	if (nIDEvent == EventNumber)
	{
		CSingleLock SingleLock(&StopCriticalSection);
		SingleLock.Lock();

		CTimeSpan Time_Difference = InitialEventTime - CurrentEventTime;

		if (!Stop)
		{
			if (Time_Difference.GetTotalSeconds() <= 0)
			{
				SystemShutdown(0);
			}
		}

		CString CurrentTimeString;
		if (Stop)
		{
			TimeLeft.SetTextColor(CurrentColor);

			CurrentTimeString.Format(CString(L"Время остановлено. Дни: %d Часы: %d Минуты: %d Секунды: %d"), Time_Difference.GetDays(), Time_Difference.GetHours(), Time_Difference.GetMinutes(), Time_Difference.GetSeconds());
			TimeLeft.SetWindowTextW(CurrentTimeString, Little);
		}
		else
		{
			CurrentColor = GrayColor;

			TimeLeft.SetTextColor(CurrentColor);

			CurrentEventTime += CTimeSpan(0, 0, 0, 1);

			CString CurrentTimeString;

			CurrentTimeString.Format(CString(L"Дни: %d Часы: %d Минуты: %d Секунды: %d"), Time_Difference.GetDays(), Time_Difference.GetHours(), Time_Difference.GetMinutes(), Time_Difference.GetSeconds());

			TimeLeft.SetWindowTextW(CurrentTimeString, Medium);
		}

		SingleLock.Unlock();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTimerDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	SystemShutdown(0);
}


void CTimerDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CSingleLock SingleLock(&StopCriticalSection);
	SingleLock.Lock();

	Stop = true;
	CurrentColor = RedColor;

	SingleLock.Unlock();
}
