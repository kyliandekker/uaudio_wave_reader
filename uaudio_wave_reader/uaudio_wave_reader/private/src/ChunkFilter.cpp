#include "ChunkFilter.h"

namespace uaudio
{
	namespace wave_reader
	{
		ChunkFilter::ChunkFilter(const char* a_Filter, size_t a_Size)
        {
			m_Data = a_Filter;
			m_Size = a_Size;
        }
	}
}