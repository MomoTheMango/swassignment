
// GlimCodingAssignmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GlimCodingAssignment.h"
#include "GlimCodingAssignmentDlg.h"
#include "afxdialogex.h"
#include <random>
#include <thread>
#include <mutex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CGlimCodingAssignmentDlg 대화 상자



CGlimCodingAssignmentDlg::CGlimCodingAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMCODINGASSIGNMENT_DIALOG, pParent)
	, m_dotRad(10)
	, m_circleThickness(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlimCodingAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DOT_RAD, m_dotRad);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_THICKNESS, m_circleThickness);
}

BEGIN_MESSAGE_MAP(CGlimCodingAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_RESET, &CGlimCodingAssignmentDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CGlimCodingAssignmentDlg::OnBnClickedBtnRandom)
	ON_BN_CLICKED(IDC_BTN_RANDOM10, &CGlimCodingAssignmentDlg::OnBnClickedBtnRandom10)
END_MESSAGE_MAP()


// CGlimCodingAssignmentDlg 메시지 처리기

BOOL CGlimCodingAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(0, 0, 880, 480);

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 그리기 위한 하얀 영역 생성
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_image.Create(nWidth, nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();
	for (int y = 0; y < nHeight; y++) {
		memset(fm + y * nPitch, 255, nWidth);
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGlimCodingAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGlimCodingAssignmentDlg::OnPaint()
{

	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 이미지 그리기
		if (m_image) {
			m_image.Draw(dc, 0, 0);
		}
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGlimCodingAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 이미지 리셋
void CGlimCodingAssignmentDlg::ClearImage() {
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	for (int y = 0; y < nHeight; y++) {
		memset(fm + y * nPitch, 255, nWidth);
	}
}

// 마우스 좌클릭 함수
void CGlimCodingAssignmentDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	
	// 원이 세개 이하면 클릭 시 원 그리기
	if (m_circles.size() < 3) {

		CString radiusStr;
		GetDlgItem(IDC_EDIT_DOT_RAD)->GetWindowText(radiusStr);
		int m_dotRadius = _ttoi(radiusStr);

		Circle newCircle = { point, m_dotRadius, 0 };
		m_circles.push_back(newCircle);
		DrawAllCircles();

	}
	// 원이 세개 이상이라면 HitTest로 원 클릭 감지
	else {
		m_dragIndex = HitTest(point);
	}

}

// 마우스가 원 위에 있고 좌클릭 시에 마우스 따라 원 좌표 업데이트
void CGlimCodingAssignmentDlg::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_dragIndex != -1 && (nFlags & MK_LBUTTON)) {
		m_circles[m_dragIndex].center = point;
		DrawAllCircles();
	}
	
}

// 마우스가 실제로 원 위에 있는지 확인
int CGlimCodingAssignmentDlg::HitTest(CPoint point) {
	for (int i = 0; i < m_circles.size(); i++) {
		// 원 세개의 위치 전부 확인 및 현재 좌표에 있는지 확인
		if (IsInCircle(point.x, point.y, m_circles[i].center.x, m_circles[i].center.y, m_circles[i].radius)) {
			return i;
		}
	}
	return -1;
}

// 마우스 좌클릭 놓았을때 드래그 인덱스 리셋
void CGlimCodingAssignmentDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	m_dragIndex = -1;
	CDialogEx::OnLButtonUp(nFlags, point);
}

// 원을 그리는 함수
void CGlimCodingAssignmentDlg::DrawDot(unsigned char* fm, int x, int y, int nRadius, int nGray) {
	
	// 원 위치 
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	// 네모를 기준으로 동그라미 Radius보다 가까운 좌표는 그리기
	for (int j = max(y, 0); j < min(y + nRadius * 2, nHeight); j++) {
		for (int i = max(x, 0); i < min(x + nRadius * 2, nWidth); i++) {
			if (IsInCircle(i, j, nCenterX, nCenterY, nRadius)) {
				fm[j * nPitch + i] = nGray;
			}
		}
	}

	// 원 중심 좌표 표시
	for (int i = 0; i < m_circles.size(); i++) {

		CPoint dotCenter = m_circles[i].center;
		CString centerCoord;
		centerCoord.Format(_T("%d, %d"), dotCenter.x, dotCenter.y);

		if (i == 0) 
			GetDlgItem(IDC_STATIC_DOT1)->SetWindowTextW(centerCoord);
		else if (i == 1)
			GetDlgItem(IDC_STATIC_DOT2)->SetWindowTextW(centerCoord);
		else if (i == 2)
			GetDlgItem(IDC_STATIC_DOT3)->SetWindowTextW(centerCoord);
		
	}

}

// 좌표가 반지름 안쪽이면 True를 반환
bool CGlimCodingAssignmentDlg::IsInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius) {

	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;

}

