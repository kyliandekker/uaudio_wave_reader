#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

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
			std::string_view* filter;
			size_t filter_size;

			Filter()
			{
				
			}

			Filter(std::string_view* a_Filter, size_t a_FilterSize)
			{
				filter = a_Filter;
				filter_size = a_FilterSize;
			}
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