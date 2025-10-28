//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include "common/exception.h"
namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

auto LRUKReplacer::Evict() -> std::optional<frame_id_t> {
  if (node_store_.empty()) {
    return std::nullopt;
  }
  size_t last_time1 = SIZE_MAX;
  size_t last_time2 = SIZE_MAX;
  frame_id_t frame_id1;
  frame_id_t frame_id2;
  for (auto &node : node_store_) {
    if (node.second.is_evictable_ == false) continue;
    std::list<size_t> &history = node.second.history_;
    if (history.size() < k_) {
      if (history.back() < last_time1) {
        last_time1 = history.back();
        frame_id1 = node.first;
      }
    } else {
      auto it = std::prev(history.end(), k_);
      if (*it < last_time2) {
        last_time2 = *it;
        frame_id2 = node.first;
      }
    }
  }
  if (last_time1 != SIZE_MAX) {
    node_store_.erase(frame_id1);
    return frame_id1;
  }
  if (last_time2 != SIZE_MAX) {
    node_store_.erase(frame_id2);
    return frame_id2;
  }
  return std::nullopt;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id, [[maybe_unused]] AccessType access_type) {
  auto &node = node_store_[frame_id];
  node.history_.push_back(current_timestamp_);
  node.fid_ = frame_id;
  current_timestamp_++;
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  node_store_[frame_id].is_evictable_ = (set_evictable);
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
  if (node_store_.find(frame_id) != node_store_.end()) {
    node_store_.erase(frame_id);
  }
}

auto LRUKReplacer::Size() -> size_t {
  auto num = 0;
  for (auto node : node_store_) {
    if (node.second.is_evictable_) {
      num++;
    }
  }
  return num;
}

}  // namespace bustub
