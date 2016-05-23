
// OpenCVBoostDlg.h : ͷ�ļ�
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

// COpenCVBoostDlg �Ի���
class COpenCVBoostDlg : public CDialogEx
{
// ����
public:
	COpenCVBoostDlg(CWnd* pParent = NULL);	// ��׼���캯��
    
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVBOOST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	Mat* srcImg1;//ͼƬ1
	Mat* srcImg2;//ͼƬ2
	Mat* dstImg;//��Ϻ��ͼƬ
	ThreadParam* tParam;
	void DrawImage(ImgData *imgData);//����
	static void Mix(ThreadParam *p);//���
	static void MatToCImage(ThreadParam *p);
	 
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpenfile1();
	afx_msg void OnBnClickedButtonOpenfile2();
	afx_msg void OnBnClickedButtonMix();
};
