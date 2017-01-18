
#include <Windows.h>

#include "sound.h"
#include "common.h"

CSound::CSound(LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, DWORD dwNumBuffers, CWaveFile* pWaveFile, DWORD dwCreationFlags)
{
    DWORD i;

    if( dwNumBuffers <= 0 )
        return;

    m_apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
    if( NULL != m_apDSBuffer )
    {
        for( i=0; i<dwNumBuffers; i++ )
            m_apDSBuffer[i] = apDSBuffer[i];

        m_dwDSBufferSize = dwDSBufferSize;
        m_dwNumBuffers   = dwNumBuffers;
        m_pWaveFile      = pWaveFile;
        m_dwCreationFlags = dwCreationFlags;

        FillBufferWithSound(m_apDSBuffer[0], false);
    }
}

HRESULT CSound::Play(DWORD dwPriority, DWORD dwFlags, LONG lVolume, LONG lFrequency, LONG lPan)
{
    BOOL    bRestored;

    if( m_apDSBuffer == NULL )
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER pDSB = GetFreeBuffer();

    RestoreBuffer(pDSB, &bRestored);

    if(bRestored)
    {
        FillBufferWithSound(pDSB, false);
    }

    if(m_dwCreationFlags & DSBCAPS_CTRLVOLUME)
    {
        pDSB->SetVolume(lVolume);
    }

    if(lFrequency != -1 &&
        (m_dwCreationFlags & DSBCAPS_CTRLFREQUENCY))
    {
        pDSB->SetFrequency(lFrequency);
    }

    if(m_dwCreationFlags & DSBCAPS_CTRLPAN)
    {
        pDSB->SetPan(lPan);
    }

	pDSB->SetCurrentPosition(0);
    return pDSB->Play(0, dwPriority, dwFlags);
}

LPDIRECTSOUNDBUFFER CSound::GetFreeBuffer()
{
    if( m_apDSBuffer == NULL )
        return false;

	DWORD i;
    for( i=0; i<m_dwNumBuffers; i++ )
    {
        if( m_apDSBuffer[i] )
        {
            DWORD dwStatus = 0;
            m_apDSBuffer[i]->GetStatus( &dwStatus );
            if ( ( dwStatus & DSBSTATUS_PLAYING ) == 0 )
                break;
        }
    }

    if( i != m_dwNumBuffers )
        return m_apDSBuffer[ i ];
    else
        return m_apDSBuffer[ rand() % m_dwNumBuffers ];
}

