
// OpenCVBoostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCVBoost.h"
#include "OpenCVBoostDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenCVBoostDlg 对话框



COpenCVBoostDlg::COpenCVBoostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENCVBOOST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	srcImg1 = NULL;
	srcImg2 = NULL;
	dstImg = NULL;
	tParam = new ThreadParam[8];
}

void COpenCVBoostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCVBoostDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &COpenCVBoostDlg::OnNMCustomdrawSliderAlpha)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE1, &COpenCVBoostDlg::OnBnClickedButtonOpenfile1)
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE2, &COpenCVBoostDlg::OnBnClickedButtonOpenfile2)
	ON_BN_CLICKED(IDC_BUTTON_MIX, &COpenCVBoostDlg::OnBnClickedButtonMix)
END_MESSAGE_MAP()


// COpenCVBoostDlg 消息处理程序

BOOL COpenCVBoostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
	csc_Alpha->SetRange(0, 10, TRUE);
	csc_Alpha->SetPos(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenCVBoostDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenCVBoostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		ImgData imgData;
		imgData.srcImg1 = srcImg1;
		imgData.srcImg2 = srcImg2;
		imgData.dstImg = dstImg;
		DrawImage(&imgData);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenCVBoostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenCVBoostDlg::OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
	CString text;
	double alpha= csc_Alpha->GetPos()*0.1;
	text.Format(_T("%.1f"), alpha);
	GetDlgItem(IDC_STATIC_ALPHANUM)->SetWindowText(text);


	*pResult = 0;
}


void COpenCVBoostDlg::OnBnClickedButtonOpenfile1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR  szFilter[] = _T("JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||");
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CString imFilePath;
		VERIFY(imFilePath = fileOpenDlg.GetPathName());
		if (srcImg1 != NULL)
		{
			srcImg1->release();
			delete srcImg1;
		}
		srcImg1 = new Mat();
		*srcImg1 = imread(imFilePath.GetBuffer(0));

		if (dstImg != NULL)
		{
			dstImg->release();
			delete dstImg;
			dstImg = NULL;
		}

		this->Invalidate();
	}
}


void COpenCVBoostDlg::OnBnClickedButtonOpenfile2()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR  szFilter[] = _T("JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||");
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CString imFilePath;
		VERIFY(imFilePath = fileOpenDlg.GetPathName());
		if (srcImg2 != NULL)
		{
			srcImg2->release();
			delete srcImg2;
		}
		srcImg2 = new Mat();
		*srcImg2 = imread(imFilePath.GetBuffer(0));

		if (dstImg != NULL)
		{
			dstImg->release();
			delete dstImg;
			dstImg = NULL;
		}

		this->Invalidate();
	}
}

