
#include "application.h"
#include "common.h"

CApplication::CApplication()
{
	device = NULL;
	camera = NULL;
	input = NULL;
}

CApplication::~CApplication()
{
	SAFE_DELETE(input);
	SAFE_DELETE(camera);
	SAFE_DELETE(device);

	SAFE_DELETE(wavMusic);
	SAFE_DELETE(wavReset);
	SAFE_DELETE(wavCharge);
	SAFE_DELETE(wavFire);
}

void CApplication::SetParams()
{
	foolScreen = false;
	wndClass = "HunterGame";
	windowSize = CPoint(1200, 800);
	windowHalfSize = CPoint(windowSize / 2);

	cameraRotation = 0;

	showFps = false;
	showMenu = true;

	bullets = 7;
	magazine = 70;
	ducks = 0;
	score = 0;
	timer = 0;

	haveBullets = true;
}

void CApplication::InitWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, 0);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = wndClass.c_str();
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	hWnd = CreateWindow(wndClass.c_str(), "Hunter 2", WS_OVERLAPPEDWINDOW, 0, 0, windowSize.x, windowSize.y, 0, 0, hInstance, 0);

	if (foolScreen)
	{
		RECT rect;
		HWND hDesktop = GetDesktopWindow();

		GetClientRect(hDesktop, &rect);

		windowSize = CPoint(rect.right, rect.bottom);
		windowHalfSize = CPoint(windowSize / 2);
	}
	else {
		// Меняем размер окна в соответствии с бордерами
		RECT rcClient, rcWind;
		POINT ptDiff;
		GetClientRect(hWnd, &rcClient);
		GetWindowRect(hWnd, &rcWind);
		ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
		ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
		MoveWindow(hWnd, rcWind.left, rcWind.top, windowSize.x + ptDiff.x, windowSize.y + ptDiff.y, TRUE);
	}

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

void CApplication::Initialize(HINSTANCE hInstance)
{
	SetParams();
	InitWindow(hInstance);

	device = new CDevice();
	camera = new CCamera();
	input = new CInput();

	device->Initialize(hWnd, windowSize.x, windowSize.y, foolScreen);

	D3DVECTOR cameraPosition = { 0.0, 1.5, 0.0 };
	D3DVECTOR cameraDirection = { 0.0, 1.5, -2.0 };
	camera->Initialize(cameraPosition, cameraDirection);

	input->Initialize(hWnd);
	soundManager.Initialize(hWnd, DSSCL_PRIORITY);
	soundManager.SetPrimaryBufferFormat(2, 22050, 16);
}

