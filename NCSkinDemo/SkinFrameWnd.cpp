// SkinFrameWnd.cpp : implementation file
////Download by http://www.cctry.com

#include "stdafx.h"
#include "SkinFrameWnd.h"

#define  ID_CLOSE_BTN 110
#define  ID_MAX_BTN   111
#define  ID_MIN_BTN   112

// CSkinFrameWnd

IMPLEMENT_DYNCREATE(CSkinFrameWnd,CFrameWndEx)

CSkinFrameWnd::CSkinFrameWnd()
{
	m_nTitleHeight = 38;
	m_nFrameWidth = 3;

	Image *pImage = CSkinManager::GetInstance()->GetSkinItem(_T("dialog\\dlg_minimize_button.bmp"));
	m_WindowBtnImage.push_back(pImage);

	pImage = CSkinManager::GetInstance()->GetSkinItem(_T("dialog\\dlg_maxmize_button.bmp"));
	m_WindowBtnImage.push_back(pImage);

	pImage = CSkinManager::GetInstance()->GetSkinItem(_T("dialog\\dlg_close_btn.bmp"));
	m_WindowBtnImage.push_back(pImage);

	m_pBkImage = CSkinManager::GetInstance()->GetSkinItem(_T("dialog\\DialogBkg.png"));

	m_iXButtonHovering = -1;

	m_bNCHovering = FALSE;
	m_bNCTracking = FALSE;

	m_rcIcon = CRect(15,13,15+GetSystemMetrics(SM_CXSMICON),13+GetSystemMetrics(SM_CYSMICON));

}

CSkinFrameWnd::~CSkinFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CSkinFrameWnd, CFrameWndEx)
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCMOUSEHOVER()
	ON_WM_NCMOUSELEAVE()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_NCCREATE()
END_MESSAGE_MAP()


// CSkinFrameWnd message handlers

