#pragma once

#include "ECS/GameObjectManager.hpp"
#include "Math/Transform.hpp"
#include "Thread/ThreadPool.hpp"
#include "Util/CoordinateHelper.hpp"
#include "World/Chunk.hpp"
#include "World/TextureAtlasManager.hpp"

using namespace Invasion::Thread;

namespace Invasion::World
{
    class IWorld
    {

    public:

        IWorld(const IWorld&) = delete;
        IWorld& operator=(const IWorld&) = delete;

        void Update(Vector3f loaderPosition)
        {
            updateFuture = threadPool += ([this, loaderPosition] { UpdateInternal(loaderPosition); });
        }

        void WaitForUpdate()
        {
            if (updateFuture.valid())
            {
                try
                {
                    updateFuture.get();
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Exception in UpdateInternal: " << e.what() << std::endl;
                }
            }
        }

        static IWorld& GetInstance()
        {
            static IWorld instance;
            return instance;
        }

        static constexpr int RENDER_DISTANCE = 2;

    private:

        IWorld() : threadPool(4) {} 

        void UpdateInternal(Vector3f loaderPosition)
        {
            Vector3i chunkPosition = CoordinateHelper::WorldToChunkCoordinates(loaderPosition);
            chunkPosition.y = 0;

            UnorderedMap<Vector3i, Shared<Chunk>> newLoadedChunks;
            Vector<Future<Pair<Vector3i, Shared<Chunk>>>> futures;

            for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; ++x)
            {
                for (int z = -RENDER_DISTANCE; z <= RENDER_DISTANCE; ++z)
                {
                    Vector3i chunkCoord = chunkPosition + Vector3i(x, 0, z);

                    std::unique_lock<std::mutex> lock(mutex);

                    if (!loadedChunks.Contains(chunkCoord))
                    {
                        lock.unlock();
                        futures |= threadPool += ([this, chunkCoord] { return GenerateChunk(chunkCoord); });
                    }
                    else
                    {
                        newLoadedChunks[chunkCoord] = loadedChunks[chunkCoord];
                        loadedChunks -= chunkCoord;
                    }
                }
            }

            for (auto& future : futures)
            {
                auto [chunkCoord, chunk] = future.get();
                LockGuard<Mutex> lock(mutex);
                newLoadedChunks[chunkCoord] = chunk;
            }

            Vector<Vector3i> chunksToUnload;
            {
                LockGuard<Mutex> lock(mutex);

                for (const auto& [chunkCoord, chunk] : loadedChunks)
                {
                    if (!newLoadedChunks.Contains(chunkCoord))
                        chunksToUnload += chunkCoord;
                }
            }

            chunksToUnload.ForEach([this](Vector3i& chunkCoord) { UnloadChunk(chunkCoord); });

            {
                LockGuard<Mutex> lock(mutex);
                loadedChunks = std::move(newLoadedChunks);
            }
        }

        Pair<Vector3i, Shared<Chunk>> GenerateChunk(const Vector3i& position)
        {
            Shared<GameObject> chunkObject = GameObjectManager::GetInstance().Register(GameObject::Create(
                Formatter::Format("Chunk_{}_{}_{}_", position.x, position.y, position.z)));

            Vector3f worldPosition = CoordinateHelper::ChunkToWorldCoordinates(position);
            chunkObject->GetTransform()->SetLocalPosition(worldPosition);

            chunkObject->AddComponent(ShaderManager::GetInstance().Get("default"));
            chunkObject->AddComponent(TextureAtlasManager::GetInstance().Get("default"));

            chunkObject->AddComponent(Mesh::Create(Formatter::Format("Chunk_Mesh_{}_{}_{}_", position.x, position.y, position.z), {}, {}));

            Shared<Chunk> chunk = chunkObject->AddComponent(Chunk::Create());
            chunk->Generate();

            return { position, std::move(chunk) };
        }

        Optional<Vector3i> UnloadChunk(const Vector3i& chunkCoord)
        {
            LockGuard<Mutex> lock(mutex);

            if (loadedChunks.Contains(chunkCoord))
            {
                Shared<Chunk> chunk = loadedChunks[chunkCoord];
                GameObjectManager::GetInstance().Unregister(chunk->GetGameObject()->GetName());
                chunk.reset();

                return chunkCoord;
            }

            return Optional<Vector3i>();
        }

        Future<void> updateFuture;
        Mutex mutex;
        UnorderedMap<Vector3i, Shared<Chunk>> loadedChunks;
        ThreadPool threadPool;
    };
}