
// teamviewerServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "teamviewerServer.h"
#include "teamviewerServerDlg.h"
#include "afxdialogex.h"
#include "CALL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CteamviewerServerDlg �Ի���

void CteamviewerServerDlg::MyinsertItem(CteamviewerServerDlg* pthis, TCHAR* str1, TCHAR* str2, TCHAR* str3)
{
	pthis->m_lst.InsertItem(pthis->m_lst.GetItemCount(), str1);
	pthis->m_lst.SetItemText(pthis->m_lst.GetItemCount() - 1, 1, str2);
	pthis->m_lst.SetItemText(pthis->m_lst.GetItemCount() - 1, 2, str3);
}

BOOL CteamviewerServerDlg::InitNet()
{
	//����ע�͵Ĳ���������֧�ֲ��֣�mfc�Դ�����˲���Ҫ
	////windows�����⣬��Ҫ��������api����������ĳ�ʼ���ͷ���ʼ��
	//WORD wVersionRequested;
	//WSADATA wsaData;
	//int err;
	//wVersionRequested = MAKEWORD(2, 2);
	////��ʼ������
	//err = WSAStartup(wVersionRequested, &wsaData);
	//if (err != 0)
	//{
	//	MessageBox( L"�����ʼ��ʧ�ܣ�");
	//	return FALSE;
	//}
	s = socket(
		AF_INET,//INETЭ���
		SOCK_STREAM,//��ʾTCPЭ��   SOCK_DGRAM��ʾudp
		0
		);
	if (s == INVALID_SOCKET)
	{
		MessageBox(L"�׽��ִ���ʧ�ܣ�");
		return FALSE;
	}
	//netsata -an��ӡ��ռ�õĶ˿�
	//�˿� 3389��RDP 80��HTTP�˿ڣ��˿����������ĸ�Ӧ�õ����ݣ�ֻ�Ǹ���ţ���Χ��0-65535����Ϊ�ڷ����ռ�����ֽڣ�
	//һ��Ӧ�ó����ʹ�ö��ٸ��˿ڣ�
	//�����ж��ٸ�tcp����
	//Դip��Դ�˿�   ----------------  Ŀ��ip��Ŀ��˿�
	//sockaddr addr;//��������ip�Ͷ˿ڵĽṹ�壬��������
	sockaddr_in addr;//�����滻sockaddr�ṹ�壬����һ�£����Ի����ã�����������á�
	addr.sin_family = AF_INET;//д��
	addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");//�̶���·��д0.0.0.0��ʾ��ǰ���������ĵ�ַ�����Խ�����������,���д��һ��ip��ֻ���Խ��ܸ�ip�ĵ�ַ����Ҫ������,���������д0.0.0.0
	addr.sin_port = htons(10087);//�и��ӡ������ֽ��򣨴�ˣ��ͱ����ֽ���С�ˣ�������˵�˿��������ֽ���ɣ������һ���Ǵ����������Ҫhtons()������host to network short(short��ʾ2���ֽڣ�long��ʾ4���ֽ�)
	int nLength = sizeof(sockaddr_in);
	int nRet = bind(//��ѡһ�������������û��ʹ�õĶ˿�����,�̶��Ķ˿ڲ���ʹ��
		s,//�׽���
		(sockaddr*)&addr,//��������ip�Ͷ˿ڵĽṹ��
		nLength
		);
	if (SOCKET_ERROR == nRet)
	{
		MessageBox( L"�����������׽���ʧ�ܣ�");
		return FALSE;
	}
	//һ���˿ڵȱ��������ӣ�Ҫ�Ǻܶ������������Ŷ�
	//����
	nRet = listen(s,
		5);//ͬһ˲����5�����Ŷ�����
	if (SOCKET_ERROR == nRet)
	{
		MessageBox(L"����������ʧ�ܣ�");
		return FALSE;
	}
	//��ֻ�����������������ˣ�listen���������ڲ���ϵͳ���˸��˿ڣ����Իᵯ����ǽ������ʵ�����Ի���

	return TRUE;
}


CteamviewerServerDlg::CteamviewerServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEAMVIEWERSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CteamviewerServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lst);
}

BEGIN_MESSAGE_MAP(CteamviewerServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CteamviewerServerDlg ��Ϣ�������

void acceptThread(CteamviewerServerDlg* pthis)
{
	pthis->InitNet();
}



BOOL CteamviewerServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_lst.InsertColumn(0, _T("ip:port"));
	m_lst.InsertColumn(1, _T("system"));
	m_lst.InsertColumn(2, _T("λ��"));//ͨ��ip�����ַ
	m_lst.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_lst.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_lst.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	//LVS_EX_FULLROWSELECT������ʾ
	m_lst.SetExtendedStyle(m_lst.GetExtendedStyle() | LVS_EX_FULLROWSELECT| LVS_EX_HEADERINALLVIEWS| LVS_EX_GRIDLINES);
	//MyinsertItem(this,_T("192.168.3.31:65537"), _T("windows7"), _T("��ɽ����"));
	HANDLE hThread=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acceptThread,this , 0, 0);//����this


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CteamviewerServerDlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CteamviewerServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

