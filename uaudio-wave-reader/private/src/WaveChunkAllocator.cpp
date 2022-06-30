#include <cassert>
#include <cstdlib>
#include <cstring>

#include "../include/WaveChunkAllocator.h"

#include "../../include/Utils.h"

namespace uaudio
{
	namespace wave_reader
	{
		WaveChunkAllocator::WaveChunkAllocator(size_t a_Size)
		{
			InitAlloc(a_Size);
		}

		WaveChunkAllocator::WaveChunkAllocator(const WaveChunkAllocator& rhs)
		{
			if (rhs.m_Start != nullptr)
			{
				InitAlloc(rhs.m_Size);
				memcpy(m_Start, rhs.m_Start, m_Size);
			}
		}

		WaveChunkAllocator::~WaveChunkAllocator()
		{
			// Free the data.
			free(m_Start);
			m_Start = nullptr;
			m_Buffer = nullptr;
		}

		WaveChunkAllocator& WaveChunkAllocator::operator=(const WaveChunkAllocator& rhs)
		{
			if (&rhs != this)
			{
				if (rhs.m_Start !=  nullptr)
				{
					InitAlloc(rhs.m_Size);
					memcpy(m_Start, rhs.m_Start, m_Size);
				}
			}

			return *this;
		}

		void* WaveChunkAllocator::Alloc(size_t a_Size)
		{
			void* current = m_Buffer;
			m_Buffer = utils::add(m_Buffer, a_Size);
			return current;
		}

		void WaveChunkAllocator::InitAlloc(size_t a_Size)
		{
			assert(a_Size != 0, "linear allocator is created with a size of 0!");
			m_Size = a_Size;
			m_Start = malloc(m_Size);
			m_Buffer = m_Start;
			m_End = utils::add(m_Start, m_Size);
		}
	}
}