void CApplication::LoadScene(void)
{
	ZeroMemory(&light1, sizeof(light1));

	light1.Type = D3DLIGHT_POINT;

	light1.Diffuse.r = 180.0f;
	light1.Diffuse.g = 180.0f;
	light1.Diffuse.b = 180.0f;

	light1.Ambient.r = 30.0f;
	light1.Ambient.g = 30.0f;
	light1.Ambient.b = 30.0f;

	light1.Position = D3DXVECTOR3(3.0f, 10.0f, 2.0f);

	light1.Range = 20;
	light1.Attenuation0 = 0.1f;
	light1.Attenuation1 = 0.3f;
	light1.Attenuation2 = 2.0f;

	light1.Specular.r = 250;
	light1.Specular.g = 250;
	light1.Specular.b = 250;

	device->d3dDevice->SetLight(0, &light1);

	objSky.LoadMesh(device->d3dDevice, "../media/models/sky.x");
	objMount.LoadMesh(device->d3dDevice, "../media/models/mount.x");
	objGround.LoadMesh(device->d3dDevice, "../media/models/ground.x");
	objGrass1.LoadMesh(device->d3dDevice, "../media/models/grass.x");
	objGrass2.LoadMesh(device->d3dDevice, "../media/models/grass2.x");

	smHandgun.InitSkinnedMesh(device->d3dDevice, "../media/models/handgun.x");
	smHandgun.SetAnimation(2);
	smFire.InitSkinnedMesh(device->d3dDevice, "../media/models/fire.x");

	objSky.position.z = 1.0f;
	objMount.position.z = 1.0f;
	objGround.position.z = 1.0f;
	objGrass1.position.z = 1.0f;
	objGrass2.position.z = 1.0f;
	smHandgun.position.y = 1.5f;
	smFire.position.y = 1.5f;

	for (int i = 0; i < 3; i++)
	{
		smDucks[i].InitSkinnedMesh(device->d3dDevice, "../media/models/duck.x");
	}

	fntFPS.Size = 9;
	fntFPS.Weight = 500;
	fntFPS.InitFont(device->d3dDevice);

	fntBullets.Size = 17;
	fntBullets.Weight = 800;
	fntBullets.InitFont(device->d3dDevice);

	fntTime.Size = 17;
	fntTime.Weight = 800;
	fntTime.InitFont(device->d3dDevice);

	fntDucks.Size = 17;
	fntDucks.Weight = 800;
	fntDucks.InitFont(device->d3dDevice);

	fntScore.Size = 17;
	fntScore.Weight = 800;
	fntScore.InitFont(device->d3dDevice);

	fntScore2.Size = 17;
	fntScore2.Weight = 100;
	fntScore2.InitFont(device->d3dDevice);

	txtEnd.position = Vector2((float)windowHalfSize.x, (float)windowHalfSize.y) - 256;
	txtEnd.size = 512;
	txtEnd.LoadTexture(device->d3dDevice, "../media/textures/panel.dds");
	txtEnd.InitTexture(device->d3dDevice);

	txtMenu.position = Vector2((float)windowHalfSize.x, (float)windowHalfSize.y) - 256;
	txtMenu.size = 512;
	txtMenu.LoadTexture(device->d3dDevice, "../media/textures/panel2.dds");
	txtMenu.InitTexture(device->d3dDevice);

	soundManager.Create(&wavMusic, "../media/sounds/music.wav", DSCREATION_FLAGS, GUID_NULL, 1);
	soundManager.Create(&wavReset, "../media/sounds/reset.wav", DSCREATION_FLAGS, GUID_NULL, 1);
	soundManager.Create(&wavCharge, "../media/sounds/charge.wav", DSCREATION_FLAGS, GUID_NULL, 1);
	soundManager.Create(&wavFire, "../media/sounds/fire.wav", DSCREATION_FLAGS, GUID_NULL, 1);

	wavMusic->Play(0, 1, -2000, 44100, 0);
}

void CApplication::Process()
{
	ShowCursor(false);
	SetCursorPos(windowHalfSize.x, windowHalfSize.y);	// Центрирование курсора

	timeBeginPeriod(1);

	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float start = (float)timeGetTime();

			input->Update();
			KeyPress();
			Frame();

			timeDelta = timeGetTime() - start;

			UpdateScene();

			frameCnt++;
			timeElapsed += timeDelta;
			if (timeElapsed >= 1000.0f)
			{
				float fps = (float)frameCnt / timeElapsed * 1000;
				timeElapsed = 0.0f;
				frameCnt = 0;

				if (timer)
					timer -= 1;

				sprintf_s(textFps, 50, "FPS=%5.2f", fps);
			}
		}
	}
	ShowCursor(true);
}

void CApplication::Frame(void)
{
	SetCursorPos(windowHalfSize.x, windowHalfSize.y);

	camera->SetRotation(cameraRotation);
	device->SetCamera(camera);

	device->Begin();

	UpdateDucksPosition();
	RenderObjects();
	RenderUI();

	device->End();

	RECT wndRect;
	GetClientRect(hWnd, &wndRect);
	ValidateRect(hWnd, &wndRect);
}

