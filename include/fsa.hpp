#pragma once

#include "types.hpp"
#include <cstring>
#include <iostream>

template <typename T> class FixedSizeAllocator {
  typedef u64 index_t;
  union Block {
    index_t next;
    T data;
  };

  u64 m_allocatedBlocks;
  u64 m_freeBlocks;
  u64 m_initilizedBlocks;
  u64 m_head;
  Block *m_data;

  Block operator[](index_t i) const { return m_data[i]; }

public:
  FixedSizeAllocator() {
    m_allocatedBlocks = 1 << 16;
    m_initilizedBlocks = 0;
    m_freeBlocks = m_allocatedBlocks;
    m_data = new Block[m_freeBlocks];
    m_head = 0;
  }

  ~FixedSizeAllocator() { delete m_data; }

  index_t alloc() {
    if (m_freeBlocks == 0) {
#ifdef DEBUG
      std::cout << "FSA reallocating..." << std::endl;
#endif // DEBUG

      // Double the capacity
      m_freeBlocks = m_allocatedBlocks;
      m_allocatedBlocks *= 2;
      auto new_buffer = new Block[m_allocatedBlocks];
      memcpy(new_buffer, m_data, m_allocatedBlocks);
      delete m_data;
      m_data = new_buffer;
    }

    if (m_initilizedBlocks < m_allocatedBlocks) {
      m_data[m_initilizedBlocks].next = m_initilizedBlocks + 1;
      m_initilizedBlocks++;
    }

    auto ret = m_head;
    m_head = m_data[m_head].next;
    m_freeBlocks--;

    return ret;
  }

  void free(index_t index) {
    m_data[index].next = m_head;
    m_head = index;
    m_freeBlocks++;
  }

  T &operator[](index_t i) { return m_data[i].data; }
};
