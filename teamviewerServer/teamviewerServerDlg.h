
// teamviewerServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CteamviewerServerDlg 对话框
class CteamviewerServerDlg : public CDialogEx
{
// 构造
public:
	CteamviewerServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEAMVIEWERSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void CteamviewerServerDlg::MyinsertItem(CteamviewerServerDlg* pthis, TCHAR* str1, TCHAR* str2, TCHAR* str3);
	BOOL CteamviewerServerDlg::InitNet();

// 实现
protected:
	HICON m_hIcon;
	SOCKET s;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lst;
};
struct MySession
{
	SOCKET sClient;//表示客户机的socket
	sockaddr_in addr;
	DWORD dwLastTickout;//客户机通信的最后活跃时间
};