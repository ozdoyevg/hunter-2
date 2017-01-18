
#include "common.h"
#include "font.h"

void CFont::InitFont(IDirect3DDevice9 *d3dDevice)
{
	D3DXFONT_DESC pDesc;
	ZeroMemory(&pDesc, sizeof(D3DXFONT_DESC));
	pDesc.Weight    = Weight;
	pDesc.Italic    = Italic;
	pDesc.CharSet   = DEFAULT_CHARSET;
	strcpy_s(pDesc.FaceName, 32, "Tahoma");
	pDesc.Height    = Size * 2;
	pDesc.Width     = Size;
	D3DXCreateFontIndirect(d3dDevice, &pDesc, &font);
}
void CFont::DrawText(char *Text, long X, long Y, DWORD Color)
{
	RECT rect;
	rect.left = X;
	rect.top = Y;
	rect.right = 2000;
	rect.bottom = 2000;

	font->DrawText(0, Text, -1, &rect, DT_TOP | DT_LEFT, Color);
}