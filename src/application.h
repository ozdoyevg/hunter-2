
#pragma once

#include <Windows.h>
#include <string>

#include "dxsdk.h"

#include "point.h"
#include "device.h"
#include "camera.h"
#include "font.h"
#include "object.h"
#include "sound.h"
#include "animation.h"
#include "input.h"

//const GUID GUID_NULL;

class CApplication
{
private:
	std::string wndClass;
	HWND hWnd;

	CPoint windowSize;
	CPoint windowHalfSize;

	bool foolScreen;

	Vector2 cameraRotation;

	bool haveBullets;

	float shootTimer;
	bool needReload;
	float timer2;
	float timer3;

	bool showMenu;

	DWORD frameCnt;
	float timeElapsed;
	float timeDelta;
	bool showFps;
	char textTemp[50];
	char textFps[50];

	int timer;
	int ducks;
	int score;
	int bullets;
	int magazine;

	D3DLIGHT9 light1;

	CDevice * device;
	CCamera * camera;
	CInput * input;

	CObject objSky;
	CObject objMount;
	CObject objGround;
	CObject objGrass1;
	CObject objGrass2;

	CSkinnedMesh smFire;
	CSkinnedMesh smHandgun;
	CSkinnedMesh smDucks[3];

	CFont fntFPS;
	CFont fntBullets;
	CFont fntTime;
	CFont fntDucks;
	CFont fntScore;
	CFont fntScore2;

	CTexture txtMenu;
	CTexture txtEnd;

	CSoundManager soundManager;
	CSound* wavMusic;
	CSound* wavReset;
	CSound* wavCharge;
	CSound* wavFire;
public:
	CApplication();
	~CApplication();

	void SetParams();
	void InitWindow(HINSTANCE hInstance);
	void Initialize(HINSTANCE hInstance);
	void LoadScene();

	void Process();
	void Frame();
	void RenderObjects();
	void RenderUI();
	void UpdateScene();
	void KeyPress();
	void UpdateDucksPosition();
private:
	static long __stdcall WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};