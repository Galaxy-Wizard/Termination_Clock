#pragma once

#include <afxwin.h>

const int Little = 0;
const int Medium = 1;
const int Big = 2;

class CStaticExtended :
    public CStatic
{
protected:
    COLORREF CurrentColor;
    CString TextString;
    int Size;
public:
    virtual void SetTextColor(COLORREF ParameterCurrentColor);

    virtual void OnPaint();
    virtual void SetWindowTextW(LPCTSTR lpszString, int size);
};

