#include <cstdio>
#include <cstring>

#include "WaveReader.h"
#include "WaveChunks.h"
#include "PrivateDefines.h"

namespace uaudio
{
	namespace wave_reader
	{
		UAUDIO_WAVE_READER_RESULT WaveReader::FTell(const char* a_FilePath, size_t& a_Size, Filter a_Filter)
		{
			FILE* file = nullptr;

			long file_size = 0;
			UAUDIO_WAVE_READER_RESULT result = OpenWave(a_FilePath, file, file_size);
			if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_OK)
				return result;

			char previous_chunk_id[CHUNK_ID_SIZE] = {};
			long previous_tell = 0;

			// First we go through the file to get the size of all the chosen chunks.
			// Check if file has reached eof.
			while (file_size != ftell(file))
			{
				char chunk_id[CHUNK_ID_SIZE] = {};
				uint32_t chunk_size = 0;

				// Load the chunk.
				result = LoadChunk(a_FilePath, file, previous_tell, &previous_chunk_id[0], &chunk_id[0], chunk_size);
				if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_OK)
					break;

				// Check if it is the riff chunk, if it is, don't allocate.
				if (strncmp(&chunk_id[0], RIFF_CHUNK_ID, CHUNK_ID_SIZE) == 0)
				{
					fseek(file, sizeof(RIFF_Chunk) - sizeof(ChunkHeader), SEEK_CUR);
					continue;
				}

				// Copy for the fail-safe above.
				memcpy(previous_chunk_id, chunk_id, sizeof(chunk_id));

				bool get_chunk = false;
				for (size_t i = 0; i < a_Filter.filter_size; i++)
					if (strncmp(&chunk_id[0], a_Filter.filter[i].data(), CHUNK_ID_SIZE) == 0)
						get_chunk = true;

				if (a_Filter.filter_size == 0)
					get_chunk = true;

				if (get_chunk)
				{
					printf("<WaveReader> Found %s\"%.4s\"%s chunk with size %s\"%i\"%s.\n", COLOR_YELLOW, chunk_id, COLOR_WHITE, COLOR_YELLOW, chunk_size, COLOR_WHITE);
					a_Size += chunk_size;
					a_Size += sizeof(ChunkHeader);
				}
				else
					printf("<WaveReader> Found %s\"%.4s\"%s chunk with size %s\"%i\"%s (not in filter).\n", COLOR_YELLOW, chunk_id, COLOR_WHITE, COLOR_YELLOW, chunk_size, COLOR_WHITE);
				fseek(file, static_cast<long>(chunk_size), SEEK_CUR);
			}

			fclose(file);
			file = nullptr;

