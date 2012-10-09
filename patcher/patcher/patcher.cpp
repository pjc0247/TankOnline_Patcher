// patcher.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <Windows.h>

#include <SDL.h>
#include <sdl_gdiplus.h>
#include <sdl_ttf.h>

#include <SDL_shape.h>

#include "drawtext.h"
#include "sprite.h"

#include "../../module_Update/updater.h"

#pragma comment (lib,"sdl")
#pragma comment (lib,"sdl_gdiplus")
#pragma comment (lib,"sdl_ttf")

#define UPDATE_URL TEXT("http://anz.smilent.net/tank_online/patch/patchlist")

void Abort(LPCTSTR msg);

SDL_Window *window;
SDL_Renderer *renderer;

UPDATER updater;

int current_version;

int total_recv;
int current_recv;
int current_index;
int max_index;

BOOL UpdateCallback(int current,int total,int index){
	total_recv = total;
	current_recv = current;
	current_index = index;

	return true;
}
DWORD WINAPI UpdateThread(LPVOID arg){
	max_index = updater.GetDownloadListMax();
	bool ret = updater.Update(UpdateCallback);
	if(ret == false){
		Abort(TEXT("업데이트 실패"));
		return -1;
	}
	current_index = max_index;

	FILE *fp = fopen("cv","w");
	fprintf(fp,"%d", updater.GetNewestVersion());
	fclose(fp);	
	return 0;
}

void ScenePatch(){
	bool quit = false;
	SDL_Event event;

	Sprite *bgi = new Sprite("resource\\bgi_patcher.jpg",1,1);
	Sprite *title = new Sprite("resource\\title.png",1,1);

	FONT gulim;

	float progress_total = 50;
	float progress_current = 30;

	gulim.InitFont("c:\\windows\\fonts\\gulim.ttc",12);
	gulim.SetColor(255,255,255);
	gulim.SetStyle(TTF_STYLE_BOLD);

	while(!quit){
		while(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		{
			progress_total = ((float)current_index / (float)max_index) * 100;
			progress_current = ((float)current_recv / (float)total_recv) * 100;

			progress_total += (1.0f / (float)max_index) * progress_current;
		}
		{
			SDL_RenderClear(renderer);

			bgi->Draw(0,0,480,272);
			title->Draw(20,20,440,170);


			SDL_Rect rtProgressTotalFrame = {20,200,440,25};
			SDL_Rect rtProgressCurrentFrame = {20,235,440,25};
			SDL_Rect rtProgressTotal = {25,205,progress_total * 4.4,15};
			SDL_Rect rtProgressCurrent = {25,240,progress_current * 4.4,15};

			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(renderer,64,64,64,255);
			SDL_RenderFillRect(renderer,&rtProgressTotalFrame);
			SDL_RenderFillRect(renderer,&rtProgressCurrentFrame);

			SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer,64,255,64,196);
			SDL_RenderFillRect(renderer,&rtProgressTotal);
			SDL_RenderFillRect(renderer,&rtProgressCurrent);

			gulim.Draw(renderer,"전체 진행률",25,190);
			gulim.Draw(renderer,"현재 진행률",25,225);

			char str[256];
			gulim.Draw(renderer,"Tank Online Patcher",343,258);
			sprintf(str,"ver.%d.%d", current_version/1000,current_version%1000);
			gulim.Draw(renderer,str,430,245);

			SDL_RenderPresent(renderer);
		}

		SDL_Delay(1);
	}

	delete bgi;
}

void SetupRC(){
	SDL_Init(SDL_INIT_VIDEO);
		
	window = SDL_CreateShapedWindow(
						"Patcher",
						SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
						480,272,
						SDL_WINDOW_SHOWN);
	/*window = SDL_CreateWindow(
						"Patcher",
						SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
						480,272,
						SDL_WINDOW_SHOWN);*/
	renderer = SDL_CreateRenderer(
						window,
						-1,
						0);

	SDL_Surface *frame;
	SDL_WindowShapeMode mode;
	SDL_Color black = {0,0,0,255};

	frame = SDL_LoadBMP("resource\\frame.bmp");
	mode.mode = ShapeModeColorKey;
	mode.parameters.colorKey = black;
	SDL_SetWindowShape(window,frame,&mode);

	TTF_Init();
}
void Quit(){
	TTF_Quit();
	SDL_Quit();
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	Mutex를 이용한 중복실행방지
	*/
	HANDLE hMutex;  
	hMutex = CreateMutex(NULL,FALSE,L"ToPatcher");  
	if(GetLastError() == ERROR_ALREADY_EXISTS){  
		Abort(TEXT("프로그램이 이미 실행중입니다."));
		return 0;  
	}  

	/*
	그래픽의 초기화
	*/
	SetupRC();

	/*
	현재 버전 정보 가져오기
	*/
	FILE*fp = fopen("cv","r");
	if(fp == NULL){
		Abort(TEXT("버전 정보를 읽을 수 없습니다."));
		return 0;  
	}
	fscanf(fp,"%d", &current_version);
	fclose(fp);
	updater.SetParam(UPDATE_URL,
				current_version);

	if(!updater.Check()){
		Abort(TEXT("업데이트 서버에 접속할 수 없습니다."));
		return 0;  
	}

	/*
	다운로드 스레드 생성
	*/
	HANDLE hThread;
	hThread = CreateThread(NULL,
							NULL,
							UpdateThread,
							NULL,NULL,NULL);

	ScenePatch();
		
	Quit();
	CloseHandle(hMutex);
	return 0;
}



void Abort(LPCTSTR msg){
	MessageBox(NULL,
				msg,
				L"Error",
				MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}