#pragma once

#include "DX11Common.h"

namespace gebase { namespace graphics {

	struct TextureHandle
	{
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* resourceView;
		ID3D11SamplerState* samplerState;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SAMPLER_DESC samplerDesc;
	};

} }