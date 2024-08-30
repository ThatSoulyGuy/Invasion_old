#pragma once

#include "ECS/GameObject.hpp"
#include "Render/Mesh.hpp"
#include "World/TextureAtlas.hpp"

using namespace Invasion::ECS;
using namespace Invasion::Render;

namespace Invasion::World
{
	class Chunk : public Component
	{
	public:

		void Initialize() override
		{
			mesh = GetGameObject()->GetComponent<Mesh>();

			Vector<int> blocks(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, 1);

			CompressBlocks(blocks);
		}

		void Generate()
		{
			Vector<int> blocks = DecompressBlocks();
			vertices.Clear();
			indices.Clear();

			for (int x = 0; x < CHUNK_SIZE; ++x)
			{
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					for (int z = 0; z < CHUNK_SIZE; ++z)
					{
						int block = GetBlock(blocks, { x, y, z });

						if (block == 0) 
							continue;

						for (int i = 0; i < 6; ++i)
						{
							int nx = x + FaceOffsets[i][0];
							int ny = y + FaceOffsets[i][1];
							int nz = z + FaceOffsets[i][2];

 							if (!IsValidPosition({ nx, ny, nz }) || GetBlock(blocks, { nx, ny, nz }) == 0)
								AddFace({ x, y, z }, i);
						}
					}
				}
			}

			mesh->SetVertices(vertices);
			mesh->SetIndices(indices);

			mesh->Generate();
		}

		static Shared<Chunk> Create()
		{
			class Enabled : public Chunk { };
			Shared<Chunk> result = std::make_shared<Enabled>();

			return std::move(result);
		}

		static constexpr int CHUNK_SIZE = 16;

	private:

		Chunk() = default;

		Shared<Mesh> mesh;

		Vector<Pair<int, int>> rleBlocks;

		Vector<Vertex> vertices;
		Vector<unsigned int> indices;

		static constexpr int FaceOffsets[6][3] = 
		{
			{ 1, 0, 0 }, { -1, 0, 0 },
			{ 0, 1, 0 }, { 0, -1, 0 },
			{ 0, 0, 1 }, { 0, 0, -1 }
		};

		int GetBlock(const Vector<int>& blocks, const Vector3i& position) const
		{
			if (!IsValidPosition(position))
				return 0;

			size_t index = static_cast<size_t>(position.x) +
				static_cast<size_t>(position.y) * CHUNK_SIZE +
				static_cast<size_t>(position.z) * CHUNK_SIZE * CHUNK_SIZE;

			return blocks[index];
		}

		bool IsValidPosition(const Vector3i& position) const
		{
			return position.x >= 0 && position.x < CHUNK_SIZE &&
				position.y >= 0 && position.y < CHUNK_SIZE &&
				position.z >= 0 && position.z < CHUNK_SIZE;
		}

		void AddFace(const Vector3i& position, int faceIndex)
		{
			static const Vector3f faceOffsets[6][4] = 
			{
				{ Vector3f(1, 0, 0), Vector3f(1, 1, 0), Vector3f(1, 1, 1), Vector3f(1, 0, 1) },
				{ Vector3f(0, 0, 1), Vector3f(0, 1, 1), Vector3f(0, 1, 0), Vector3f(0, 0, 0) },
				{ Vector3f(0, 1, 1), Vector3f(1, 1, 1), Vector3f(1, 1, 0), Vector3f(0, 1, 0) },
				{ Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector3f(1, 0, 1), Vector3f(0, 0, 1) },
				{ Vector3f(1, 0, 1), Vector3f(1, 1, 1), Vector3f(0, 1, 1), Vector3f(0, 0, 1) },
				{ Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector3f(1, 1, 0), Vector3f(1, 0, 0) },
			};

			Array<Vector2f, 4> texCoords = GetGameObject()->GetComponent<TextureAtlas>()->GetTextureCoordinates("dirt");

			static const Vector3f normals[6] =
			{
				Vector3f(1.0f, 0.0f, 0.0f),
				Vector3f(-1.0f, 0.0f, 0.0f),
				Vector3f(0.0f, 1.0f, 0.0f),
				Vector3f(0.0f, -1.0f, 0.0f), 
				Vector3f(0.0f, 0.0f, 1.0f),
				Vector3f(0.0f, 0.0f, -1.0f)
			};

			Vector3f faceVertices[4];
			Vector3f normal = normals[faceIndex];

			for (int i = 0; i < 4; ++i)
				faceVertices[i] = Vector3f(position) + faceOffsets[faceIndex][i];
			
			normal = normals[faceIndex];

			int startIndex = vertices.Length();

			for (int i = 0; i < 4; ++i)
				vertices += Vertex{ faceVertices[i], Vector3f(1.0f, 1.0f, 1.0f), normal, texCoords[i] };
			
			indices += startIndex + 0;
			indices += startIndex + 1;
			indices += startIndex + 2;
			indices += startIndex + 2;
			indices += startIndex + 3;
			indices += startIndex + 0;
		}

		void CompressBlocks(const Vector<int>& blocks)
		{
			rleBlocks.Clear();

			if (blocks.IsEmpty())
				return;

			int currentBlock = blocks[0];
			int runLength = 1;

			for (size_t i = 1; i < blocks.Length(); ++i)
			{
				if (blocks[i] == currentBlock)
					runLength++;
				else
				{
					rleBlocks += Pair(currentBlock, runLength);
					currentBlock = blocks[i];
					runLength = 1;
				}
			}

			rleBlocks += Pair(currentBlock, runLength);
		}

		Vector<int> DecompressBlocks() const
		{
			Vector<int> decompressed;

			for (const auto& [block, length] : rleBlocks)
			{
				decompressed.Reserve(decompressed.Length() + length);

				for (int i = 0; i < length; ++i)
					decompressed += block;
			}

			return decompressed;
		}
	};
}