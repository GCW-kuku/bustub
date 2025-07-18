//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0) {
  n_bits_ = n_bits;
  if (n_bits < 0) {
    return;
  }
  buckets_.resize(1 << n_bits_, 0);
}

/**
 * @brief Function that computes binary.
 *
 * @param[in] hash
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  return hash;
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  int b = static_cast<int>(n_bits_);
  for (int i = BITSET_CAPACITY - b - 1; i >= 0; i--) {
    if (bset[i]) {
      return BITSET_CAPACITY - b - i;
    }
  }
  return 0;
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  auto hash_val = CalculateHash(val);
  auto bset = ComputeBinary(hash_val);
  uint8_t first_1_position = PositionOfLeftmostOne(bset);
  auto insert_position = GetBucketValue(bset);
  buckets_[insert_position] = std::max(first_1_position, buckets_[insert_position]);
}

/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  if (n_bits_ < 0) {
    return;
  }
  double sum = 0;
  for (auto bucket : buckets_) {
    sum += 1.0 / std::pow(2, bucket);
  }
  auto m = buckets_.size();
  auto result = CONSTANT * m * m / sum;
  cardinality_ = static_cast<size_t>(result);
}

template <typename KeyType>
auto HyperLogLog<KeyType>::GetBucketValue(std::bitset<BITSET_CAPACITY> bset) -> int {
  int position = 0;
  for (int i = BITSET_CAPACITY - n_bits_; i < BITSET_CAPACITY; i++) {
    position += static_cast<int>(bset[i]) << (i - BITSET_CAPACITY + n_bits_);
    /* code */
  }
  return position;
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub