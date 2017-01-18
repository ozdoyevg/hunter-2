
#include "input.h"
#include "common.h"

CInput::CInput()
{
	m_pDirectInput = 0;
	m_pKeyboardDevice = 0;
	m_pMouseDevice = 0;
}

CInput::~CInput()
{
	if (m_pKeyboardDevice) m_pKeyboardDevice->Unacquire();
	if (m_pMouseDevice) m_pMouseDevice->Unacquire();
	SAFE_RELEASE(m_pKeyboardDevice);
	SAFE_RELEASE(m_pMouseDevice);
	SAFE_RELEASE(m_pDirectInput);
}

void CInput::Initialize(HWND hWindow)
{
    ZeroMemory(&DIks, sizeof(BYTE)*256);
    for(unsigned short i = 0; i < 256; i++)
		kNew[i] = true;
    ZeroMemory(&DIms, sizeof(DIMOUSESTATE2));
    for(unsigned short i = 0; i < 8; i++)
		mbNew[i] = true;

	HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);

	m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboardDevice->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);

	m_pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	m_pMouseDevice->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

void CInput::Update(void)
{
    HRESULT hr;

    hr = m_pKeyboardDevice->GetDeviceState(sizeof(BYTE)*256, &DIks);
    if (FAILED(hr))
    {
        do
		{
			hr = m_pKeyboardDevice->Acquire();
        }
        while(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED);
    }

    hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &DIms);
    if (FAILED(hr))
    {
        do
		{
			hr = m_pMouseDevice->Acquire();
        }
        while(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED);
	}
}


bool CInput::Key(BYTE in_kb)
{
    if (DIks[in_kb] & 0x80) return true;
    else return false;
}

bool CInput::KeyNew(BYTE in_kb)
{
    if (DIks[in_kb] & 0x80)
    {
        if (kNew[in_kb]){ kNew[in_kb] = false; return true; }
    }
    else kNew[in_kb] = true;

    return false;
}

bool CInput::MouseButton(BYTE in_mb)
{
    if (DIms.rgbButtons[in_mb] & 0x80) return true;
    else return false;
}

bool CInput::MouseButtonNew(BYTE in_mb)
{
    if (DIms.rgbButtons[in_mb] & 0x80)
    {
        if (mbNew[in_mb]){ mbNew[in_mb] = false; return true; }
    }
    else mbNew[in_mb] = true;

    return false;
}

LONG CInput::MouseMoveX() {
	return DIms.lX;
}

LONG CInput::MouseMoveY() {
	return DIms.lY;
}

LONG CInput::MouseMoveZ() {
	return DIms.lZ;
}