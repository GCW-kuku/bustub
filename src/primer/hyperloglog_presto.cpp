//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) : cardinality_(0) {
  n_bits_ = n_leading_bits;
  if (n_leading_bits < 0) {
    return;
  }
  dense_bucket_.resize(1 << n_leading_bits);
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  auto hash_val = CalculateHash(val);
  auto bset = std::bitset<64>(hash_val);
  auto num_of_0 = PositionOfLeftmostOne(bset);
  auto insert_position = GetBucketValue(bset);
  auto low4 = num_of_0 & 0x0F;
  auto high3 = (num_of_0 >> 4) & 0x07;
  auto current_bucket_value =
      GetOverflowBucketofIndex(insert_position << 4).to_ulong() | dense_bucket_[insert_position].to_ulong();

  if (num_of_0 > current_bucket_value) {
    dense_bucket_[insert_position] = low4;
    overflow_bucket_[insert_position] = high3;
  }
}

template <typename KeyType>
auto HyperLogLogPresto<KeyType>::PositionOfLeftmostOne(const std::bitset<64> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  int b = static_cast<int>(n_bits_);
  for (int i = 0; i < 64 - b; i++) {
    if (bset[i]) {
      return i;
    }
  }
  return 64 - b;
}

template <typename KeyType>
auto HyperLogLogPresto<KeyType>::GetBucketValue(std::bitset<64> bset) -> int {
  int position = 0;
  for (int i = 64 - n_bits_; i < 64; i++) {
    position += static_cast<int>(bset[i]) << (i - 64 + n_bits_);
    /* code */
  }
  return position;
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  if (n_bits_ < 0) {
    return;
  }
  double sum = 0;
  for (size_t i = 0; i < dense_bucket_.size(); i++) {
    auto low4 = dense_bucket_[i];
    auto high3 = GetOverflowBucketofIndex(i);
    auto value = (high3.to_ulong() << 4) | low4.to_ulong();
    sum += 1.0 / std::pow(2, value);
  }
  auto m = dense_bucket_.size();
  auto result = CONSTANT * m * m / sum;
  cardinality_ = static_cast<size_t>(result);
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub