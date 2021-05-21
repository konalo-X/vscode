#include"wintext.h"

CTestApp app;
BOOL CTestApp::InitInstance()
{
    m_pMainWnd=new CMainWNd();
    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();
    return true;
}



CMianWnd::CMainWnd()
{
    Create(NULL,_T("TestApp"));
}
