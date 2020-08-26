
// teamviewerServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "teamviewerServer.h"
#include "teamviewerServerDlg.h"
#include "afxdialogex.h"
#include "CALL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CteamviewerServerDlg 对话框

void CteamviewerServerDlg::MyinsertItem(CteamviewerServerDlg* pthis, TCHAR* str1, TCHAR* str2, TCHAR* str3)
{
	pthis->m_lst.InsertItem(pthis->m_lst.GetItemCount(), str1);
	pthis->m_lst.SetItemText(pthis->m_lst.GetItemCount() - 1, 1, str2);
	pthis->m_lst.SetItemText(pthis->m_lst.GetItemCount() - 1, 2, str3);
}

BOOL CteamviewerServerDlg::InitNet()
{
	//以下注释的部分是网络支持部分，mfc自带，因此不需要
	////windows很特殊，需要单独调用api来进行网络的初始化和反初始化
	//WORD wVersionRequested;
	//WSADATA wsaData;
	//int err;
	//wVersionRequested = MAKEWORD(2, 2);
	////初始化操作
	//err = WSAStartup(wVersionRequested, &wsaData);
	//if (err != 0)
	//{
	//	MessageBox( L"网络初始化失败！");
	//	return FALSE;
	//}
	s = socket(
		AF_INET,//INET协议簇
		SOCK_STREAM,//表示TCP协议   SOCK_DGRAM表示udp
		0
		);
	if (s == INVALID_SOCKET)
	{
		MessageBox(L"套接字创建失败！");
		return FALSE;
	}
	//netsata -an打印被占用的端口
	//端口 3389：RDP 80：HTTP端口，端口用于区分哪个应用的数据，只是个编号，范围是0-65535（因为在封包中占两个字节）
	//一个应用程序可使用多少个端口？
	//看见有多少个tcp连接
	//源ip：源端口   ----------------  目标ip：目标端口
	//sockaddr addr;//用于描述ip和端口的结构体，但不好用
	sockaddr_in addr;//用与替换sockaddr结构体，两者一致，可以混着用，但这个更好用。
	addr.sin_family = AF_INET;//写死
	addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");//固定套路，写0.0.0.0表示当前所有网卡的地址都可以接受外界的连接,如果写死一个ip就只可以接受该ip的地址。想要连外网,局域网最好写0.0.0.0
	addr.sin_port = htons(10087);//有个坑。网络字节序（大端）和本地字节序（小端）。比如说端口由两个字节组成，网络的一般是大端排序，所以要htons()函数：host to network short(short表示2个字节，long表示4个字节)
	int nLength = sizeof(sockaddr_in);
	int nRet = bind(//挑选一个本机其他软件没有使用的端口来绑定,固定的端口不能使用
		s,//套接字
		(sockaddr*)&addr,//用于描述ip和端口的结构体
		nLength
		);
	if (SOCKET_ERROR == nRet)
	{
		MessageBox( L"服务器连接套接字失败！");
		return FALSE;
	}
	//一个端口等别人来连接，要是很多人来就让他排队
	//监听
	nRet = listen(s,
		5);//同一瞬间来5个，排队容量
	if (SOCKET_ERROR == nRet)
	{
		MessageBox(L"服务器监听失败！");
		return FALSE;
	}
	//绑定只是在软件里面绑定起来了，listen才是真正在操作系统开了个端口，所以会弹防火墙允许访问的请求对话框。

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


// CteamviewerServerDlg 消息处理程序

void acceptThread(CteamviewerServerDlg* pthis)
{
	pthis->InitNet();
}



BOOL CteamviewerServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	m_lst.InsertColumn(0, _T("ip:port"));
	m_lst.InsertColumn(1, _T("system"));
	m_lst.InsertColumn(2, _T("位置"));//通过ip反查地址
	m_lst.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_lst.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_lst.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	//LVS_EX_FULLROWSELECT整行显示
	m_lst.SetExtendedStyle(m_lst.GetExtendedStyle() | LVS_EX_FULLROWSELECT| LVS_EX_HEADERINALLVIEWS| LVS_EX_GRIDLINES);
	//MyinsertItem(this,_T("192.168.3.31:65537"), _T("windows7"), _T("佛山昆明"));
	HANDLE hThread=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)acceptThread,this , 0, 0);//传入this


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CteamviewerServerDlg::OnPaint()
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
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CteamviewerServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