void CSkinFrameWnd::OnNcPaint()
{
	CWindowDC dc(this);

	if (m_pBkImage==NULL)
	{
		return;
	}

	CDC MemDC;

	// 获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect( -rcWindow.left, -rcWindow.top);


	CRect rcClient;
	GetClientRect(&rcClient);

	//剪除掉客户区
	rcClient.OffsetRect(m_nFrameWidth, m_nTitleHeight);
	dc.ExcludeClipRect(rcClient.left, rcClient.top,
		rcClient.right, rcClient.bottom);


	MemDC.CreateCompatibleDC(&dc);


	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,rcWindow.Width(),rcWindow.Height());
	MemDC.SelectObject(&bmp);
	MemDC.SetBkMode(TRANSPARENT);

	Gdiplus::Graphics graphics(MemDC.GetSafeHdc());
	RectF destRect;

	//绘制底部和边框
	destRect.X = 0;
	destRect.Y = rcWindow.Height() - m_nFrameWidth;
	destRect.Width = rcWindow.Width();
	destRect.Height = m_nFrameWidth;
	graphics.DrawImage(m_pBkImage,destRect,m_nTitleHeight,m_pBkImage->GetHeight()-m_nFrameWidth,m_nFrameWidth,m_nFrameWidth,UnitPixel);

	destRect.X = rcWindow.right-m_nTitleHeight;
	destRect.Y = rcWindow.bottom - m_nTitleHeight;
	destRect.Width = m_nTitleHeight;
	destRect.Height = m_nTitleHeight;
	graphics.DrawImage(m_pBkImage,destRect,m_pBkImage->GetWidth()-m_nTitleHeight,m_pBkImage->GetHeight()-m_nTitleHeight,m_nTitleHeight,m_nTitleHeight,UnitPixel);

	destRect.X = 0;
	destRect.Y = rcWindow.Height() - m_nTitleHeight;
	destRect.Width = m_nTitleHeight;
	destRect.Height = m_nTitleHeight;
	graphics.DrawImage(m_pBkImage,destRect,0,m_pBkImage->GetHeight()-m_nTitleHeight,m_nTitleHeight,m_nTitleHeight,UnitPixel);

	//左边框
	destRect.X = 0;
	destRect.Y = m_nTitleHeight;
	destRect.Width = m_nFrameWidth;
	destRect.Height = rcWindow.Height() - 2*m_nTitleHeight;
	graphics.DrawImage(m_pBkImage,destRect,0,m_nTitleHeight,m_nFrameWidth,m_nFrameWidth,UnitPixel);


	//右边框
	destRect.X = rcWindow.Width() - m_nFrameWidth;
	destRect.Y = m_nTitleHeight;
	destRect.Width = m_nFrameWidth;
	destRect.Height = rcWindow.Height() - 2*m_nTitleHeight;
	graphics.DrawImage(m_pBkImage,destRect,m_pBkImage->GetWidth()-m_nFrameWidth,m_nTitleHeight,m_nFrameWidth,m_nFrameWidth,UnitPixel);

	//绘制标题栏
	destRect.X = 0;
	destRect.Y = 0;
	destRect.Width = rcWindow.Width();
	destRect.Height = m_nTitleHeight;
	graphics.DrawImage(m_pBkImage,destRect,50,0,5,m_nTitleHeight,UnitPixel);
	graphics.DrawImage(m_pBkImage,0,0,0,0,50,m_nTitleHeight,UnitPixel);
	graphics.DrawImage(m_pBkImage,rcWindow.Width() - 50,0,m_pBkImage->GetWidth()-50,0,50,m_nTitleHeight,UnitPixel);


	int xPos = 15;
	int yPos = 15;

	HICON hIcon = GetIcon(FALSE);
	if (hIcon)
	{
		DrawIconEx(MemDC,xPos, yPos-2,hIcon,GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0, 0, DI_NORMAL);

		xPos += GetSystemMetrics(SM_CXSMICON);
		xPos += 5;
	}

	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		//绘制标题
		LOGFONT lfFont;
		memset(&lfFont, 0, sizeof(lfFont));
		lfFont.lfHeight = -12;
		lfFont.lfWeight |= FW_BOLD;
		lstrcpy(lfFont.lfFaceName, _T("宋体"));
		Gdiplus::Font font(dc.GetSafeHdc(), &lfFont);

		StringFormat stringFormat;
		stringFormat.SetFormatFlags( StringFormatFlagsNoWrap);
		stringFormat.SetAlignment(StringAlignmentNear);
		stringFormat.SetLineAlignment(StringAlignmentNear);
		CStringW strTitle(strText);
		SolidBrush brush((ARGB)Color::White);
		PointF point;

		point.X=(Gdiplus::REAL)xPos;
		point.Y=(Gdiplus::REAL)yPos;
		graphics.DrawString(strTitle, strTitle.GetLength(), &font, point,&stringFormat, &brush);

	}

	CRect rcPaint;
	dc.GetClipBox(&rcPaint);
	std::map<int,CDUIButton>::iterator iter;
	for (iter = m_TitleBtn.begin(); iter != m_TitleBtn.end(); iter++)
	{
		CDUIButton& dcControl = iter->second;
		CRect rcControl;
		dcControl.GetRect(&rcControl);

		// 判断当前按钮是否需要重绘
		if(!rcPaint.IsRectEmpty() && !CRect().IntersectRect(&rcControl, rcPaint))
		{
			continue;
		}

		dcControl.DrawButton(graphics);
	}



	dc.BitBlt(0,0,rcWindow.Width(),rcWindow.Height(),&MemDC,0,0,SRCCOPY);

	dc.SelectClipRgn(NULL);
}

void CSkinFrameWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (bCalcValidRects)   
	{   

		CRect& rc = (CRect&)lpncsp->rgrc[0];//get the client rectangle   
		rc.top += m_nTitleHeight;   
		rc.left += m_nFrameWidth;   
		rc.bottom -= m_nFrameWidth;   
		rc.right -= m_nFrameWidth;   

	}
}

