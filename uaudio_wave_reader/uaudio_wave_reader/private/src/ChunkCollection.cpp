#include <cstdio>
#include <cstring>

#include "ChunkCollection.h"

namespace uaudio
{
	namespace wave_reader
	{
		ChunkCollection::ChunkCollection(void* a_Ptr, size_t a_Size)
		{
			assert(a_Size != 0);
			m_Size = a_Size;
			m_Start = a_Ptr;
			m_Allocated = 0;
			m_End = utils::add(m_Start, m_Size);
		}

		ChunkHeader* ChunkCollection::GetChunkFromAllocator(const char* a_ChunkID) const
		{
			assert(m_Start != nullptr);

			unsigned char* data = reinterpret_cast<unsigned char*>(m_Start);
			while (data < m_End)
			{
				ChunkHeader* wave_chunk_data = reinterpret_cast<ChunkHeader*>(data);
				if (strncmp(&a_ChunkID[0], &reinterpret_cast<char*>(wave_chunk_data->chunk_id)[0], CHUNK_ID_SIZE) == 0)
				{
					return wave_chunk_data;
				}
				data = reinterpret_cast<unsigned char*>(utils::add(data, sizeof(ChunkHeader) + wave_chunk_data->chunkSize));
			}
			return nullptr;
		}
		
		ChunkHeader* ChunkCollection::GetChunkFromAllocator(uint32_t a_Index) const
		{
			assert(m_Start != nullptr);

			unsigned char* data = reinterpret_cast<unsigned char*>(m_Start);
			uint32_t index = 0;
			while (data < m_End)
			{
				ChunkHeader* wave_chunk_data = reinterpret_cast<ChunkHeader*>(data);
				data = reinterpret_cast<unsigned char*>(utils::add(data, sizeof(ChunkHeader) + wave_chunk_data->chunkSize));
				if (index == a_Index)
					return wave_chunk_data;
				index++;
			}
			return nullptr;
		}
		
		uint32_t ChunkCollection::GetNumberOfChunks() const
		{
			assert(m_Start != nullptr);

			unsigned char* data = reinterpret_cast<unsigned char*>(m_Start);
			uint32_t index = 0;
			while (data < m_End)
			{
				ChunkHeader* wave_chunk_data = reinterpret_cast<ChunkHeader*>(data);
				data = reinterpret_cast<unsigned char*>(utils::add(data, sizeof(ChunkHeader) + wave_chunk_data->chunkSize));
				index++;
			}
			return index;
		}
		
		void* ChunkCollection::Alloc(size_t a_Size)
		{
			assert(m_Start != nullptr);
			assert(m_Size > 0);
			assert(m_Allocated + a_Size <= m_Size);
			void* current = utils::add(m_Start, m_Allocated);
			m_Allocated += a_Size;
			return current;
		}
		
		void* ChunkCollection::End() const
		{
			return m_End;
		}

        void ChunkCollection::Realloc(void* a_Buffer, size_t a_Size)
        {
			assert(a_Size > m_Size);

			memmove(a_Buffer, m_Start, a_Size);
			m_Start = a_Buffer;
			m_Size = a_Size;
        }
		
		void* ChunkCollection::data() const
		{
			return m_Start;
		}
		
		UAUDIO_WAVE_READER_RESULT ChunkCollection::GetChunkSize(uint32_t& a_Size, const char* a_ChunkID) const
		{
			a_Size = 0;
			ChunkHeader* data = GetChunkFromAllocator(a_ChunkID);
			if (data != nullptr)
			{
				a_Size = data->chunkSize;
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
			}

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_CHUNK_NOT_FOUND;
		}
		
		UAUDIO_WAVE_READER_RESULT ChunkCollection::GetNumberOfChunks(size_t& a_Size) const
		{
			a_Size = GetNumberOfChunks();
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}
		
		UAUDIO_WAVE_READER_RESULT ChunkCollection::HasChunk(bool& a_ChunkFound, const char* a_ChunkID) const
		{
			a_ChunkFound = false;

			ChunkHeader* data = GetChunkFromAllocator(a_ChunkID);
			if (data != nullptr)
			{
				a_ChunkFound = true;
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
			}

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_CHUNK_NOT_FOUND;
		}

		size_t ChunkCollection::size() const
		{
			return m_Size;
		}
	}
}