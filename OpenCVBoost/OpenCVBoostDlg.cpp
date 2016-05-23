
// OpenCVBoostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenCVBoost.h"
#include "OpenCVBoostDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// COpenCVBoostDlg �Ի���



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


// COpenCVBoostDlg ��Ϣ�������

BOOL COpenCVBoostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
	csc_Alpha->SetRange(0, 10, TRUE);
	csc_Alpha->SetPos(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenCVBoostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenCVBoostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenCVBoostDlg::OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
	CString text;
	double alpha= csc_Alpha->GetPos()*0.1;
	text.Format(_T("%.1f"), alpha);
	GetDlgItem(IDC_STATIC_ALPHANUM)->SetWindowText(text);


	*pResult = 0;
}


void COpenCVBoostDlg::OnBnClickedButtonOpenfile1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		//����boost���߳�
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
		//�ȴ��߳̽���
		for (int i = 0;i < 8;++i)
		{
			thr[i].join();
		}

		CDC* DC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		CDC memDC;             // ���ڻ����ͼ���ڴ滭��  
		CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
		memDC.CreateCompatibleDC(DC);  // ������ԭ���ʼ��ݵĻ���
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // ������ԭλͼ���ݵ��ڴ滭��
		memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// ��cImg���������Ż����ڴ滭����
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// ���ѻ��õĻ������Ƶ������Ļ�������
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
		CDC memDC;             // ���ڻ����ͼ���ڴ滭��  
		CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
		memDC.CreateCompatibleDC(DC);  // ������ԭ���ʼ��ݵĻ���
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // ������ԭλͼ���ݵ��ڴ滭��
		memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// ��cImg���������Ż����ڴ滭����
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// ���ѻ��õĻ������Ƶ������Ļ�������
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
		CDC memDC;             // ���ڻ����ͼ���ڴ滭��  
		CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
		memDC.CreateCompatibleDC(DC);  // ������ԭ���ʼ��ݵĻ���
		memBitmap.CreateCompatibleBitmap(DC, cImg->GetWidth(), cImg->GetHeight());  // ������ԭλͼ���ݵ��ڴ滭��
		memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
		memDC.FillSolidRect(rect, DC->GetBkColor());

		// ��cImg���������Ż����ڴ滭����
		cImg->StretchBlt(memDC.m_hDC, 0, 0, cImg->GetWidth(), cImg->GetHeight());

		// ���ѻ��õĻ������Ƶ������Ļ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (srcImg1 != NULL&&srcImg2 != NULL)
	{
		//��ȡalpha
		CSliderCtrl* csc_Alpha = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
		double alpha = csc_Alpha->GetPos()*0.1;

		//����Ŀ��ͼ��
		if (dstImg != NULL)
		{
			dstImg->release();
			delete dstImg;
		}
		dstImg = new Mat(srcImg1->rows, srcImg1->cols, CV_8SC3, Scalar(0, 0, 0));

		
		//����boost���߳�
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
		//�ȴ��߳̽���
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

	//MatתCImage
	for (int i = startindex;i < endindex;++i)
	{
		int x = i%width;
		int y = i / width;
		*(pRealData + y*pit + x*bitCount + 2) = p->mImg->at<Vec3b>(y, x)[2];
		*(pRealData + y*pit + x*bitCount + 1) = p->mImg->at<Vec3b>(y, x)[1];
		*(pRealData + y*pit + x*bitCount) = p->mImg->at<Vec3b>(y, x)[0];
	}
}