			printf("<WaveReader> Loaded file successfully: (%s\"%s\"%s).\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			printf("<WaveReader> Wave file (%s\"%s%s\") has a size of %s%zu%s.\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE, COLOR_YELLOW, a_Size, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}

		/// <summary>
		/// Loads the sound.
		/// </summary>
		/// <param name="a_FilePath">The path to the file.</param>
		/// <param name="a_ChunkCollection">The chunk collection.</param>
		/// <returns>WAVE loading status.</returns>
		UAUDIO_WAVE_READER_RESULT WaveReader::LoadWave(const char* a_FilePath, ChunkCollection& a_ChunkCollection, Filter a_Filter)
		{
			FILE* file = nullptr;

			long file_size = 0;
			UAUDIO_WAVE_READER_RESULT result = OpenWave(a_FilePath, file, file_size);
			if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_OK)
				return result;

			char previous_chunk_id[CHUNK_ID_SIZE] = {};
			long previous_tell = 0;

			// Set the actual data of the now allocated chunks.
			// Check if file has reached eof.
			while (file_size != ftell(file))
			{
				char chunk_id[CHUNK_ID_SIZE] = {};
				uint32_t chunk_size = 0;

				// Load the chunk.
				result = LoadChunk(a_FilePath, file, previous_tell, &previous_chunk_id[0], &chunk_id[0], chunk_size);
				if (result != UAUDIO_WAVE_READER_RESULT::UAUDIO_OK)
					break;

				// Check if it is the riff chunk, if it is, don't allocate.
				if (strncmp(&chunk_id[0], RIFF_CHUNK_ID, CHUNK_ID_SIZE) == 0)
				{
					fseek(file, sizeof(RIFF_Chunk) - sizeof(ChunkHeader), SEEK_CUR);
					continue;
				}

				// Copy for the fail-safe above.
				memcpy(previous_chunk_id, chunk_id, sizeof(chunk_id));

				bool get_chunk = false;
				for (size_t i = 0; i < a_Filter.filter_size; i++)
					if (strncmp(&chunk_id[0], a_Filter.filter[i].data(), CHUNK_ID_SIZE) == 0)
						get_chunk = true;

				if (a_Filter.filter_size == 0)
					get_chunk = true;

				if (get_chunk)
				{
					printf("<WaveReader> Found %s\"%.4s\"%s chunk with size %s\"%i\"%s.\n", COLOR_YELLOW, chunk_id, COLOR_WHITE, COLOR_YELLOW, chunk_size, COLOR_WHITE);
					ChunkHeader* chunk_data = reinterpret_cast<ChunkHeader*>(a_ChunkCollection.Alloc(chunk_size + sizeof(ChunkHeader)));

					if (chunk_data != nullptr)
					{
						memcpy(chunk_data->chunk_id, chunk_id, sizeof(chunk_id));
						chunk_data->chunkSize = chunk_size;
						fread(utils::add(chunk_data, sizeof(ChunkHeader)), 1, chunk_size, file);
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

			printf("<WaveReader> Loaded file successfully: (%s\"%s\"%s).\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}

		/// <summary>
		/// Saves a file with the chunks inside the wave format.
		/// </summary>
		/// <param name="a_FilePath">The path to save to.</param>
		/// <param name="a_ChunkCollection">The chunk collection.</param>
		/// <returns>WAVE saving status.</returns>
		UAUDIO_WAVE_READER_RESULT WaveReader::SaveWave(const char* a_FilePath, const ChunkCollection& a_ChunkCollection)
		{
			FILE* file  = nullptr;

			// Open the file.
			fopen_s(&file, a_FilePath, "wb");
			if (file == nullptr)
			{
				printf("<WaveReader> Failed saving file: (%s\"%s%s\").", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_FILE_BAD;
			}

			// Calculate total file size (minus riff chunk header).
			const uint32_t size = sizeof(RIFF_Chunk) + static_cast<uint32_t>(a_ChunkCollection.GetSize());

			// Construct riff chunk.
			fwrite(RIFF_CHUNK_ID, CHUNK_ID_SIZE, 1, file);
			fwrite(&size, sizeof(size), 1, file);
			fwrite(RIFF_CHUNK_FORMAT, CHUNK_ID_SIZE, 1, file);

			// Write the chunks.
			fwrite(a_ChunkCollection.Start(), a_ChunkCollection.GetSize(), 1, file);

			fclose(file);
			file = nullptr;

			printf("<WaveReader> Saved file successfully: (%s\"%s%s\").\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}

		/// <summary>
		/// Opens a file.
		/// </summary>
		/// <param name="a_FilePath">The path to the file.</param>
		/// <param name="a_File">The file pointer.</param>
		/// <param name="a_FileSize">The file size.</param>
		/// <returns></returns>
		UAUDIO_WAVE_READER_RESULT WaveReader::OpenWave(const char* a_FilePath, FILE*& a_File, long& a_FileSize)
		{
			// Check if the file that has been passed is opened in some way.
			if (a_File != nullptr)
			{
				fclose(a_File);
				a_File = nullptr;
			}

			// Open the file.
			fopen_s(&a_File, a_FilePath, "rb");
			if (a_File == nullptr)
			{
				printf("<WaveReader> Cannot find file: (%s\"%s%s\").\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
				return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_FILE_NOTFOUND;
			}

			// Go to eof and store the size.
			fseek(a_File, 0, SEEK_END);
			a_FileSize = ftell(a_File);
			rewind(a_File);

			printf("<WaveReader> Opened file successfully: (%s\"%s\"%s).\n", COLOR_YELLOW, a_FilePath, COLOR_WHITE);
			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}

		UAUDIO_WAVE_READER_RESULT WaveReader::LoadChunk(const char*, FILE* a_File, long& a_PreviousTell, char* a_PrevChunkID, char* a_ChunkID, uint32_t& a_ChunkSize)
		{
			char chunk_id[CHUNK_ID_SIZE] = {};

			// Store chunk id.
			fread(&chunk_id, sizeof(chunk_id), 1, a_File);

			memcpy(a_ChunkID, chunk_id, sizeof(chunk_id));

			// Fail-safe for if the algorithm is stuck with a specific chunk. It gives up at second try.
			if (a_PreviousTell == ftell(a_File))
			{
				// Set the current tell.
				a_PreviousTell = ftell(a_File);

				memcpy(a_PrevChunkID, a_ChunkID, sizeof(chunk_id));

				return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_FILE_BAD;
			}

			// Fail-safe for if the algorithm is stuck with a specific chunk. It gives up at second try.
			if (strncmp(a_ChunkID, a_PrevChunkID, CHUNK_ID_SIZE) == 0)
			{
				// Set the current tell.
				a_PreviousTell = ftell(a_File);

				memcpy(a_PrevChunkID, a_ChunkID, sizeof(chunk_id));

				return UAUDIO_WAVE_READER_RESULT::UAUDIO_ERR_FILE_BAD;
			}

			// Store chunk size.
			fread(&a_ChunkSize, sizeof(a_ChunkSize), 1, a_File);

			// Set the current tell.
			a_PreviousTell = ftell(a_File);

			return UAUDIO_WAVE_READER_RESULT::UAUDIO_OK;
		}
	}
}
