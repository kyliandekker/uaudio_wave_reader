#include "WaveReader.h"

namespace uaudio
{
	namespace wave_reader
	{
		Filter::~Filter()
		{
			free(m_Data);
		}

		Filter::Filter(uint16_t a_BitsPerSample)
		{
			bits_per_sample = a_BitsPerSample;
		}

		Filter::Filter(const Filter& rhs)
		{
			Assign(rhs);
		}

		Filter::Filter(std::vector<std::string> a_Filter, uint16_t a_BitsPerSample)
		{
			char* text = reinterpret_cast<char*>(malloc(a_Filter.size() * CHUNK_ID_SIZE));
			for (size_t i = 0; i < a_Filter.size(); i++)
				if (text)
					memcpy(text + (i * CHUNK_ID_SIZE), a_Filter[i].c_str(), CHUNK_ID_SIZE);

			SetData(text, a_Filter.size(), a_BitsPerSample);
		}

		Filter::Filter(std::vector<char*> a_Filter, uint16_t a_BitsPerSample)
		{
			char* text = reinterpret_cast<char*>(malloc(a_Filter.size() * CHUNK_ID_SIZE));
			for (size_t i = 0; i < a_Filter.size(); i++)
				if (text)
					memcpy(text + (i * CHUNK_ID_SIZE), a_Filter[i], CHUNK_ID_SIZE);

			SetData(text, a_Filter.size(), a_BitsPerSample);
		}

		void Filter::SetData(char* a_Data, size_t a_FilterSize, uint16_t a_BitsPerSample)
		{
			m_Data = a_Data;
			m_Size = a_FilterSize;
			bits_per_sample = a_BitsPerSample;
		}

		void Filter::Assign(const Filter& rhs)
		{
			m_Data = reinterpret_cast<char*>(malloc(rhs.m_Size * CHUNK_ID_SIZE));
			memcpy(m_Data, rhs.m_Data, rhs.m_Size * CHUNK_ID_SIZE);
			m_Size = rhs.m_Size;
			bits_per_sample = rhs.bits_per_sample;
		}
	}
}