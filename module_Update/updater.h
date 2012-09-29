#include <windows.h>
#include <urlmon.h>
#include <stdio.h>

struct DownloadList{
	char URL[100];									// ������ �ٿ�ε��� url
	char Local[100];								// ������ ������ ���
	int Version;									// �� ������ ���� 
};

class UPDATER{
public:
	
	void SetParam(LPCTSTR url,int Ver);				// ������Ʈ ������ �����Ѵ�

	BOOL Check();									// ������Ʈ�� üũ�Ѵ�

	int GetNewestVersion();							// �ֽ� ������ ���ؿ´� 
	DownloadList *GetDownloadList();				// ������Ʈ ���� �׸���� ������ִ� �迭�� �����͸� ���´�
	int GetDownloadListMax();						// ������Ʈ ���� �׸��� ������ �˾ƿ´�

	BOOL Update(BOOL (*CallBack)(int, int, int));	// ������Ʈ�Ѵ�
	
private:
	TCHAR URL[100];									// ������Ʈ ������ �޾ƿ� �ּ�
	int Version;									// ���� ���α׷��� ����
	int NewVersion;									// �ֽ� ����
	DownloadList *d_list;						
	int d_list_max;
};