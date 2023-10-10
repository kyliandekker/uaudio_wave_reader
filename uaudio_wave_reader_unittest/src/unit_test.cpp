#include "doctest.h"

#include <complex>
#include <string>
#include <vector>
#include "uaudio_wave_reader/uaudio_wave_reader/include/WaveReader.h"
#include "uaudio_wave_reader/uaudio_wave_reader/include/Defines.h"
#include "uaudio_wave_reader/uaudio_wave_reader/include/ChunkCollection.h"
#include "uaudio_wave_reader/uaudio_wave_reader/include/WaveChunks.h"
#include "uaudio_wave_reader/uaudio_wave_reader/include/ChunkFilter.h"

constexpr auto COLOR_YELLOW = "\033[0;33m";
constexpr auto COLOR_WHITE = "\033[0;37m";
constexpr auto COLOR_GREEN = "\033[0;32m";
constexpr auto COLOR_RED = "\033[31m";
constexpr auto COLOR_CYAN = "\033[0;36m";
constexpr auto COLOR_PURPLE = "\033[0;35m";
constexpr auto COLOR_BLUE = "\033[0;34m";

void log(const char* head, const char* message)
{
	printf("%s[%s%s%s]%s %s%s.\n", COLOR_YELLOW, COLOR_CYAN, head, COLOR_YELLOW, COLOR_CYAN, message, COLOR_WHITE);
}

void log_success(const char* head, const char* message)
{
	printf("%s[%s%s%s]%s %s%s.\n", COLOR_YELLOW, COLOR_GREEN, head, COLOR_YELLOW, COLOR_GREEN, message, COLOR_WHITE);
}

