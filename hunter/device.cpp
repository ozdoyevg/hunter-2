
#include "common.h"
#include "device.h"

CDevice::CDevice()
{
	d3dDevice = NULL;
	d3dObject = NULL;
}
CDevice::~CDevice()
{
	SAFE_RELEASE(d3dDevice);
	SAFE_RELEASE(d3dObject);
}

void CDevice::Initialize(HWND hwnd, int Width, int Height, bool FullScreen)
{
	D3DDISPLAYMODE d3ddm;
	d3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	d3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (FullScreen)
	{
		d3dpp.BackBufferWidth  = Width;
		d3dpp.BackBufferHeight = Height;
		d3dpp.BackBufferCount  = 1;
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;
		d3dpp.Windowed = false;
	}
	else
	{
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.Windowed = true;
	};
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);

	d3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	d3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x88);
	d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	d3dDevice->SetRenderState(D3DRS_ADAPTIVETESS_Y, (D3DFORMAT)MAKEFOURCC('A', 'T', 'O', 'C'));

	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

	d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, 0);
	d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, 45.0f, (float)Width / (float)Height, 0.1f, 100.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CDevice::Begin()
{
	d3dDevice->BeginScene();
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(44, 111, 179), 1.0f, 0);
}

void CDevice::End()
{
	d3dDevice->EndScene();
	d3dDevice->Present(0, 0, 0, 0);
}

void CDevice::Draw2DLine(float X1, float X2, float Y1, float Y2, DWORD Color)
{
	struct
	{
		float x, y, z, rhw;
		D3DCOLOR color;
	} verticies[2] = {X1, Y1, 0, 0, Color,
					  X2, Y2, 0, 0, Color};

	d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	d3dDevice->SetRenderState(D3DRS_POINTSIZE, 5);
	d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 2, verticies, sizeof(verticies[0]));
	d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	d3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}

void CDevice::SetCamera(CCamera * camera)
{
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 vLookAt = camera->direction;
	D3DXVECTOR3 vEyePos = camera->position;

	D3DXMatrixLookAtLH( &matView, &vEyePos, &vLookAt, &vUpVec );
	d3dDevice->SetTransform(D3DTS_VIEW, &matView);
}