void CApplication::RenderObjects()
{
	device->d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	device->d3dDevice->LightEnable(0, true);

	for (int i = 0; i < 3; i++)
	{
		light1.Position = D3DXVECTOR3(smDucks[i].position.x + 5, smDucks[i].position.y - 3, smDucks[i].position.z);
		device->d3dDevice->SetLight(0, &light1);

		smDucks[i].elapsedTime = timeDelta / 600;
		smDucks[i].Render(device->d3dDevice);
	}

	device->d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	objSky.ShowMesh(device->d3dDevice);
	objMount.ShowMesh(device->d3dDevice);
	objGround.ShowMesh(device->d3dDevice);

	device->d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	device->d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	objGrass1.ShowMesh(device->d3dDevice);
	objGrass2.ShowMesh(device->d3dDevice);

	device->d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, true);
	device->d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	light1.Position = D3DXVECTOR3(3.0f, 10.0f, 2.0f);
	device->d3dDevice->SetLight(0, &light1);

	smHandgun.elapsedTime = timeDelta / 600;
	smHandgun.Render(device->d3dDevice);

	device->d3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	device->d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	smFire.elapsedTime = timeDelta / 600;
	smFire.Render(device->d3dDevice);
}

void CApplication::RenderUI()
{
	float Width = (float)windowSize.x;
	float Height = (float)windowSize.y;

	if (timer && (magazine || bullets))
	{
		device->Draw2DLine(Width / 2 - 15, Width / 2 - 5, Height / 2, Height / 2, D3DCOLOR_RGBA(0, 255, 0, 150));
		device->Draw2DLine(Width / 2 + 15, Width / 2 + 5, Height / 2, Height / 2, D3DCOLOR_RGBA(0, 255, 0, 150));
		device->Draw2DLine(Width / 2, Width / 2, Height / 2 - 15, Height / 2 - 5, D3DCOLOR_RGBA(0, 255, 0, 150));
		device->Draw2DLine(Width / 2, Width / 2, Height / 2 + 15, Height / 2 + 5, D3DCOLOR_RGBA(0, 255, 0, 150));
	}

	if (showFps)
	{
		fntFPS.DrawText(textFps, 6, 6, D3DCOLOR_RGBA(0, 0, 0, 200));
		fntFPS.DrawText(textFps, 5, 5, D3DCOLOR_RGBA(250, 250, 250, 150));
	}

	if (!timer || !magazine && !bullets)
	{
		if (showMenu)
		{
			txtMenu.DrawTexture(device->d3dDevice);
		}
		else
		{
			txtEnd.DrawTexture(device->d3dDevice);
			sprintf_s(textTemp, 50, "%d", score);
			fntScore2.DrawText(textTemp, long(Width / 2 + 37), long(Height / 2 - 37), D3DCOLOR_RGBA(0, 156, 255, 204));
			cameraRotation = 0;
			smHandgun.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
			smFire.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
		}
	}
	else
	{
		sprintf_s(textTemp, 50, "Время: %d", timer);
		fntTime.DrawText(textTemp, long(22), long(Height - 38), D3DCOLOR_RGBA(0, 0, 0, 150));

		if (timer == 1 || timer == 3 || timer == 5)
			fntTime.DrawText(textTemp, long(20), long(Height - 40), D3DCOLOR_RGBA(250, 0, 0, 150));
		else
			fntTime.DrawText(textTemp, long(20), long(Height - 40), D3DCOLOR_RGBA(250, 250, 250, 150));

		sprintf_s(textTemp, 50, "Утки: %d", ducks);
		fntDucks.DrawText(textTemp, long(Width / 4 + 22), long(Height - 38), D3DCOLOR_RGBA(0, 0, 0, 150));
		fntDucks.DrawText(textTemp, long(Width / 4 + 20), long(Height - 40), D3DCOLOR_RGBA(250, 250, 250, 150));
		sprintf_s(textTemp, 50, "Очки: %d", score);
		fntScore.DrawText(textTemp, long(Width / 4 * 2 + 22), long(Height - 38), D3DCOLOR_RGBA(0, 0, 0, 150));
		fntScore.DrawText(textTemp, long(Width / 4 * 2 + 20), long(Height - 40), D3DCOLOR_RGBA(250, 250, 250, 150));
		sprintf_s(textTemp, 50, "Патроны: %d/%d", bullets, magazine);
		fntBullets.DrawText(textTemp, long(Width / 4 * 3 + 22), long(Height - 38), D3DCOLOR_RGBA(0, 0, 0, 150));
		fntBullets.DrawText(textTemp, long(Width / 4 * 3 + 20), long(Height - 40), D3DCOLOR_RGBA(250, 250, 250, 150));
	}
}