LRESULT CSkinFrameWnd::OnNcHitTest(CPoint point)
{
	CRect rtButton;
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	CPoint pt = point;
	pt.x -=rcWindow.left;
	pt.y -= rcWindow.top;

	int iButton = TButtonHitTest(pt, rtButton);
	if(iButton != -1)
	{
		switch(iButton)
		{
		case ID_CLOSE_BTN:
			{
				return HTCLOSE;
			}
			break;
		case ID_MAX_BTN:
			{

				return HTMAXBUTTON;
			}
			break;
		case ID_MIN_BTN:
			{
				return HTMINBUTTON;
			}
			break;
		}
	}
	else if (m_rcIcon.PtInRect(pt))
	{
		return HTSYSMENU;
	}

	return CFrameWndEx::OnNcHitTest(point);
}

void CSkinFrameWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	 lpMMI->ptMinTrackSize.y = m_nTitleHeight + m_nFrameWidth;
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

BOOL CSkinFrameWnd::OnNcActivate(BOOL bActive)
{
	 OnNcPaint();
	 return TRUE;
}

void CSkinFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

	OnNcPaint();
}

void CSkinFrameWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.bottom = rcWindow.top + m_nTitleHeight;


	CPoint pt = point;
    CRect rtButton;
	int iButton = TButtonHitTest(pt, rtButton);
	if(iButton != -1)
	{
		m_TitleBtn[iButton].LButtonDown();
		OnNcPaint();
		return;
	}

	if (rcWindow.PtInRect(pt))
	{
		if (IsZoomed())
		{
			ShowWindow(SW_RESTORE);
		}
		else
		{
			ShowWindow(SW_MAXIMIZE);
		}

		OnNcPaint();
		return;
	}

	OnNcPaint();
}

void CSkinFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CRect rtButton;
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	CPoint pt = point;
	pt.x -=rcWindow.left;
	pt.y -= rcWindow.top;

	int iButton = TButtonHitTest(pt, rtButton);
	if(iButton != -1)
	{
		m_TitleBtn[iButton].LButtonDown();
		OnNcPaint();
		return;
	}
	
	
	if(m_rcIcon.PtInRect(pt)) 
	{
		CMenu *pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu)
		{
			pSysMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y,this, NULL);
		}
		return;
	}
	else if(nHitTest >= HTLEFT && nHitTest <= HTBOTTOMRIGHT || nHitTest == HTCAPTION)
	{		
		CFrameWnd::OnNcLButtonDown(nHitTest, point);
		return;
	}


	CFrameWndEx::OnNcLButtonDown(nHitTest, point);
}

void CSkinFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	CRect rtButton;
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	CPoint pt = point;
	pt.x -=rcWindow.left;
	pt.y -= rcWindow.top;

	int iButton = TButtonHitTest(pt, rtButton);
	if(iButton != -1)
	{
		switch(iButton)
		{
		case ID_CLOSE_BTN:
			{
				SendMessage(WM_SYSCOMMAND,SC_CLOSE,0);
			}
			break;
		case ID_MAX_BTN:
			{
				if (IsZoomed())
				{
					SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
				}
				else
				{
					SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
				}
			}
			break;
		case ID_MIN_BTN:
			{
				SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
			}
			break;
		default:
			{
               m_TitleBtn[iButton].LButtonUp();
			   OnNcPaint();
			}
			break;
		}
	}
	else
	{
		CFrameWndEx::OnNcLButtonUp(nHitTest, point);
	}
}

void CSkinFrameWnd::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if (!m_bNCTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_NONCLIENT;
		tme.dwHoverTime = 1;
		m_bNCTracking = _TrackMouseEvent(&tme);
	}

	if(nHitTest>=HTLEFT && nHitTest <= HTBOTTOMRIGHT || 
		nHitTest == HTCAPTION)
	{
		CFrameWnd::OnNcMouseMove(nHitTest, point);

	}

	CRect rtButton;
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	point.x -=rcWindow.left;
	point.y -= rcWindow.top;
	int iButton = TButtonHitTest(point, rtButton);
	if(iButton != m_iXButtonHovering)
	{
		if(m_iXButtonHovering != -1)
		{
			m_TitleBtn[m_iXButtonHovering].MouseLeave();
			m_iXButtonHovering = -1;
		}
		if(iButton != -1)
		{
			m_iXButtonHovering = iButton;
			m_TitleBtn[m_iXButtonHovering].MouseHover();
		}

		OnNcPaint();
	}
}

