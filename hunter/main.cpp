
#include "common.h"
#include "application.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	CApplication * app = new CApplication();
	app->Initialize(hInstance);
	app->LoadScene();
	app->Process();

	SAFE_DELETE(app);
	return 0;
}