// 디스플레이 업데이트
void CGlimCodingAssignmentDlg::UpdateDisplay(){
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

// 원 계산을 위한 함수
void CGlimCodingAssignmentDlg::CalcCircle() {
	if (m_circles.size() != 3) return;

	double x1 = m_circles[0].center.x, y1 = m_circles[0].center.y;
	double x2 = m_circles[1].center.x, y2 = m_circles[1].center.y;
	double x3 = m_circles[2].center.x, y3 = m_circles[2].center.y;

	double a = x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2);
	double b = (x1 * x1 + y1 * y1) * (y3 - y2) + (x2 * x2 + y2 * y2) * (y1 - y3) + (x3 * x3 + y3 * y3) * (y2 - y1);
	double c = (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * x2 + y2 * y2) * (x3 - x1) + (x3 * x3 + y3 * y3) * (x1 - x2);

	double x = -b / (2 * a);
	double y = -c / (2 * a);
	double r = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

	m_circleCenter = CPoint(static_cast<int>(x), static_cast<int>(y));
	m_circleRadius = static_cast<int>(r);

}

// 정원을 그리는 함수
void CGlimCodingAssignmentDlg::DrawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray) {

	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	// 정원의 두께 입력 받기
	CString sizeStr;
	GetDlgItem(IDC_EDIT_CIRCLE_THICKNESS)->GetWindowText(sizeStr);
	m_circleThickness = _ttoi(sizeStr);
	int nThickness = m_circleThickness;
	
	// 정원의 중심 좌표에서 거리 확인 후, 두께에 부합하게 그리기
	for (int j = max(y, 0); j < min(y + nRadius * 2, nHeight); j++) {
		for (int i = max(x, 0); i < min(x + nRadius * 2, nWidth); i++) {
			double distance = sqrt(pow(i - nCenterX, 2) + pow(j - nCenterY, 2));
			if (abs(distance - nRadius) < nThickness) {
				fm[j * nPitch + i] = nGray;
			}
		}
	}

}

// 원하고 정원 정보를 받아 그리는 두개의 함수를 호출
void CGlimCodingAssignmentDlg::DrawAllCircles() {

	ClearImage();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 원이 세개라면 정원 그리기
	if (m_circles.size() == 3) {
		CalcCircle();
		DrawCircle(fm, m_circleCenter.x - m_circleRadius, m_circleCenter.y - m_circleRadius, m_circleRadius, 0);
	}

	// 원 그리기
	for (const auto& circle : m_circles) {
		DrawDot(fm, circle.center.x - circle.radius, circle.center.y - circle.radius, circle.radius, 0);
	}

	UpdateDisplay();
}

// 리셋 버튼 함수
void CGlimCodingAssignmentDlg::OnBnClickedBtnReset()
{
	ClearImage();
	m_circles.clear();
	m_circleCenter = CPoint(0, 0);
	m_circleRadius = 0;
	GetDlgItem(IDC_STATIC_DOT1)->SetWindowTextW(_T("0, 0"));
	GetDlgItem(IDC_STATIC_DOT2)->SetWindowTextW(_T("0, 0"));
	GetDlgItem(IDC_STATIC_DOT3)->SetWindowTextW(_T("0, 0"));
	UpdateDisplay();
}

// 랜덤으로 원과 정원을 그리는 함수
void CGlimCodingAssignmentDlg::OnBnClickedBtnRandom()
{
	ClearImage();
	m_circles.clear();

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	
	// 원 반지름 입력 받기
	CString radiusStr;
	GetDlgItem(IDC_EDIT_DOT_RAD)->GetWindowText(radiusStr);
	int m_dotRadius = _ttoi(radiusStr);

	// 랜덤 원 생성
	srand(static_cast<unsigned>(time(nullptr)));
	for (int i = 0; i < 3; i++) {
		int x = rand() % nWidth-10;
		int y = rand() % nHeight-10;
		Circle newCircle = { CPoint(x, y), m_dotRadius, 0 };
		m_circles.push_back(newCircle);
	}

	DrawAllCircles();

}

// 랜덤을 10번 반복하는 함수
void CGlimCodingAssignmentDlg::OnBnClickedBtnRandom10()
{
	
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	// 쓰레드 실행
	std::thread(&CGlimCodingAssignmentDlg::ThreadFunction, this, nWidth, nHeight).detach();

}

// 실행할 별도 쓰레드 함수
void CGlimCodingAssignmentDlg::ThreadFunction(int nWidth, int nHeight) {

	int nTotalDraws = 10;

	// 원 반지름 입력 받기
	CString radiusStr;
	GetDlgItem(IDC_EDIT_DOT_RAD)->GetWindowText(radiusStr);
	int m_dotRadius = _ttoi(radiusStr);

	srand(static_cast<unsigned>(time(nullptr)));

	// 랜덤 10번 반복
	for (int i = 0; i < nTotalDraws; i++) {
		
		// 이미지 리셋시 데이터 접근 제한
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			ClearImage();
			m_circles.clear();
		}

		// 랜덤 원 생성
		auto newCircles = new std::vector<Circle>();

		for (int j = 0; j < 3; j++) {
			int x = rand() % nWidth-10;
			int y = rand() % nHeight-10;
			Circle newCircle = { CPoint(x, y), m_dotRadius, 0 };
			newCircles->push_back(newCircle);
		}

		// 랜덤 원을 m_circles에 넣을시 데이터 접근 제한
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_circles.insert(m_circles.end(), newCircles->begin(), newCircles->end());
			delete newCircles;
		}

		DrawAllCircles();

		// 초당 2번 업데이트
		Sleep(500);

	}


}