void COpenCVBoostDlg::DrawImage(ImgData *imgData)
{
	if (imgData->dstImg != NULL)
	{
		CImage* cImg = new CImage();
		cImg->Create(imgData->dstImg->cols, imgData->dstImg->rows, 8 * imgData->dstImg->channels());
		//创建boost多线程
		thread thr[8];
		int subLength = cImg->GetWidth() * cImg->GetHeight() / 8;
		for (int i = 0; i < 8; ++i)
		{
			tParam[i].startIndex = i * subLength;
			tParam[i].endIndex = (i != 7) ? (i + 1) * subLength - 1 : cImg->GetWidth() * cImg->GetHeight() - 1;
			tParam[i].mImg = dstImg;
			tParam[i].cImg = cImg;
			thr[i] = thread(bind(&COpenCVBoostDlg::MatToCImage, &tParam[i]));
		}
		//等待线程结束
		for (int i = 0;i < 8;++i)
		{
			thr[i].join();
		}

		CDC* DC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		CDC memDC;             // 用于缓冲绘图的内存画笔  
		CBitmap memBitmap;         // 用于缓冲绘图的内存画布
		memDC.CreateCompatibleDC(DC);  // 创建与原画笔兼容的画笔
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // 创建与原位图兼容的内存画布
		memDC.SelectObject(&memBitmap);      // 创建画笔与画布的关联
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// 将cImg的内容缩放画到内存画布中
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// 将已画好的画布复制到真正的缓冲区中
		DC->BitBlt(680, 50, cImg->GetWidth(), cImg->GetHeight(), &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();

		if (cImg != NULL)
		{
			cImg->Destroy();
			delete cImg;
		}
	}
	if (imgData->srcImg1 != NULL)
	{
		CImage* cImg = new CImage();
		cImg->Create(imgData->srcImg1->cols, imgData->srcImg1->rows, 8 * imgData->srcImg1->channels());

		thread thr[8];
		int subLength = cImg->GetWidth() * cImg->GetHeight() / 8;
		for (int i = 0; i < 8; ++i)
		{
			tParam[i].startIndex = i * subLength;
			tParam[i].endIndex = (i != 7) ? (i + 1) * subLength - 1 : cImg->GetWidth() * cImg->GetHeight() - 1;
			tParam[i].mImg = srcImg1;
			tParam[i].cImg = cImg;
			thr[i] = thread(bind(&COpenCVBoostDlg::MatToCImage, &tParam[i]));
		}

		for (int i = 0;i < 8;++i)
		{
			thr[i].join();
		}
		CDC* DC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		CDC memDC;             // 用于缓冲绘图的内存画笔  
		CBitmap memBitmap;         // 用于缓冲绘图的内存画布
		memDC.CreateCompatibleDC(DC);  // 创建与原画笔兼容的画笔
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // 创建与原位图兼容的内存画布
		memDC.SelectObject(&memBitmap);      // 创建画笔与画布的关联
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// 将cImg的内容缩放画到内存画布中
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// 将已画好的画布复制到真正的缓冲区中
		DC->BitBlt(20, 50, cImg->GetWidth(), cImg->GetHeight(), &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();

		if (cImg != NULL)
		{
			cImg->Destroy();
			delete cImg;
		}
	}
	if (imgData->srcImg2 != NULL)
	{
		CImage* cImg = new CImage();
		cImg->Create(imgData->srcImg2->cols, imgData->srcImg2->rows, 8 * imgData->srcImg2->channels());

		thread thr[8];
		int subLength = cImg->GetWidth() * cImg->GetHeight() / 8;
		for (int i = 0; i < 8; ++i)
		{
			tParam[i].startIndex = i * subLength;
			tParam[i].endIndex = (i != 7) ? (i + 1) * subLength - 1 : cImg->GetWidth() * cImg->GetHeight() - 1;
			tParam[i].mImg = srcImg2;
			tParam[i].cImg = cImg;
			thr[i] = thread(bind(&COpenCVBoostDlg::MatToCImage, &tParam[i]));
		}
		for (int i = 0;i < 8;++i)
		{
			thr[i].join();
		}
		CDC* DC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		CDC memDC;             // 用于缓冲绘图的内存画笔  
		CBitmap memBitmap;         // 用于缓冲绘图的内存画布
		memDC.CreateCompatibleDC(DC);  // 创建与原画笔兼容的画笔
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // 创建与原位图兼容的内存画布
		memDC.SelectObject(&memBitmap);      // 创建画笔与画布的关联
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// 将cImg的内容缩放画到内存画布中
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// 将已画好的画布复制到真正的缓冲区中
		DC->BitBlt(350, 50, cImg->GetWidth(), cImg->GetHeight(), &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();

		if (cImg != NULL)
		{
			cImg->Destroy();
			delete cImg;
		}
	}
}

void COpenCVBoostDlg::OnBnClickedButtonMix()
{
	// TODO: 在此添加控件通知处理程序代码
	if (srcImg1 != NULL&&srcImg2 != NULL)
	{
		//获取alpha
		CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
		double alpha = csc_Alpha->GetPos()*0.1;

		//创建目标图像
		if (dstImg != NULL)
		{
			dstImg->release();
			delete dstImg;
		}
		dstImg = new Mat(srcImg1->rows, srcImg1->cols, CV_8SC3, Scalar(0, 0, 0));

		
		//创建boost多线程
		thread thr[8];
		int subLength = dstImg->cols * dstImg->rows/ 8;
		for (int i = 0; i < 8; ++i)
		{
			tParam[i].startIndex = i * subLength;
			tParam[i].endIndex = (i != 7) ?(i + 1) * subLength - 1 : dstImg->cols * dstImg->rows - 1;
			tParam[i].srcImg1 = srcImg1;
			tParam[i].srcImg2 = srcImg2;
			tParam[i].dstImg = dstImg;
			tParam[i].alpha = alpha;
	
			thr[i] = thread(bind(&COpenCVBoostDlg::Mix, &tParam[i]));
		}
		//等待线程结束
		for (int i = 0;i < 8;++i)
		{
			thr[i].join();
		}
		this->Invalidate();
	}
}

void COpenCVBoostDlg::Mix(ThreadParam *p)
{
	int width = p->srcImg1->cols;
	int height = p->srcImg1->rows;
	double alpha = p->alpha;
	int startindex = p->startIndex;
	int endindex = p->endIndex;

		for (int i = startindex;i <= endindex;++i)
		{
			int x = i%width;
			int y = i / width;

			p->dstImg->at<Vec3b>(y, x)[2] = (double)(p->srcImg1->at<Vec3b>(y, x)[2] * alpha) + (double)(p->srcImg2->at<Vec3b>(y, x)[2] * (1 - alpha));
			p->dstImg->at<Vec3b>(y, x)[1] = (double)(p->srcImg1->at<Vec3b>(y, x)[1] * alpha) + (double)(p->srcImg2->at<Vec3b>(y, x)[1] * (1 - alpha));
			p->dstImg->at<Vec3b>(y, x)[0] = (double)(p->srcImg1->at<Vec3b>(y, x)[0] * alpha) + (double)(p->srcImg2->at<Vec3b>(y, x)[0] * (1 - alpha));
		}
}

void COpenCVBoostDlg::MatToCImage(ThreadParam *p)
{
	int width = p->cImg->GetWidth();
	int height = p->cImg->GetHeight();
	int startindex = p->startIndex;
	int endindex = p->endIndex;

	byte* pRealData = (byte*)p->cImg->GetBits();
	int pit = p->cImg->GetPitch();
	int bitCount = p->cImg->GetBPP() / 8;

	//Mat转CImage
	for (int i = startindex;i < endindex;++i)
	{
		int x = i%width;
		int y = i / width;
		*(pRealData + y*pit + x*bitCount + 2) = p->mImg->at<Vec3b>(y, x)[2];
		*(pRealData + y*pit + x*bitCount + 1) = p->mImg->at<Vec3b>(y, x)[1];
		*(pRealData + y*pit + x*bitCount) = p->mImg->at<Vec3b>(y, x)[0];
	}
}