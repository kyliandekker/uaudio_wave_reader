#include "ChunkFilter.h"

namespace uaudio
{
	namespace wave_reader
	{
		ChunkFilter::ChunkFilter(uint16_t a_BitsPerSample)
		{
			bits_per_sample = a_BitsPerSample;
		}

		ChunkFilter::ChunkFilter(const char* a_Filter, size_t a_Size, uint16_t a_BitsPerSample)
        {
			m_Data = a_Filter;
			m_Size = a_Size;
			bits_per_sample = a_BitsPerSample;
        }
	}
}