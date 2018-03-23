#pragma once

#include "Common.h"
#include "Utils/LogUtil.h"

#include <d3d11.h>
#include <d3dcompiler.h>

static bool CheckD3DError(HRESULT result, const char* file, int32 line)
{
	if (result != S_OK && false)
		gebase::utils::LogUtil::WriteLine("ERROR", "[D3D11 Error] File: " + (String)file + "    Line: " + std::to_string(line));

	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		gebase::utils::LogUtil::WriteLine("ERROR", "D3D11_ERROR_FILE_NOT_FOUND");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		gebase::utils::LogUtil::WriteLine("ERROR", "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		gebase::utils::LogUtil::WriteLine("ERROR", "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		gebase::utils::LogUtil::WriteLine("ERROR", "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD");
		break;
	case DXGI_ERROR_INVALID_CALL:
		gebase::utils::LogUtil::WriteLine("ERROR", "DXGI_ERROR_INVALID_CALL");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		gebase::utils::LogUtil::WriteLine("ERROR", "DXGI_ERROR_WAS_STILL_DRAWING");
		break;
	case E_FAIL:
		gebase::utils::LogUtil::WriteLine("ERROR", "E_FAIL");
		break;
	case E_INVALIDARG:
		gebase::utils::LogUtil::WriteLine("ERROR", "E_INVALIDARG");
		break;
	case E_OUTOFMEMORY:
		gebase::utils::LogUtil::WriteLine("ERROR", "E_OUTOFMEMORY");
		break;
	case E_NOTIMPL:
		gebase::utils::LogUtil::WriteLine("ERROR", "E_NOTIMPL");
		break;
	case S_FALSE:
		gebase::utils::LogUtil::WriteLine("ERROR", "S_FALSE");
		break;
	}

	return result == S_OK;
}

#ifdef GE_DEBUG
#define DXCall(x) do {				\
	HRESULT __hr = x;				\
		if(!CheckD3DError(__hr, __FILE__, __LINE__))	\
			__debugbreak();			\
	} while(false)
#else
	#define DXCall(x) x
#endif