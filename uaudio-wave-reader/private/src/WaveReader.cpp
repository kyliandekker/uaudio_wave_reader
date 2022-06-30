#include <cstdio>
#include <cstring>

#include "../../include/WaveReader.h"
#include "../../include/WaveFile.h"
#include "../../include/WaveChunks.h"
#include "../include/WaveChunkAllocator.h"
#include "../include/PrivateDefines.h"
#include "../include/WaveChunkAllocator.h"

namespace uaudio
{
	namespace wave_reader
	{
		/// <summary>
		/// Loads the sound.
		/// </summary>
		/// <param name="a_FilePath">The path to the file.</param>
		/// <param name="a_WaveFile">The wave format.</param>
		/// <returns>WAVE loading status.</returns>
		UAUDIO_WAVE_READER_RESULT WaveReader::LoadSound(const char* a_FilePath, WaveFile& a_WaveFile)
		{
			FILE* file = nullptr;

			// Empty the wave format.
			a_WaveFile = WaveFile();

			// Check if the file that has been passed is opened in some way.
			if (file != nullptr)
			{
				fclose(file);
				file = nullptr;
			}

			// Open the file.
			fopen_s(&file, a_FilePath, "rb");
			if (file == nullptr)
			{
				printf("<WaveReader> Failed opening file: (%s\"%s%s\").\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_FILE_NOT_FOUND;
			}

			// Go to eof and store the size.
			fseek(file, 0, SEEK_END);
			const long total_size = ftell(file);
			rewind(file);

			printf("<WaveReader> Reading wave file: (%s\"%s\"%s).\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);

			// Set allocation size to 0.
			size_t allocation_size = 0;

			char previous_chunk_id[CHUNK_ID_SIZE] = {};
			long previous_tell = 0;

			// First we go through the file to get the size of all the chosen chunks.
			// Check if file has reached eof.
			while (total_size != ftell(file))
			{
				char chunk_id[CHUNK_ID_SIZE] = {};
				uint32_t chunk_size = 0;

				// Load the chunk.
				UAUDIO_WAVE_READER_RESULT result = LoadChunk(a_FilePath, file, previous_tell, &previous_chunk_id[0], &chunk_id[0], chunk_size);
				if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK)
					return result;

				// Check if it is the riff chunk, if it is, don't allocate.
				if (strncmp(&chunk_id[0], RIFF_CHUNK_ID, CHUNK_ID_SIZE) == 0)
				{
					fseek(file, CHUNK_ID_SIZE, SEEK_CUR);
					continue;
				}

				// Copy for the fail-safe above.
				memcpy(previous_chunk_id, chunk_id, sizeof(chunk_id));

				bool get_chunk = true;

				// Check if the chunk is in the config.
				//for (const auto& chunk_name : a_WaveConfig.chunksToLoad)
				//	if (strncmp(&chunk_id[0], chunk_name, CHUNK_ID_SIZE) == 0)
				//		get_chunk = true;

				if (get_chunk)
				{
					allocation_size += chunk_size;
					allocation_size += sizeof(WaveChunkData);
					fseek(file, static_cast<long>(chunk_size), SEEK_CUR);
				}
			}

			// Allocate for chunks.
			a_WaveFile.m_Chunks = new WaveChunkAllocator(allocation_size);

			rewind(file);
			previous_tell = 0;

			// Set the actual data of the now allocated chunks.
			// Check if file has reached eof.
			while (total_size != ftell(file))
			{
				char chunk_id[CHUNK_ID_SIZE] = {};
				uint32_t chunk_size = 0;

				// Load the chunk.
				UAUDIO_WAVE_READER_RESULT result = LoadChunk(a_FilePath, file, previous_tell, &previous_chunk_id[0], &chunk_id[0], chunk_size);
				if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK)
					return result;

				// Check if it is the riff chunk, if it is, don't allocate.
				if (strncmp(&chunk_id[0], RIFF_CHUNK_ID, CHUNK_ID_SIZE) == 0)
				{
					printf("<WaveReader> Found %s\"%.4s\"%s chunk.\n", COLOR_YELLOW, RIFF_CHUNK_ID, COLOR_WHITE);
					fseek(file, 4, SEEK_CUR);
					continue;
				}

				bool get_chunk = true;

				if (get_chunk)
				{
					printf("<WaveReader> Found %s\"%.4s\"%s chunk with size %s\"%i\"%s.\n", COLOR_YELLOW, chunk_id, COLOR_WHITE, COLOR_YELLOW, chunk_size, COLOR_WHITE);
					WaveChunkData* chunk_data = reinterpret_cast<WaveChunkData*>(a_WaveFile.m_Chunks->Alloc(chunk_size + sizeof(WaveChunkData)));

					if (chunk_data != nullptr)
					{
						memcpy(chunk_data->chunk_id, chunk_id, sizeof(chunk_id));
						chunk_data->chunkSize = chunk_size;
						fread(utils::add(chunk_data, sizeof(WaveChunkData)), 1, chunk_size, file);
					}
					else
						fseek(file, static_cast<long>(chunk_size), SEEK_CUR);
				}
				else
				{
					printf("<WaveReader> Found %s\"%.4s\"%s chunk with size %s\"%i\"%s that is not in config.\n", COLOR_YELLOW, chunk_id, COLOR_WHITE, COLOR_YELLOW, chunk_size, COLOR_WHITE);
					fseek(file, static_cast<long>(chunk_size), SEEK_CUR);
				}
			}

			fclose(file);
			file = nullptr;

			printf("<WaveReader> Opened file successfully: (%s\"%s\"%s).\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
		}

		/// <summary>
		/// Saves a file with the chunks inside the wave format.
		/// </summary>
		/// <param name="a_FilePath">The path to save to.</param>
		/// <param name="a_WaveFile">The format with all the chunks.</param>
		/// <returns>WAVE saving status.</returns>
		UAUDIO_WAVE_READER_RESULT WaveReader::SaveSound(const char* a_FilePath, const WaveFile& a_WaveFile)
		{
			FILE* file;

			// Open the file.
			fopen_s(&file, a_FilePath, "wb");
			if (file == nullptr)
			{
				printf("<WaveReader> Failed saving file: (%s\"%s%s\").", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_FILE_BAD;
			}

			WaveChunkData* chunk_data = nullptr;
			RIFF_Chunk riff_chunk;
			memcpy(riff_chunk.format, FMT_CHUNK_FORMAT, CHUNK_ID_SIZE);
			WaveFile::ConstructChunk(chunk_data, RIFF_CHUNK_ID, riff_chunk);

			chunk_data->chunkSize = sizeof(RIFF_Chunk) + static_cast<uint32_t>(a_WaveFile.m_Chunks->Size());

			fwrite(chunk_data, sizeof(WaveChunkData) + sizeof(RIFF_Chunk), 1, file);

			fwrite(a_WaveFile.m_Chunks->Start(), a_WaveFile.m_Chunks->Size(), 1, file);

			fclose(file);
			file = nullptr;

			printf("<WaveReader> Saved file: (%s\"%s%s\")\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
		}

		UAUDIO_WAVE_READER_RESULT WaveReader::LoadChunk(const char* a_FilePath, FILE* a_File, long& a_PreviousTell, char* a_PrevChunkID, char* a_ChunkID, uint32_t& a_ChunkSize)
		{
			char chunk_id[CHUNK_ID_SIZE] = {};

			// Store chunk id.
			fread(&chunk_id, sizeof(chunk_id), 1, a_File);

			memcpy(a_ChunkID, chunk_id, sizeof(chunk_id));

			// Fail-safe for if the algorithm is stuck with a specific chunk. It gives up at second try.
			if (a_PreviousTell == ftell(a_File))
			{
				printf("<WaveReader> Failed to load wave file (%s\"%s\"%s): Got stuck on %s\"%.4s\"%s and %s\"%.4s\"%s chunks.\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE, COLOR_YELLOW, a_ChunkID, COLOR_WHITE, COLOR_YELLOW, a_PrevChunkID, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_FILE_BAD;
			}

			// Fail-safe for if the algorithm is stuck with a specific chunk. It gives up at second try.
			if (strncmp(a_ChunkID, a_PrevChunkID, CHUNK_ID_SIZE) == 0)
			{
				printf("<WaveReader> Failed to load wave file (%s\"%s\"%s): Got stuck on %s\"%.4s\"%s and %s\"%.4s\"%s chunks.\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE, COLOR_YELLOW, a_ChunkID, COLOR_WHITE, COLOR_YELLOW, a_PrevChunkID, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_ERR_FILE_BAD;
			}

			memcpy(a_PrevChunkID, a_ChunkID, sizeof(chunk_id));

			// Store chunk size.
			fread(&a_ChunkSize, sizeof(a_ChunkSize), 1, a_File);

			// Set the current tell.
			a_PreviousTell = ftell(a_File);

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_WAVE_READER_OK;
		}
	}
}
