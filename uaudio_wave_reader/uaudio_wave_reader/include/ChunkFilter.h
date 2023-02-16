#pragma once

#include <cstdint>
#include <Defines.h>

namespace uaudio
{
	namespace wave_reader
	{
		/*
		 * This is a filter for the wave file loader. If you want to only get specific chunks, use this.
		*/
		class ChunkFilter
		{
		public:
			uint16_t bits_per_sample = WAVE_BITS_PER_SAMPLE_16;
			size_t size() const { return m_Size; }

			ChunkFilter()
			{ }

			ChunkFilter(uint16_t a_BitsPerSample);

			ChunkFilter(const char* a_Filter, size_t a_Size, uint16_t a_BitsPerSample = WAVE_BITS_PER_SAMPLE_16);

			const char* operator [](size_t i) const
			{
				return m_Data + (i * CHUNK_ID_SIZE);
			}
		private:
			size_t m_Size = 0;
			const char* m_Data = nullptr;
		};
	}
}