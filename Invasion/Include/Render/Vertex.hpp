#pragma once

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Util/Typedefs.hpp"

using namespace Invasion::Math;
using namespace Invasion::Util;

namespace Invasion::Render
{
	struct Vertex
	{
		Vector3f position;
		Vector3f color;
		Vector3f normal;
		Vector2f textureCoordinates;

		static Array<D3D11_INPUT_ELEMENT_DESC, 4> GetInputElementLayout()
		{
			return
			{
				D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
		}
	};
}