TEST_CASE("Audio Loading")
{
	SUBCASE("Existing file")
	{
		SUBCASE("")
		{
			log("OPENING EXISTING VALID FILE", "Process started");
		}

		const char* chunk_filters = "datafmt factacid";

		SUBCASE("Telling filesize")
		{
			SUBCASE("16-bit (no chunks, but chunkfilter is not nullptr")
			{
				uaudio::wave_reader::WaveReadSettings settings;
				uaudio::wave_reader::ChunkFilter filters{ nullptr, 0 };
				settings.SetChunkFilter(filters);

				const char* path = "resources/16.wav";

				size_t size = 0;
				uaudio::wave_reader::UAUDIO_WAVE_READER_RESULT result = uaudio::wave_reader::WaveReader::FTell(path, size, settings);

				CHECK(!(UAUDIOWAVEREADERFAILED(result)));
				CHECK(size != 0);

				log_success("16-bit, fmt and data filter", "Done");

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size);

				CHECK(size == 564512);

				log_success("16-bit, no filter", "Done");
			}
			SUBCASE("8-bit")
			{
				uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };
				uaudio::wave_reader::WaveReadSettings settings;
				settings.SetChunkFilter(filters);

				const char* path = "resources/8.wav";

				size_t size = 0;
				uaudio::wave_reader::UAUDIO_WAVE_READER_RESULT result = uaudio::wave_reader::WaveReader::FTell(path, size, settings);

				CHECK(!(UAUDIOWAVEREADERFAILED(result)));
				CHECK(size == 282272);

				log_success("8-bit, fmt and data filter", "Done");

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size);

				CHECK(size == 282272);

				log_success("8-bit, no filter", "Done");
			}
			SUBCASE("16-bit")
			{
				uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };
				uaudio::wave_reader::WaveReadSettings settings;
				settings.SetChunkFilter(filters);

				const char* path = "resources/16.wav";

				size_t size = 0;
				uaudio::wave_reader::UAUDIO_WAVE_READER_RESULT result = uaudio::wave_reader::WaveReader::FTell(path, size, settings);

				CHECK(!(UAUDIOWAVEREADERFAILED(result)));
				CHECK(size == 564512);

				log_success("16-bit, fmt and data filter", "Done");

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size);

				CHECK(size == 564512);

				log_success("16-bit, no filter", "Done");
			}
			SUBCASE("24-bit")
			{
				uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };

				uaudio::wave_reader::WaveReadSettings settings;
				settings.SetChunkFilter(filters);

				const char* path = "resources/24.wav";

				size_t size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, settings);

				CHECK(size == 846752);

				log_success("24-bit, fmt and data filter", "Done");

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size);

				CHECK(size == 846822);

				log_success("24-bit, no filter", "Done");
			}
			SUBCASE("32-bit")
			{
				uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };

				uaudio::wave_reader::WaveReadSettings settings;
				settings.SetChunkFilter(filters);

				const char* path = "resources/32.wav";

				size_t size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, settings);

				CHECK(size == 1128992);

				log_success("32-bit, fmt and data filter", "Done");

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size);

				CHECK(size == 1128992);

				log_success("32-bit, no filter", "Done");
			}

			SUBCASE("Reading a wave file")
			{
				SUBCASE("16-bit")
				{
					uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };

					uaudio::wave_reader::WaveReadSettings settings;
					settings.SetChunkFilter(filters);

					const char* path = "resources/2 Brothers on the 4th Floor - Fairytales (Re-Style Remix).wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, settings);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, settings);

					uaudio::wave_reader::FACT_Chunk fact_chunk;
					chunkCollection.GetChunkFromData(fact_chunk, uaudio::wave_reader::FACT_CHUNK_ID);

					CHECK(fact_chunk.sample_length == 8581621);

					uaudio::wave_reader::ACID_Chunk acid_chunk;
					chunkCollection.GetChunkFromData(acid_chunk, uaudio::wave_reader::ACID_CHUNK_ID);

					CHECK(acid_chunk.tempo == 185.0f);

					uaudio::wave_reader::FMT_Chunk fmt_chunk;
					chunkCollection.GetChunkFromData(fmt_chunk, uaudio::wave_reader::FMT_CHUNK_ID);

					CHECK(fmt_chunk.audioFormat == uaudio::wave_reader::WAV_FORMAT_UNCOMPRESSED);
					CHECK(fmt_chunk.bitsPerSample == uaudio::wave_reader::WAVE_BITS_PER_SAMPLE_16);
					CHECK(fmt_chunk.numChannels == uaudio::wave_reader::WAVE_CHANNELS_STEREO);
					CHECK(fmt_chunk.sampleRate == uaudio::wave_reader::WAVE_SAMPLE_RATE_44100);
					CHECK(fmt_chunk.byteRate == 176400);
					CHECK(fmt_chunk.blockAlign == uaudio::wave_reader::BLOCK_ALIGN_16_BIT_STEREO);

					free(allocated_space);

					log_success("16-bit", "Done");
				}
				SUBCASE("24-bit")
				{
					uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };

					uaudio::wave_reader::WaveReadSettings settings;
					settings.SetChunkFilter(filters);

					const char* path = "resources/24.wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, settings);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, settings);

					uaudio::wave_reader::FMT_Chunk fmt_chunk;
					chunkCollection.GetChunkFromData(fmt_chunk, uaudio::wave_reader::FMT_CHUNK_ID);

					CHECK(fmt_chunk.audioFormat == uaudio::wave_reader::WAV_FORMAT_UNCOMPRESSED);
					CHECK(fmt_chunk.bitsPerSample == uaudio::wave_reader::WAVE_BITS_PER_SAMPLE_24);
					CHECK(fmt_chunk.numChannels == uaudio::wave_reader::WAVE_CHANNELS_STEREO);
					CHECK(fmt_chunk.sampleRate == uaudio::wave_reader::WAVE_SAMPLE_RATE_44100);
					CHECK(fmt_chunk.byteRate == 264600);
					CHECK(fmt_chunk.blockAlign == uaudio::wave_reader::BLOCK_ALIGN_24_BIT_STEREO);

					free(allocated_space);

					log_success("24-bit", "Done");
				}
				SUBCASE("32-bit")
				{
					uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 4 };

					uaudio::wave_reader::WaveReadSettings settings;
					settings.SetChunkFilter(filters);

					const char* path = "resources/32.wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, settings);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, settings);

					uaudio::wave_reader::FMT_Chunk fmt_chunk;
					chunkCollection.GetChunkFromData(fmt_chunk, uaudio::wave_reader::FMT_CHUNK_ID);

					CHECK(fmt_chunk.audioFormat == 3);
					CHECK(fmt_chunk.bitsPerSample == uaudio::wave_reader::WAVE_BITS_PER_SAMPLE_32);
					CHECK(fmt_chunk.numChannels == uaudio::wave_reader::WAVE_CHANNELS_STEREO);
					CHECK(fmt_chunk.sampleRate == uaudio::wave_reader::WAVE_SAMPLE_RATE_44100);
					CHECK(fmt_chunk.byteRate == 352800);
					CHECK(fmt_chunk.blockAlign == uaudio::wave_reader::BLOCK_ALIGN_32_BIT_STEREO);

					free(allocated_space);

					log_success("32-bit", "Done");
				}

				SUBCASE("")
				{
					log_success("OPENING EXISTING VALID FILE", "Process finished");
				}
			}
		}
	}
	SUBCASE("Saving files")
	{
		log("SAVING FILES", "Process started");

		const char* chunk_filters = "datafmt ";

		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

		uaudio::wave_reader::WaveReadSettings settings;
		settings.SetChunkFilter(filters);

		const char* path = "resources/32.wav";

		size_t size = 0;
		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

		void* allocated_space = malloc(size);
		uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

		uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, settings);

		uaudio::wave_reader::WaveReader::SaveWave("./resources/32-new.wav", chunkCollection);

		size = 0;
		uaudio::wave_reader::WaveReader::FTell("./resources/32-new.wav", size, settings);
		void* new_allocated_space = malloc(size);
		uaudio::wave_reader::ChunkCollection newChunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

		uaudio::wave_reader::WaveReader::LoadWave("./resources/32-new.wav", newChunkCollection, settings);

		free(allocated_space);
		free(new_allocated_space);

		log_success("SAVING FILES", "Process finished");
	}
	SUBCASE("Adding chunks")
	{
		log("ADDING CHUNKS", "Process started");

		const char* chunk_filters = "datafmt ";

		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

		uaudio::wave_reader::WaveReadSettings settings;
		settings.SetChunkFilter(filters);

		const char* path = "resources/32.wav";

		size_t size = 0;
		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

		void* allocated_space = malloc(size);
		uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

		CHECK(chunkCollection.size() == 1128992);

		uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, settings);

		size_t num_of_chunks = 0;
		chunkCollection.GetNumberOfChunks(num_of_chunks);
		CHECK(num_of_chunks == 2);

		size_t new_size = size + sizeof(uaudio::wave_reader::ACID_Chunk);
		chunkCollection.Realloc(malloc(size + sizeof(uaudio::wave_reader::ACID_Chunk)), new_size);
		uaudio::wave_reader::ACID_Chunk* acid_chunk = reinterpret_cast<uaudio::wave_reader::ACID_Chunk*>(chunkCollection.Alloc(sizeof(uaudio::wave_reader::ACID_Chunk), uaudio::wave_reader::ACID_CHUNK_ID));
		acid_chunk->tempo = 220;

		CHECK(chunkCollection.size() == 1129024);
		CHECK(chunkCollection.size() == new_size);
		CHECK(chunkCollection.size() == 1128992 + sizeof(uaudio::wave_reader::ACID_Chunk));
		chunkCollection.GetNumberOfChunks(num_of_chunks);
		CHECK(num_of_chunks == 3);

		free(allocated_space);

		log_success("ADDING CHUNKS", "Process finished");
	}
	SUBCASE("OPENING FILES")
	{
		const char* path = "resources/33.wav";

		size_t size = 0;
		uaudio::wave_reader::WaveReader::FTell(path, size);

		void* allocated_space = malloc(size);
		uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

		log_success("OPENING FILE THAT DOES NOT EXIST", "Process finished");
	}
	//SUBCASE("CONVERSION")
	//{
	//	const char* chunk_filters = "datafmt ";

	//	SUBCASE("8-bit to 32-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_32 = "resources/32.wav";

	//		size_t size_32 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_32, size_32, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_32_BIT);

	//		const char* path = "resources/8.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_32 == size);

	//		log_success("8-bit to 32-bit", "Done");
	//	}

	//	SUBCASE("16-bit to 32-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_32 = "resources/32.wav";

	//		size_t size_32 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_32, size_32, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_32_BIT);

	//		const char* path = "resources/16.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_32 == size);

	//		log_success("16-bit to 32-bit", "Done");
	//	}

	//	SUBCASE("24-bit to 32-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_32 = "resources/32.wav";

	//		size_t size_32 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_32, size_32, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_32_BIT);

	//		const char* path = "resources/24.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_32 == size);

	//		log_success("24-bit to 32-bit", "Done");
	//	}

	//	SUBCASE("32-bit to 24-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_24 = "resources/24.wav";

	//		size_t size_32 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_24, size_32, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_24_BIT);

	//		const char* path = "resources/32.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_32 == size);

	//		log_success("32-bit to 24-bit", "Done");
	//	}

	//	SUBCASE("8-bit to 24-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_24 = "resources/24.wav";

	//		size_t size_24 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_24, size_24, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_24_BIT);

	//		const char* path = "resources/8.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_24 == size);

	//		log_success("8-bit to 24-bit", "Done");
	//	}

	//	SUBCASE("16-bit to 24-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_24 = "resources/24.wav";

	//		size_t size_24 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_24, size_24, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_24_BIT);

	//		const char* path = "resources/16.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_24 == size);

	//		log_success("16-bit to 24-bit", "Done");
	//	}

	//	SUBCASE("32-bit to 16-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_16 = "resources/16.wav";

	//		size_t size_16 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_16, size_16, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_16_BIT);

	//		const char* path = "resources/32.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_16 == size);

	//		log_success("32-bit to 16-bit", "Done");
	//	}

	//	SUBCASE("24-bit to 16-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_16 = "resources/16.wav";

	//		size_t size_16 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_16, size_16, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_16_BIT);

	//		const char* path = "resources/24.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_16 == size);

	//		log_success("24-bit to 16-bit", "Done");
	//	}

	//	SUBCASE("8-bit to 16-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_16 = "resources/16.wav";

	//		size_t size_16 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_16, size_16, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_16_BIT);

	//		const char* path = "resources/8.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_16 == size);

	//		log_success("8-bit to 16-bit", "Done");
	//	}

	//	SUBCASE("32-bit to 8-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_8 = "resources/8.wav";

	//		size_t size_8 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_8, size_8, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_8_BIT);

	//		const char* path = "resources/32.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_8 == size);

	//		log_success("32-bit to 8-bit", "Done");
	//	}

	//	SUBCASE("24-bit to 8-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_8 = "resources/8.wav";

	//		size_t size_8 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_8, size_8, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_8_BIT);

	//		const char* path = "resources/24.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_8 == size);

	//		log_success("24-bit to 16-bit", "Done");
	//	}

	//	SUBCASE("16-bit to 8-bit")
	//	{
	//		uaudio::wave_reader::ChunkFilter filters{ chunk_filters, 2 };

	//		uaudio::wave_reader::WaveReadSettings settings;
	//		settings.SetChunkFilter(filters);

	//		const char* path_8 = "resources/8.wav";

	//		size_t size_8 = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path_8, size_8, settings);

	//		settings.SetBitsPerSample(uaudio::wave_reader::BPSConversionSettings::CONVERSION_8_BIT);

	//		const char* path = "resources/16.wav";

	//		size_t size = 0;
	//		uaudio::wave_reader::WaveReader::FTell(path, size, settings);

	//		CHECK(size_8 == size);

	//		log_success("16-bit to 16-bit", "Done");
	//	}

	//	SUBCASE("")
	//	{
	//		log_success("CONVERSION", "Process finished");
	//	}
	//}
}