#include "../../include/WaveFile.h"
#include "../include/WaveChunkAllocator.h"

namespace uaudio
{
	namespace wave_reader
	{
		/// <summary>
		/// Retrieves a chunk with a specific chunk id from the loaded chunks.
		/// </summary>
		/// <param name="a_ChunkID">The chunk id (must be a length of 4 characters).</param>
		/// <returns>The chunk on success, nullptr on failure.</returns>
		WaveChunkData* WaveFile::GetChunkFromAllocator(const char* a_ChunkID) const
		{
			unsigned char* data = reinterpret_cast<unsigned char*>(m_Chunks->Start());
			while (data < m_Chunks->End())
			{
				WaveChunkData* wave_chunk_data = reinterpret_cast<WaveChunkData*>(data);
				if (strncmp(&a_ChunkID[0], &reinterpret_cast<char*>(wave_chunk_data->chunk_id)[0], CHUNK_ID_SIZE) == 0)
				{
					return wave_chunk_data;
				}
				data = reinterpret_cast<unsigned char*>(utils::add(data, wave_chunk_data->chunkSize));
			}
			return nullptr;
		}

		WaveFile::~WaveFile()
		{
			delete m_Chunks;
		}

		/// <summary>
		/// Gets the chunk size of the first chunk that shares the chunk id.
		/// </summary>
		/// <param name="a_ChunkID">The chunk id (must be a length of 4 characters).</param>
		/// <returns>UAudio Result.</returns>
		UAUDIO_WAVE_READER_RESULT WaveFile::GetChunkSize(uint32_t& a_Size, const char* a_ChunkID) const
		{
			a_Size = 0;
			WaveChunkData* data = GetChunkFromAllocator(a_ChunkID);
			if (data != nullptr)
			{
				a_Size = data->chunkSize;
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
			}

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_CHUNK_NOT_FOUND;
		}

		/// <summary>
		/// Returns the number of loaded chunks.
		/// </summary>
		/// <param name="a_Size"></param>
		/// <returns>UAudio Result.</returns>
		UAUDIO_WAVE_READER_RESULT WaveFile::GetNumberOfChunks(size_t& a_Size) const
		{
			a_Size = 0;
			unsigned char* data = reinterpret_cast<unsigned char*>(m_Chunks->Start());
			while (data < m_Chunks->End())
			{
				WaveChunkData* wave_chunk_data = reinterpret_cast<WaveChunkData*>(data);

				a_Size++;
				data = reinterpret_cast<unsigned char*>(utils::add(data, wave_chunk_data->chunkSize));
			}
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
		}

		/// <summary>
		/// Checks whether a chunk that shares the chunk id exists.
		/// </summary>
		/// <param name="a_ChunkID">The chunk id (must be a length of 4 characters).</param>
		/// <returns>UAudio Result.</returns>
		UAUDIO_WAVE_READER_RESULT WaveFile::HasChunk(bool& a_ChunkFound, const char* a_ChunkID) const
		{
			a_ChunkFound = false;

			WaveChunkData* data = GetChunkFromAllocator(a_ChunkID);
			if (data != nullptr)
			{
				a_ChunkFound = true;
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
			}

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_CHUNK_NOT_FOUND;
		}
	}
}
