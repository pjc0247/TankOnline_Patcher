#include <windows.h>
#include <wininet.h>

#define T(str) TEXT(str)
#define PROG_NAME ("Aurelia FTP")
#define UP_LEVEL T("..")

class FTP{
public:
	void SetParam(LPCTSTR Server,LPCTSTR User,LPCTSTR Pass);		//������ ���� ����
	BOOL Connect();													// ����
	void UnConnect();												// ���� ����

	BOOL DownLoad(LPCTSTR remote,LPCTSTR local);					// ���� �ٿ�ε�
	BOOL UpLoad(LPCTSTR remote,LPCTSTR local);						// ���ε�
	BOOL Cd(LPCTSTR cd);											// �۾� ���丮 ����
	BOOL CreateDirectory(LPCTSTR dn);								// ���丮 ����

	BOOL IsConnected();												// ������ִ��� �˾ƿ�
private:
	TCHAR p_Server[50]; 
	TCHAR p_User[20];  
	TCHAR p_Pass[20]; 

	HINTERNET hInternet, hFtp;	
};