void CSkinFrameWnd::OnNcMouseHover(UINT nFlags, CPoint point)
{
	m_bNCHovering = TRUE;
	//CFrameWndEx::OnNcMouseHover(nFlags, point);
}

void CSkinFrameWnd::OnNcMouseLeave()
{
	m_bNCTracking = FALSE;
	m_bNCHovering = FALSE;

	if(m_iXButtonHovering != -1)
	{
		m_TitleBtn[m_iXButtonHovering].MouseLeave();
		m_iXButtonHovering = -1;
	}

	//CFrameWndEx::OnNcMouseLeave();
}

void CSkinFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED && nType != SIZE_MAXHIDE )
	{
		if (m_Rgn.GetSafeHandle())
		{
			m_Rgn.DeleteObject();
		}

		CRect rc;
		GetWindowRect(&rc); //获得窗口矩形
		rc -= rc.TopLeft();
		m_Rgn.CreateRoundRectRgn(rc.left, rc.top,   rc.right+1, rc.bottom+1, 5, 5); //根据窗口矩形创建一个圆角矩形
		SetWindowRgn(m_Rgn, TRUE); //根据圆角矩形指定的区域改变窗口的形状
	}

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.OffsetRect( -rcWnd.left, -rcWnd.top);

	CRect rMin(rcWnd.right - 74, 8, rcWnd.right-54, 30);
	m_TitleBtn[ID_MIN_BTN].SetRect(rMin);
	CRect rMax(rcWnd.right - 52, 8, rcWnd.right-32, 30);
	m_TitleBtn[ID_MAX_BTN].SetRect(rMax);
	CRect rClose(rcWnd.right - 30, 8, rcWnd.right - 10, 30);
	m_TitleBtn[ID_CLOSE_BTN].SetRect(rClose);

	if (nType == SIZE_MAXIMIZED||
		nType == SIZE_RESTORED)
	{
		OnNcPaint();
	}

}

void CSkinFrameWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID != SC_CLOSE)
	{
		OnNcPaint();
	}

	CFrameWndEx::OnSysCommand(nID, lParam);
}

LRESULT CSkinFrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 0x125)
	{
		OnNcPaint();
	}

	if (message ==0x00AE||//WM_NCUAHDRAWCAPTION
		message==0x00AF)//WM_NCUAHDRAWFRAME)
	{
		OnNcPaint();
		return 0;
	}

	return CFrameWndEx::WindowProc(message, wParam, lParam);
}

BOOL CSkinFrameWnd::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CFrameWndEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.OffsetRect( -rcWnd.left, -rcWnd.top);

	CRect rcControl;
	rcControl.left = rcWnd.right- 35;
	rcControl.top = 5;
	rcControl.right = rcControl.left + 20;
	rcControl.bottom = rcControl.top + 20;
	AddTitleButton(rcControl,m_WindowBtnImage[2],3,ID_CLOSE_BTN);

	AddTitleButton(rcControl,m_WindowBtnImage[1],3,ID_MAX_BTN);

	AddTitleButton(rcControl,m_WindowBtnImage[0],3,ID_MIN_BTN);


	return TRUE;
}

//往对话框添加一个按钮
CDUIButton* CSkinFrameWnd::AddTitleButton(LPRECT lpRect, Image *pImage, UINT nCount /*= 3*/ , UINT nID/* = 0*/,LPCTSTR lpszCaption/*=NULL*/)
{
	CDUIButton dcControl;
	dcControl.CreateButton(lpszCaption, lpRect, this,pImage,nCount, nID);
	m_TitleBtn[nID] = dcControl;

	return &m_TitleBtn[nID];
}

int CSkinFrameWnd::TButtonHitTest(CPoint point, CRect& rtButton)
{
	std::map<int,CDUIButton>::iterator iter;
	for (iter = m_TitleBtn.begin(); iter != m_TitleBtn.end(); iter++)
	{
		CDUIButton& dcControl = iter->second;
		if(dcControl.PtInButton(point))
		{
			dcControl.GetRect(&rtButton);
			return dcControl.GetControlId();
		}
	}
	return -1;
}