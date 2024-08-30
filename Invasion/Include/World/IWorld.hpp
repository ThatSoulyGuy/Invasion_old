#pragma once

#include "ECS/GameObjectManager.hpp"
#include "Math/Transform.hpp"
#include "Util/CoordinateHelper.hpp"
#include "World/Chunk.hpp"
#include "World/TextureAtlasManager.hpp"

namespace Invasion::World
{
    class IWorld
    {
    public:

        IWorld(const IWorld&) = delete;
        IWorld& operator=(const IWorld&) = delete;

        void Update(Vector3f loaderPosition)
        {
            Vector3i chunkPosition = CoordinateHelper::WorldToChunkCoordinates(loaderPosition);

            chunkPosition.y = 0;

            UnorderedMap<Vector3i, Shared<Chunk>> newLoadedChunks;

            for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; ++x)
            {
                for (int z = -RENDER_DISTANCE; z <= RENDER_DISTANCE; ++z)
                {
                    Vector3i chunkCoord = chunkPosition + Vector3i(x, 0, z);

                    if (!loadedChunks.Contains(chunkCoord))
                    {
                        Shared<Chunk> newChunk = GenerateChunk(chunkCoord);
                        newLoadedChunks[chunkCoord] = newChunk;
                    }
                    else
                    {
                        newLoadedChunks[chunkCoord] = loadedChunks[chunkCoord];
                        loadedChunks -= chunkCoord;
                    }
                }
            }

            for (const auto& [chunkCoord, chunk] : loadedChunks)
                UnloadChunk(chunkCoord);
            
            loadedChunks = std::move(newLoadedChunks);
        }

        Shared<Chunk> GenerateChunk(const Vector3i& position)
        {
            Shared<GameObject> chunkObject = GameObjectManager::GetInstance().Register(GameObject::Create(String("Chunk_") + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z) + "_"));

            Vector3f worldPosition = CoordinateHelper::ChunkToWorldCoordinates(position);
            chunkObject->GetTransform()->SetLocalPosition(worldPosition);
            
            chunkObject->AddComponent(ShaderManager::GetInstance().Get("default"));
            chunkObject->AddComponent(TextureAtlasManager::GetInstance().Get("default"));

            chunkObject->AddComponent(Mesh::Create(String("Chunk_Mesh_") + std::to_string(position.x) + "_" + std::to_string(position.y) + "_" + std::to_string(position.z) + "_", {}, {}));
            
            Shared<Chunk> chunk = chunkObject->AddComponent(Chunk::Create());

            chunk->Generate();

            return chunk;
        }

        void UnloadChunk(const Vector3i& chunkCoord)
        {
            if (loadedChunks.Contains(chunkCoord))
            {
                Shared<Chunk> chunk = loadedChunks[chunkCoord];
                GameObjectManager::GetInstance().Unregister(chunk->GetGameObject()->GetName());
            }
        }

        static IWorld& GetInstance()
        {
            static IWorld instance;
            return instance;
        }

        static constexpr int RENDER_DISTANCE = 2;

    private:

        IWorld() = default;

        UnorderedMap<Vector3i, Shared<Chunk>> loadedChunks;
    };
}