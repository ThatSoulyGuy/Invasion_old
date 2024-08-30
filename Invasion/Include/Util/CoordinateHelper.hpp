#pragma once

#include "Math/Vector3.hpp"
#include "World/Chunk.hpp"

using namespace Invasion::Math;
using namespace Invasion::World;

namespace Invasion::Util
{
	class CoordinateHelper
	{
	public:

        CoordinateHelper(const CoordinateHelper&) = delete;
        CoordinateHelper& operator=(const CoordinateHelper&) = delete;

        static Vector3i WorldToChunkCoordinates(const Vector3f& worldPosition)
        {
            return Vector3i
            (
                static_cast<int>(std::floor(worldPosition.x / Chunk::CHUNK_SIZE)),
                static_cast<int>(std::floor(worldPosition.y / Chunk::CHUNK_SIZE)),
                static_cast<int>(std::floor(worldPosition.z / Chunk::CHUNK_SIZE))
            );
        }

        static Vector3f ChunkToWorldCoordinates(const Vector3i& chunkPosition)
        {
            return Vector3f
            (
                chunkPosition.x * (float)Chunk::CHUNK_SIZE,
                chunkPosition.y * (float)Chunk::CHUNK_SIZE,
                chunkPosition.z * (float)Chunk::CHUNK_SIZE
            );
        }

    private:

		CoordinateHelper() = default;

	};
}