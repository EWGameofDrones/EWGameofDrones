#ifndef __BINARY_H
#define __BINARY_H

#include <stdint.h>
#include <string.h>

struct Buffer {
  /* Wrap a buffer, containing the given number of
   * data bytes and a given remaining capacity (so total capacity is len
   * + capacity). */
  Buffer(uint8_t *buf, size_t len, size_t capacity = 0) : head(buf), tail(buf + len), end(tail + capacity) { }

  uint8_t *head;
  uint8_t *tail;
  uint8_t *end;

  /**
   * Appends the given value to the end of this buffer and returns true.
   *
   * If this would write past the end of the available memory (end),
   * nothing happens and false is returned.
   */
  template <typename T>
  bool append(const T &v) {
    if (tail + sizeof(T) <= end) {
      memcpy(tail, (const uint8_t*)&v, sizeof(T));
      tail += sizeof(T);
      return true;
    }
    return false;
  }

  /**
   * Gets a copy of the value of the given type, on the given offset
   * into the data in this buffer.
   *
   * If this would read past the end of the data in this buffer (tail),
   * an all-zeroes value is returned instead.
   */
  template <typename T>
  T get(size_t offset) {
    T res;
    if (head + offset + sizeof(T) <= tail)
      memcpy((uint8_t *)&res, head + offset, sizeof(T));
    else
      memset((uint8_t *)&res, 0, sizeof(T));
    return res;
  }

   /*
   * Gets a pointer into this buffer, starting from the given offset
   * within the data.
   *
   * If this would read past the end of the data in this buffer (tail),
   * NULL is returned instead.
   */
  uint8_t *get(size_t offset) {
    if (head + offset < tail)
      return head + offset;
    return NULL;
  }

  /*
   * Remove one value of the given type from the data and return it.
   *
   * If this would read past the end of the data in this buffer (tail),
   * an all-zeroes value is returned instead (any partial data that
   * could have been read is removed, but not returned).
   */
  template <typename T>
  T remove() {
    T res = get<T>(0);
    head += min(len(), sizeof(T));
    return res;
  }

   /*
   * Remove the given number of bytes from the data and return a pointer
   * to them.
   *
   * If this would read past the end of the data in this buffer (tail),
   * NULL returned instead (any partial data that could have been read
   * is removed, but not returned).
   */
  uint8_t *remove(size_t bytes) {
    uint8_t *res = get(0);
    head += min(len(), bytes);
    return res;
  }

  size_t len() {
    return tail - head;
  }
};

template <size_t size>
struct AllocBuffer : Buffer {
  AllocBuffer() : Buffer(buf, 0, size) { };
  uint8_t buf[size];
};

#endif // __BINARY_H
