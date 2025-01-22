
// GlimCodingAssignmentDlg.h: 헤더 파일
//

#pragma once
#include <iostream>
#include <vector>
#include <mutex>

struct Circle {
	CPoint center;
	int radius;
	int gray;
};


// CGlimCodingAssignmentDlg 대화 상자
class CGlimCodingAssignmentDlg : public CDialogEx
{
private:
	CImage m_image;
	int m_dragindex = -1;
	std::vector<Circle> m_circles;
	CPoint m_circleCenter;
	int m_circleRadius;
	std::mutex m_mutex;
	int HitTest(CPoint point);
	int m_dragIndex = -1;


// 생성입니다.
public:
	CGlimCodingAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	void ClearImage();
	void DrawDot(unsigned char* fm, int x, int y, int nRadius, int nGray);
	bool IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	void DrawAllCircles();
	void UpdateDisplay();
	void CalcCircle();
	void DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	void ThreadFunction(int nWidth, int nHeight);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMCODINGASSIGNMENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	int m_dotRad;
	int m_circleThickness;
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRandom();
	afx_msg void OnBnClickedBtnRandom10();
};

