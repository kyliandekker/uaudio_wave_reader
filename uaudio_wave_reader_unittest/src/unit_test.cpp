#include "doctest.h"

#include <complex>
#include <WaveReader.h>
#include <Defines.h>
#include <WaveChunks.h>

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
		SUBCASE("16-bit")
		{
			log("OPENING EXISTING VALID FILE", "Process started");
		}

		std::string_view chunk_filters[] =
		{
			"data",
			"fmt ",
			"fact",
			"acid",
		};

		const uaudio::wave_reader::Filter filters{ chunk_filters, std::size(chunk_filters) };

		SUBCASE("Telling filesize")
		{
			SUBCASE("16-bit")
			{
				const char* path = "resources/16.wav";

				size_t size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, filters);

				CHECK(size == 564512);

				log_success("16-bit, fmt and data filter", "Done");

				const uaudio::wave_reader::Filter bigger_filters{};

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, bigger_filters);

				CHECK(size == 564582);

				log_success("16-bit, no filter", "Done");
			}
			SUBCASE("24-bit")
			{
				const char* path = "resources/24.wav";

				size_t size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, filters);

				CHECK(size == 846752);

				log_success("24-bit, fmt and data filter", "Done");

				const uaudio::wave_reader::Filter bigger_filters{};

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, bigger_filters);

				CHECK(size == 846822);

				log_success("24-bit, no filter", "Done");
			}
			SUBCASE("32-bit")
			{
				const char* path = "resources/32.wav";

				size_t size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, filters);

				CHECK(size == 1128992);

				log_success("32-bit, fmt and data filter", "Done");

				const uaudio::wave_reader::Filter bigger_filters{};

				size = 0;
				uaudio::wave_reader::WaveReader::FTell(path, size, bigger_filters);

				CHECK(size == 1129062);

				log_success("32-bit, no filter", "Done");
			}

			SUBCASE("Reading a wave file")
			{
				SUBCASE("16-bit")
				{
					const char* path = "resources/2 Brothers on the 4th Floor - Fairytales (Re-Style Remix).wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, filters);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, filters);

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
					const char* path = "resources/24.wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, filters);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, filters);

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
					const char* path = "resources/32.wav";

					size_t size = 0;
					uaudio::wave_reader::WaveReader::FTell(path, size, filters);

					void* allocated_space = malloc(size);
					uaudio::wave_reader::ChunkCollection chunkCollection = uaudio::wave_reader::ChunkCollection(allocated_space, size);

					uaudio::wave_reader::WaveReader::LoadWave(path, chunkCollection, filters);

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

				SUBCASE("16-bit")
				{
					log_success("OPENING EXISTING VALID FILE", "Process finished");
				}
			}
		}
	}
}