HRESULT CSound::RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored)
{
    HRESULT hr;
    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;
    if( pbWasRestored )
        *pbWasRestored = false;

    DWORD dwStatus;
    pDSB->GetStatus( &dwStatus );

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        do
        {
            hr = pDSB->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while((hr = pDSB->Restore()) == DSERR_BUFFERLOST);

        if( pbWasRestored != NULL )
            *pbWasRestored = TRUE;

        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT CSound::FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger)
{
	VOID*   pDSLockedBuffer      = NULL;
	DWORD   dwDSLockedBufferSize = 0;
	DWORD   dwWavDataRead        = 0;

	if( pDSB == NULL )
		return CO_E_NOTINITIALIZED;

	RestoreBuffer( pDSB, NULL );
	pDSB->Lock( 0, m_dwDSBufferSize, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, 0L);
	m_pWaveFile->ResetFile();
	m_pWaveFile->Read((BYTE*) pDSLockedBuffer, dwDSLockedBufferSize, &dwWavDataRead);

	if( dwWavDataRead == 0 )
	{
		FillMemory((BYTE*) pDSLockedBuffer, 
			dwDSLockedBufferSize, 
			(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ));
	}
	if( dwWavDataRead < dwDSLockedBufferSize )
	{
		if( bRepeatWavIfBufferLarger )
		{
			DWORD dwReadSoFar = dwWavDataRead;
			while(dwReadSoFar < dwDSLockedBufferSize )
			{
				m_pWaveFile->ResetFile();
                m_pWaveFile->Read((BYTE*)pDSLockedBuffer + dwReadSoFar,
                                        dwDSLockedBufferSize - dwReadSoFar,
                                        &dwWavDataRead);

				dwReadSoFar += dwWavDataRead;
			}
		}
		else
        {
			FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead,
				dwDSLockedBufferSize - dwWavDataRead,
				(BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
		}
	}
	pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    return S_OK;
}

HRESULT CSoundManager::Initialize(HWND hWnd, DWORD dwCoopLevel)
{
    SAFE_RELEASE(m_pDS);

    DirectSoundCreate8(NULL, &m_pDS, NULL);
    m_pDS->SetCooperativeLevel(hWnd, dwCoopLevel);

    return S_OK;
}

HRESULT CSoundManager::SetPrimaryBufferFormat(DWORD dwPrimaryChannels,
                                          DWORD dwPrimaryFreq,
                                          DWORD dwPrimaryBitRate)
{
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if(m_pDS == NULL)
        return CO_E_NOTINITIALIZED;

    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;

    m_pDS->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL);

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
    wfx.wFormatTag      = (WORD) WAVE_FORMAT_PCM;
    wfx.nChannels       = (WORD) dwPrimaryChannels;
    wfx.nSamplesPerSec  = (DWORD) dwPrimaryFreq;
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate;
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

    pDSBPrimary->SetFormat(&wfx);

    SAFE_RELEASE(pDSBPrimary);
    return S_OK;
}

HRESULT CSoundManager::Create(CSound** ppSound,
						  char *strWaveFileName,
                          DWORD dwCreationFlags,
                          GUID guid3DAlgorithm,
                          DWORD dwNumBuffers )
{
    HRESULT hr;
    HRESULT hrRet = S_OK;
    DWORD   i;
    LPDIRECTSOUNDBUFFER* apDSBuffer     = NULL;
    DWORD                dwDSBufferSize = NULL;
    CWaveFile*       pWaveFile      = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;
    if( strWaveFileName == NULL || ppSound == NULL || dwNumBuffers < 1 )
        return E_INVALIDARG;

    apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
    if( apDSBuffer == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    pWaveFile = new CWaveFile();
    if( pWaveFile == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ );

    if( pWaveFile->m_dwSize == 0 )
    {
        hr = E_FAIL;
        goto LFail;
    }

    dwDSBufferSize = pWaveFile->m_dwSize;

    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = pWaveFile->m_pwfx;

    hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer[0], NULL );

    if( hr == DS_NO_VIRTUALIZATION )
        hrRet = DS_NO_VIRTUALIZATION;

    if( FAILED(hr) )
    {
        goto LFail;
    }

    if( (dwCreationFlags & DSBCAPS_CTRLFX) == 0 )
    {
        for( i=1; i<dwNumBuffers; i++ )
        {
            if( FAILED( hr = m_pDS->DuplicateSoundBuffer( apDSBuffer[0], &apDSBuffer[i] ) ) )
            {
                goto LFail;
            }
        }
    }
    else
    {
        for( i=1; i<dwNumBuffers; i++ )
        {
            hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer[i], NULL );
            if( FAILED(hr) )
            {
                goto LFail;
            }
        }
   }

    *ppSound = new CSound( apDSBuffer, dwDSBufferSize, dwNumBuffers, pWaveFile, dwCreationFlags );

    SAFE_DELETE_ARRAY(apDSBuffer);
    return hrRet;

LFail:
    SAFE_DELETE(pWaveFile);
    SAFE_DELETE_ARRAY(apDSBuffer);
    return hr;
}

CWaveFile::CWaveFile()
{
    m_pwfx    = NULL;
    m_hmmio   = NULL;
    m_pResourceBuffer = NULL;
    m_dwSize  = 0;
    m_bIsReadingFromMemory = false;
}

HRESULT CWaveFile::Open(char *strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags)
{
    HRESULT hr;

    m_dwFlags = dwFlags;

    if( m_dwFlags == WAVEFILE_READ )
    {
        if( strFileName == NULL )
            return E_INVALIDARG;
        SAFE_DELETE_ARRAY(m_pwfx);

        m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );

        if( NULL == m_hmmio )
        {
            HRSRC   hResInfo;
            HGLOBAL hResData;
            DWORD   dwSize;
            VOID*   pvRes;

            if( NULL == ( hResInfo = FindResource( NULL, strFileName, "WAVE" ) ) )
            {
                hResInfo = FindResource( NULL, strFileName, "WAV" );
            }

            hResData = LoadResource( GetModuleHandle(NULL), hResInfo );

            dwSize = SizeofResource( GetModuleHandle(NULL), hResInfo );

            pvRes = LockResource( hResData );

            m_pResourceBuffer = new CHAR[ dwSize ];
            memcpy( m_pResourceBuffer, pvRes, dwSize );

            MMIOINFO mmioInfo;
            ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
            mmioInfo.fccIOProc = FOURCC_MEM;
            mmioInfo.cchBuffer = dwSize;
            mmioInfo.pchBuffer = (CHAR*) m_pResourceBuffer;

            m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
        }

        if( FAILED( hr = ReadMMIO() ) )
        {
            mmioClose( m_hmmio, 0 );
        }

        hr = ResetFile();

        m_dwSize = m_ck.cksize;
    }
    else
    {
        m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF  |
                                                  MMIO_READWRITE |
                                                  MMIO_CREATE );

        if( FAILED( hr = WriteMMIO( pwfx ) ) )
        {
            mmioClose( m_hmmio, 0 );
        }

        hr = ResetFile();
    }

    return hr;
}

