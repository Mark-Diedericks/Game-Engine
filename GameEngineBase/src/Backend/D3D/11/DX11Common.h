#pragma once

#include "Common.h"

#include <d3d11.h>
#include <d3dcompiler.h>

static bool CheckD3DError(HRESULT result, const char* file, int32 line)
{
	if (result != S_OK)
		std::cout << "[D3D11 Error] File: " << file << "    Line: " << line << std::endl;

	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		std::cout << "D3D11_ERROR_FILE_NOT_FOUND" << std::endl;
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		std::cout << "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS" << std::endl;
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		std::cout << "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS" << std::endl;
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		std::cout << "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD" << std::endl;
		break;
	case DXGI_ERROR_INVALID_CALL:
		std::cout << "DXGI_ERROR_INVALID_CALL" << std::endl;
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		std::cout << "DXGI_ERROR_WAS_STILL_DRAWING" << std::endl;
		break;
	case E_FAIL:
		std::cout << "E_FAIL" << std::endl;
		break;
	case E_INVALIDARG:
		std::cout << "E_INVALIDARG" << std::endl;
		break;
	case E_OUTOFMEMORY:
		std::cout << "E_OUTOFMEMORY" << std::endl;
		break;
	case E_NOTIMPL:
		std::cout << "E_NOTIMPL" << std::endl;
		break;
	case S_FALSE:
		std::cout << "S_FALSE" << std::endl;
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