void CApplication::UpdateScene()
{
	if (!haveBullets)
		shootTimer += timeDelta;

	if (bullets)
	{
		if (shootTimer > 550)
		{
			shootTimer = 0;
			haveBullets = true;
			smHandgun.SetAnimation(2);
			smFire.SetAnimation(0);
		}
		else if (needReload && shootTimer > 180)
		{
			needReload = false;
			wavReset->Play(0, 0, -1000, 30050, 0);
		}
	}
	else
	{
		if (shootTimer > 180)
		{
			if (magazine)
				wavCharge->Play(0, 0, -500, 60050, 0);

			shootTimer = 0;
			haveBullets = true;
			if (magazine)
				smHandgun.SetAnimation(1);
			else
				smHandgun.SetAnimation(2);
			smFire.SetAnimation(0);
			timer2 = 1;
		}
	}

	if (!bullets && timer2 && magazine)
		timer2 += timeDelta / 12;

	if (timer2 > 130)
	{
		wavReset->Play(0, 0, -1000, 30050, 0);
		smHandgun.SetAnimation(0);
		timer2 = 0;
		timer3 = 1;
	}

	if (timer3)
		timer3 += timeDelta / 12;

	if (timer3 > 35)
	{
		bullets = 7;
		magazine -= 7;
		smHandgun.SetAnimation(2);
		timer3 = 0;
	}
}

void CApplication::KeyPress()
{
	if (input->MouseButtonNew(0))
	{
		if (haveBullets && timer)
		{
			if (bullets)
			{
				wavFire->Play(0, 0, -1000, 22050, 0);
				haveBullets = false;
				needReload = true;
				smHandgun.SetAnimation(3);
				smFire.SetAnimation(1);

				for (int i = 0; i < 3; i++)
				{
					if (smDucks[i].DirectionCheck(camera->position, camera->direction))
					{
						smDucks[i].velocity.y = -0.1f;
						smDucks[i].SetAnimation(0);
						ducks++;
						score = int(smDucks[i].velocity.z * 500 - 30 + score);
					}
				}

				bullets--;
			}
		}
	}

	if (input->MouseMoveX() || input->MouseMoveY())
	{
		if (timer > 0)
		{
			CPoint mouseMove = CPoint(-input->MouseMoveX(), -input->MouseMoveY());

			cameraRotation += Vector2(float(mouseMove.y) / 700, float(mouseMove.x) / 700);

			cameraRotation.y = min(cameraRotation.y, D3DX_PI / 6);
			cameraRotation.y = max(cameraRotation.y, -D3DX_PI / 6);

			cameraRotation.x = min(cameraRotation.x, D3DX_PI / 3);
			cameraRotation.x = max(cameraRotation.x, 0);

			smHandgun.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
			smFire.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
		}
		else
		{
			cameraRotation = 0;
			
			smHandgun.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
			smFire.rotation = Vector3(cameraRotation.x, cameraRotation.y, 0.0f);
		}
	}

	if (input->Key(DIK_RETURN))
	{
		if (!timer)
		{
			score = 0;
			timer = 60;
			bullets = 7;
			magazine = 70;
			ducks = 0;
			showMenu = false;
		}
	}
	if (input->Key(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}
}

void CApplication::UpdateDucksPosition()
{
	for (int i = 0; i < 3; i++)
	{
		if (smDucks[i].position.y <= 0 || smDucks[i].position.z >= 0)
		{
			smDucks[i].position = Vector3(float(rand() % 40 - 20) * 0.1f + (i * 8 - 8), 4.0f, -53.0f);
			smDucks[i].velocity.z = float(rand() % 20) / 100 + 0.45f;
			smDucks[i].velocity.y = smDucks[i].velocity.z * 0.5f;
			smDucks[i].SetAnimation(1);
		}

		if (smDucks[i].velocity.y < 0)
			smDucks[i].velocity.y -= 0.02f;
	}
}

long __stdcall CApplication::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}; break;
	default: return (long)DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}