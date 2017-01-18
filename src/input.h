
#pragma once

#include <Windows.h>
#include "dxsdk.h"

class CInput
{
public:
	CInput();
	~CInput();

	void Initialize(HWND hWindow);
	void Update();

	bool Key(BYTE in_kb);
	bool KeyNew(BYTE in_kb);
	bool MouseButton(BYTE in_mb);
	bool MouseButtonNew(BYTE in_mb);
	LONG MouseMoveX();
	LONG MouseMoveY();
	LONG MouseMoveZ();
private:
	IDirectInput8 * m_pDirectInput;
	IDirectInputDevice8 * m_pKeyboardDevice;
	IDirectInputDevice8 * m_pMouseDevice;

	BYTE DIks[256];
	bool kNew[256];
	bool mbNew[8];
	DIMOUSESTATE2 DIms;
};