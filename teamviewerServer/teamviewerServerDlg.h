
// teamviewerServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CteamviewerServerDlg �Ի���
class CteamviewerServerDlg : public CDialogEx
{
// ����
public:
	CteamviewerServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEAMVIEWERSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void CteamviewerServerDlg::MyinsertItem(CteamviewerServerDlg* pthis, TCHAR* str1, TCHAR* str2, TCHAR* str3);
	BOOL CteamviewerServerDlg::InitNet();

// ʵ��
protected:
	HICON m_hIcon;
	SOCKET s;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lst;
};
struct MySession
{
	SOCKET sClient;//��ʾ�ͻ�����socket
	sockaddr_in addr;
	DWORD dwLastTickout;//�ͻ���ͨ�ŵ�����Ծʱ��
};