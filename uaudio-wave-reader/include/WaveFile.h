#pragma once

#include <cstdlib>
#include <cstring>

#include "./UAUDIO_WAVE_READER_RESULT.h"
#include "./WaveChunkData.h"
#include "./Utils.h"

namespace uaudio
{
	namespace wave_reader
	{
		/*
		 * WHAT IS THIS FILE?
		 * This is the WaveFile class, where all the chunks are stored.
		 * It has methods to retrieve specific chunks and can remove chunks as well.
		 *
		 * This class does not hold information such as volume, panning, looping and pitch. It only stored the actual wave file information.
		*/
		class WaveReader;
		class WaveChunkAllocator;

		class WaveFile
		{
		public:
			WaveFile() = default;
			WaveFile(const WaveFile& rhs) = default;

			~WaveFile();

			WaveFile& operator=(const WaveFile& rhs) = default;

			UAUDIO_WAVE_READER_RESULT GetChunkSize(uint32_t& a_Size, const char* a_ChunkID) const;
			UAUDIO_WAVE_READER_RESULT GetNumberOfChunks(size_t& a_Size) const;
			UAUDIO_WAVE_READER_RESULT HasChunk(bool& a_ChunkFound, const char* a_ChunkID) const;
		private:
			friend class WaveReader;

			WaveChunkData* GetChunkFromAllocator(const char* a_ChunkID) const;
			WaveChunkAllocator* m_Chunks = nullptr;
		public:
			/// <summary>
			/// Adds a chunk to the chunk list.
			/// </summary>
			/// <param name="a_ChunkData"></param>
			/// <param name="a_ChunkID">The chunk id (must be a length of 4 characters).</param>
			/// <param name="a_Chunk">The chunk data.</param>
			template <class T>
			static UAUDIO_WAVE_READER_RESULT ConstructChunk(WaveChunkData*& a_ChunkData, const char* a_ChunkID, T& a_Chunk)
			{
				a_ChunkData = reinterpret_cast<WaveChunkData*>(malloc(sizeof(WaveChunkData) + sizeof(T)));

				// Set the chunk id.
				if (a_ChunkID != nullptr)
					memcpy(a_ChunkData->chunk_id, a_ChunkID, CHUNK_ID_SIZE);

				// Set the chunk size.
				a_ChunkData->chunkSize = sizeof(T);

				T* custom_chunk = reinterpret_cast<T*>(utils::add(a_ChunkData, sizeof(WaveChunkData)));
				memcpy(custom_chunk, &a_Chunk, sizeof(T));

				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
			}

			/// <summary>
			/// Returns the first chunk that shares the chunk id.
			/// </summary>
			/// <param name="a_ChunkID">The chunk id (must be a length of 4 characters).</param>
			template <class T>
			UAUDIO_WAVE_READER_RESULT GetChunkFromData(T& a_Type, const char* a_ChunkID) const
			{
				WaveChunkData* data = GetChunkFromAllocator(a_ChunkID);
				if (data != nullptr)
				{
					a_Type = T(reinterpret_cast<T*>(utils::add(data, sizeof(WaveChunkData))));
					return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
				}

				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_CHUNK_NOT_FOUND;
			}
		};
	}
}
