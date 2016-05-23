
// OpenCVBoostDlg.h : 头文件
//

#pragma once


struct ImgData
{
	Mat* srcImg1;
	Mat* srcImg2;
	Mat* dstImg;
};

struct ThreadParam
{
	Mat* srcImg1;
	Mat* srcImg2;
	Mat* dstImg;
	Mat* mImg;
	CImage* cImg;
	int startIndex;
	int endIndex;
	double alpha;
};

// COpenCVBoostDlg 对话框
class COpenCVBoostDlg : public CDialogEx
{
// 构造
public:
	COpenCVBoostDlg(CWnd* pParent = NULL);	// 标准构造函数
    
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVBOOST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	Mat* srcImg1;//图片1
	Mat* srcImg2;//图片2
	Mat* dstImg;//混合后的图片
	ThreadParam* tParam;
	void DrawImage(ImgData *imgData);//绘制
	static void Mix(ThreadParam *p);//混合
	static void MatToCImage(ThreadParam *p);
	 
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpenfile1();
	afx_msg void OnBnClickedButtonOpenfile2();
	afx_msg void OnBnClickedButtonMix();
};
