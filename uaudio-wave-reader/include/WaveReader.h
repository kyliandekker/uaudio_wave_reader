#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>

#include "./UAUDIO_WAVE_READER_RESULT.h"
#include "./ChunkCollection.h"

namespace uaudio
{
	namespace wave_reader
	{
		/*
		 * WHAT IS THIS FILE?
		 * This is the wave reader. It is responsible for loading the chunks of a wave file and creating a ChunkCollection.
		 * It is also responsible for saving wave files.
		*/
		struct WaveInfo
		{
			void* ptr;
			size_t size;
		};

		class WaveReader
		{
		public:
			static UAUDIO_WAVE_READER_RESULT GetSoundSize(const char* a_FilePath, size_t& a_Size);
			static UAUDIO_WAVE_READER_RESULT LoadSound(const char* a_FilePath, ChunkCollection& a_ChunkCollection, WaveInfo a_WaveInfo);
			static UAUDIO_WAVE_READER_RESULT SaveSound(const char* a_FilePath, const ChunkCollection& a_ChunkCollection);
		private:
			static UAUDIO_WAVE_READER_RESULT LoadChunk(const char* a_FilePath, FILE* a_File, long& a_PreviousTell, char* a_PrevChunkID, char* a_ChunkID, uint32_t& a_ChunkSize);
		};
	}
}