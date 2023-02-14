#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "./UAUDIO_WAVE_READER_RESULT.h"
#include "./ChunkCollection.h"

namespace uaudio
{
	namespace wave_reader
	{
		/*
		 * This is a filter for the wave file loader. If you want to only get specific chunks, use this.
		*/
		class Filter
		{
		public:
			uint16_t bits_per_sample = WAVE_BITS_PER_SAMPLE_16;
			size_t Size() const { return m_Size; }

			~Filter();

			Filter()
			{ }

			Filter(uint16_t a_BitsPerSample);

			Filter(const Filter& rhs);
			
			Filter& operator=(const Filter& rhs)
			{
				if (this != &rhs)
					Assign(rhs);
				return *this;
			}

			Filter(std::vector<std::string> a_Filter, uint16_t a_BitsPerSample = WAVE_BITS_PER_SAMPLE_16);

			Filter(std::vector<char*> a_Filter, uint16_t a_BitsPerSample = WAVE_BITS_PER_SAMPLE_16);

			char* operator [](size_t i) const
			{
				return m_Data + (i * CHUNK_ID_SIZE);
			}
		private:
			void SetData(char* a_Data, size_t a_FilterSize, uint16_t a_BitsPerSample);
			void Assign(const Filter& rhs);

			size_t m_Size = 0;
			char* m_Data = nullptr;
		};

		/*
		 * This is the wave reader. It is responsible for loading the chunks of a wave file and creating a ChunkCollection.
		 * It is also responsible for saving wave files.
		*/
		class WaveReader
		{
		public:
			static UAUDIO_WAVE_READER_RESULT FTell(const char* a_FilePath, size_t& a_Size, Filter a_Filter = Filter());
			static UAUDIO_WAVE_READER_RESULT LoadWave(const char* a_FilePath, ChunkCollection& a_ChunkCollection, Filter a_Filter = Filter());
			static UAUDIO_WAVE_READER_RESULT SaveWave(const char* a_FilePath, const ChunkCollection& a_ChunkCollection);
		private:
			static UAUDIO_WAVE_READER_RESULT OpenWave(const char* a_FilePath, FILE*& a_File, long& a_FileSize);
			static UAUDIO_WAVE_READER_RESULT LoadChunk(const char* a_FilePath, FILE* a_File, long& a_PreviousTell, char* a_PrevChunkID, char* a_ChunkID, uint32_t& a_ChunkSize);
		};
	}
}