HRESULT CWaveFile::ReadMMIO()
{
    MMCKINFO        ckIn;
    PCMWAVEFORMAT   pcmWaveFormat;

    m_pwfx = NULL;

    mmioDescend(m_hmmio, &m_ckRiff, NULL, 0);
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK );

    mmioRead(m_hmmio, (HPSTR) &pcmWaveFormat, sizeof(pcmWaveFormat));

    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
    {
        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = 0;
    }
    else
    {
        WORD cbExtraBytes = 0L;
        mmioRead(m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD));

        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];

        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = cbExtraBytes;

        if( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),
                      cbExtraBytes ) != cbExtraBytes )
        {
            SAFE_DELETE( m_pwfx );
        }
    }

    if(0!=mmioAscend(m_hmmio, &ckIn, 0))
    {
        SAFE_DELETE( m_pwfx );
    }

    return S_OK;
}

HRESULT CWaveFile::ResetFile()
{
    if( m_bIsReadingFromMemory )
    {
        m_pbDataCur = m_pbData;
    }
    else
    {
        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;

        if( m_dwFlags == WAVEFILE_READ )
        {
            mmioSeek(m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC), SEEK_SET);

            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK );
        }
        else
        {
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            m_ck.cksize = 0;

            mmioCreateChunk(m_hmmio, &m_ck, 0);
            mmioGetInfo(m_hmmio, &m_mmioinfoOut, 0);
        }
    }

    return S_OK;
}

HRESULT CWaveFile::WriteMMIO(WAVEFORMATEX *pwfxDest)
{
    DWORD    dwFactChunk;
    MMCKINFO ckOut1;

    dwFactChunk = (DWORD)-1;

    m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    m_ckRiff.cksize = 0;

    mmioCreateChunk(m_hmmio, &m_ckRiff, MMIO_CREATERIFF);

    m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
    m_ck.cksize = sizeof(PCMWAVEFORMAT);

    mmioCreateChunk( m_hmmio, &m_ck, 0 );

    if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
    {
        mmioWrite(m_hmmio, (HPSTR) pwfxDest, sizeof(PCMWAVEFORMAT));
    }
    else
    {
        mmioWrite(m_hmmio, (HPSTR) pwfxDest, sizeof(*pwfxDest));
    }

    mmioAscend(m_hmmio, &m_ck, 0);

    ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
    ckOut1.cksize = 0;

    mmioCreateChunk(m_hmmio, &ckOut1, 0);
    mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk));
    mmioAscend(m_hmmio, &ckOut1, 0);

    return S_OK;
}

HRESULT CWaveFile::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead)
{
    if( m_bIsReadingFromMemory )
    {
        if( m_pbDataCur == NULL )
            return CO_E_NOTINITIALIZED;
        if( pdwSizeRead != NULL )
            *pdwSizeRead = 0;

        if( (BYTE*)(m_pbDataCur + dwSizeToRead) >
            (BYTE*)(m_pbData + m_ulDataSize))
        {
            dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        }

        CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );

        if( pdwSizeRead != NULL )
            *pdwSizeRead = dwSizeToRead;

        return S_OK;
    }
    else
    {
        MMIOINFO mmioinfoIn;

        if(m_hmmio == NULL )
           return CO_E_NOTINITIALIZED;
        if(pBuffer == NULL || pdwSizeRead == NULL)
           return E_INVALIDARG;

        if(pdwSizeRead != NULL)
           *pdwSizeRead = 0;

        mmioGetInfo(m_hmmio, &mmioinfoIn, 0);

        UINT cbDataIn = dwSizeToRead;
        if( cbDataIn > m_ck.cksize )
            cbDataIn = m_ck.cksize;

        m_ck.cksize -= cbDataIn;

        for( DWORD cT = 0; cT < cbDataIn; cT++ )
        {
            if(mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
            {
                mmioAdvance(m_hmmio, &mmioinfoIn, MMIO_READ);
            }
            *((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
            mmioinfoIn.pchNext++;
        }

        mmioSetInfo(m_hmmio, &mmioinfoIn, 0);

        if( pdwSizeRead != NULL )
            *pdwSizeRead = cbDataIn;

        return S_OK;
    }
}
