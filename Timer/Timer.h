
// Timer.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTimerApp:
// Сведения о реализации этого класса: Timer.cpp
//

class CTimerApp : public CWinApp
{
public:
	CTimerApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTimerApp theApp;
