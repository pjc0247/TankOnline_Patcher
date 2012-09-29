
#include <urlmon.h>
#include <stdio.h>
#include "updater.h"

#include "ibindcallback.h"

#include <string>
#include <map>
using namespace std;

#pragma comment (lib,"urlmon.lib")

#define T(str) TEXT(str)

map<string,bool> downloaded;

void UPDATER::SetParam(LPCTSTR url,int Ver){
	Version = Ver;
	wsprintf(URL,(url));
}

BOOL UPDATER::Check(){
	URLDownloadToFile(NULL,URL,T("tmp.txt"),NULL,NULL);

	FILE *in = fopen("tmp.txt", "r");
	if(in == NULL) return false;
	fscanf(in,"%d\n%d\n", &NewVersion,&d_list_max);

	d_list = (DownloadList*)malloc(sizeof(DownloadList)*d_list_max);

	int v,p = 0;
	for(int i = 0;i< d_list_max;i++){
		fscanf(in,"%d\n", &v);		
		if(Version >= v) continue; // ���� �������� ���ų� ���� ������ �ٿ�ε� ��Ͽ��� ����
		d_list[p].Version = v;
		fscanf(in,"%s\n%s\n", d_list[p].URL,d_list[p].Local);
		//fgets(d_list[p].URL,100,in);
		//fgets(d_list[p].Local,100,in);

		map<string,bool>::iterator itor;
		bool find = false;

		for(itor=downloaded.begin();itor!=downloaded.end();++itor){
			if(itor->first == string(d_list[p].URL)){
				find = true;
				break;
			}
		}
		if(find == true){
			p--;
			continue;
		}
		downloaded[string(d_list[p].URL)] = true;

		p++;
	}

	d_list_max = p;
	fclose(in);
	remove("tmp.txt");
	return true;
}
int UPDATER::GetNewestVersion(){
	return NewVersion;
}
DownloadList *UPDATER::GetDownloadList(){
	return d_list;
}
int UPDATER::GetDownloadListMax(){
	return d_list_max;
}
BOOL UPDATER::Update(BOOL (*CallBack)(int now, int total, int index)){

	CBindTransferStatus icb;
	icb.RegistCallBack(CallBack);
	

	for(int i = 0;i<d_list_max;i++){
		icb.SetIndex(i);
		int ret = URLDownloadToFileA(NULL,d_list[i].URL,d_list[i].Local,0,&icb);
		if(ret != 0){
			printf("err at %d\n", i);
			printf(".. %s, %s\n", d_list[i].URL,d_list[i].Local);
			return false;
		}
	}

	return true;
}