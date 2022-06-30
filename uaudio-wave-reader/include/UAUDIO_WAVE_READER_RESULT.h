#pragma once

namespace uaudio
{
	namespace wave_reader
	{
		enum class UAUDIO_WAVE_READER_RESULT
		{
			UAUDIO_WAVE_READER_OK,
			UAUDIO_WAVE_READER_ERR_CHUNK_NOT_FOUND,
			UAUDIO_WAVE_READER_ERR_FILE_BAD,
			UAUDIO_WAVE_READER_ERR_FILE_NOT_FOUND,
		};
	}
}