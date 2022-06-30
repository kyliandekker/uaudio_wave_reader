#pragma once

#include <cstdint>
#include <cstdio>

#include "./UAUDIO_WAVE_READER_RESULT.h"

namespace uaudio
{
	namespace wave_reader
	{
		class WaveFile;

		/*
		 * WHAT IS THIS FILE?
		 * This is the wave reader. It is responsible for loading the chunks of a wave file and creating a WaveFormat.
		 * It is also responsible for saving wave files.
		 * It uses the WaveConfig to determine which chunks need to be stored into memory.
		 */
		class WaveReader
		{
		public:
			static UAUDIO_WAVE_READER_RESULT LoadSound(const char* a_FilePath, WaveFile& a_WaveFile);
			static UAUDIO_WAVE_READER_RESULT SaveSound(const char* a_FilePath, const WaveFile& a_WaveFile);
		private:
			static UAUDIO_WAVE_READER_RESULT LoadChunk(const char* a_FilePath, FILE* a_File, long& a_PreviousTell, char* a_PrevChunkID, char* a_ChunkID, uint32_t& a_ChunkSize);
		};
	}
}
