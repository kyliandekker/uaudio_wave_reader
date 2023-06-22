#pragma once

#include <cstdint>
#include "UAUDIO_WAVE_READER_CONVERSION_RESULT.h"

namespace uaudio
{
	namespace wave_reader
	{
		namespace conversion
		{
			uint32_t Calculate24To16Size(uint32_t a_Size);
			uint32_t Calculate32To16Size(uint32_t a_Size);

			uint32_t Calculate24To32Size(uint32_t a_Size);
			uint32_t Calculate16To32Size(uint32_t a_Size);

			uint32_t Calculate16To24Size(uint32_t a_Size);
			uint32_t Calculate32To24Size(uint32_t a_Size);

			uint32_t CalculateMonoToStereoSize(uint32_t a_Size);
			uint32_t CalculateStereoToMonoSize(uint32_t a_Size);

			UAUDIO_WAVE_READER_CONVERSION_RESULT Convert24To16(unsigned char* a_DataBuffer, unsigned char* a_OriginalDataBuffer, uint32_t& a_Size);
			UAUDIO_WAVE_READER_CONVERSION_RESULT Convert32To16(unsigned char* a_DataBuffer, unsigned char* a_OriginalDataBuffer, uint32_t& a_Size);
			UAUDIO_WAVE_READER_CONVERSION_RESULT ConvertMonoToStereo(unsigned char* a_DataBuffer, const unsigned char* a_OriginalDataBuffer, uint32_t& a_Size, uint16_t a_BlockAlign);
			UAUDIO_WAVE_READER_CONVERSION_RESULT ConvertStereoToMono(unsigned char* a_DataBuffer, const unsigned char* a_OriginalDataBuffer, uint32_t& a_Size, uint16_t a_BlockAlign);

			//UAUDIO_RESULT ConvertToSamples(float* a_OutSamples, unsigned char* a_DataBuffer, uint32_t a_SampleCount);

			UAUDIO_WAVE_READER_CONVERSION_RESULT CalculateBufferSize(uint32_t a_StartingPoint, uint32_t a_ElementCount, uint32_t a_ChunkSize, uint32_t& a_BufferSize);
			UAUDIO_WAVE_READER_CONVERSION_RESULT ReadDataBuffer(uint32_t a_StartingPoint, const uint32_t& a_ElementCount, unsigned char* a_DataBuffer, unsigned char* a_OriginalDataBuffer);
		}
	}
}