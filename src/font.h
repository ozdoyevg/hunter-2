
#include "dxsdk.h"

class CFont
{
public:
	int Size;
	int Weight;
	bool Italic;

	void InitFont(IDirect3DDevice9 *d3dDevice);
	void DrawText(char *Text, long X, long Y, DWORD Color);
private:
	ID3DXFont* font;
};