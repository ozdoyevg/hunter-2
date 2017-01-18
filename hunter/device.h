
#include "dxsdk.h"
#include "camera.h"

class CDevice
{ 
public:
	IDirect3DDevice9 *d3dDevice;

	int Width;
	int Height;
	bool FullScreen;
	bool MotionBlur;

	CDevice();
	~CDevice();

	void Initialize(HWND hwnd, int Width, int Height, bool FullScreen);

	void Begin();
	void End();

	void Draw2DLine(float X1, float X2, float Y1, float Y2, DWORD Color);
	void SetCamera(CCamera * camera);
private:
	IDirect3D9 *d3dObject;
	IDirect3DTexture9 *MotionTexture;
	IDirect3DVertexBuffer9 *MotionVertexBuffer;
	IDirect3DSurface9  *OLD_RT, *RT;
	D3DPRESENT_PARAMETERS d3dpp;
};