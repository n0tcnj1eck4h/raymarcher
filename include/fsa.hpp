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

  u64 m_allocated_blocks;
  u64 m_free_blocks;
  u64 m_initilized_blocks;
  u64 m_head;
  Block *m_data;

  Block operator[](index_t i) const { return m_data[i]; }

public:
  FixedSizeAllocator() {
    m_allocated_blocks = 1 << 16;
    m_initilized_blocks = 0;
    m_free_blocks = m_allocated_blocks;
    m_data = new Block[m_free_blocks];
    m_head = 0;
  }

  ~FixedSizeAllocator() { delete m_data; }

  index_t alloc() {
    if (m_free_blocks == 0) {
#ifdef DEBUG
      std::cout << "FSA reallocating..." << std::endl;
#endif // DEBUG

      // Double the capacity
      m_free_blocks = m_allocated_blocks;
      m_allocated_blocks *= 2;
      auto new_buffer = new Block[m_allocated_blocks];
      memcpy(new_buffer, m_data, m_allocated_blocks);
      delete m_data;
      m_data = new_buffer;
    }

    if (m_initilized_blocks < m_allocated_blocks) {
      m_data[m_initilized_blocks].next = m_initilized_blocks + 1;
      m_initilized_blocks++;
    }

    auto ret = m_head;
    m_head = m_data[m_head].next;
    m_free_blocks--;

    return ret;
  }

  void free(index_t index) {
    m_data[index].next = m_head;
    m_head = index;
    m_free_blocks++;
  }

  T &operator[](index_t i) { return m_data[i].data; }
};
