
#pragma once

#include <dsound.h>

#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

#define DSCREATION_FLAGS (DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY)

class CWaveFile
{
public:
	CWaveFile();

	HRESULT Open(char *strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags);
	HRESULT ReadMMIO();
	HRESULT ResetFile();
	HRESULT WriteMMIO(WAVEFORMATEX *pwfxDest);
	HRESULT Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);

	DWORD m_dwSize;
	WAVEFORMATEX* m_pwfx;
private:
	HMMIO m_hmmio;
	DWORD m_dwFlags;
	CHAR* m_pResourceBuffer;
	MMCKINFO m_ck;
	MMCKINFO m_ckRiff;
    MMIOINFO m_mmioinfoOut;
    BOOL m_bIsReadingFromMemory;
    BYTE* m_pbData;
    BYTE* m_pbDataCur;
	ULONG m_ulDataSize;
};

class CSound
{
public:
	CSound(LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, DWORD dwNumBuffers, CWaveFile* pWaveFile, DWORD dwCreationFlags);
	HRESULT Play(DWORD dwPriority, DWORD dwFlags, LONG lVolume, LONG lFrequency, LONG lPan);
	LPDIRECTSOUNDBUFFER GetFreeBuffer();
	HRESULT RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored);
	HRESULT FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger);
private:
    LPDIRECTSOUNDBUFFER* m_apDSBuffer;
    DWORD                m_dwDSBufferSize;
    DWORD                m_dwNumBuffers;
    DWORD                m_dwCreationFlags;
	CWaveFile*       m_pWaveFile;
};

class CSoundManager
{
public:
	HRESULT Initialize(HWND hWnd, DWORD dwCoopLevel);
	HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate);
	HRESULT Create(CSound** ppSound, char *strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm, DWORD dwNumBuffers );
private:
	IDirectSound8* m_pDS;
};