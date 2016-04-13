#ifndef VOXBLOX_MAP_H
#define VOXBLOX_MAP_H

#include <utility>

#include "voxblox/common.h"
#include "voxblox/block.h"
#include "voxblox/block_hash.h"

namespace voxblox {

struct MapConfig {};

template <typename BlockType>
class Map {
 public:
  typedef typename BlockHashMapType<typename BlockType::Ptr>::type BlockHashMap;

  virtual ~Map() {}

  // By index.
  inline const BlockType& getBlockByIndex(const BlockIndex& index) const {
    typename BlockHashMap::const_iterator it = block_map_.find(index);
    if (it != block_map_.end()) {
      return *(it->second);
    } else {
      LOG(FATAL) << "Accessed unallocated block at " << index.transpose();
    }
  }

  inline BlockType& getBlockByIndex(const BlockIndex& index) {
    typename BlockHashMap::iterator it = block_map_.find(index);
    if (it != block_map_.end()) {
      return *(it->second);
    } else {
      LOG(FATAL) << "Accessed unallocated block at " << index.transpose();
    }
  }

  inline typename BlockType::ConstPtr getBlockPtrByIndex(
      const BlockIndex& index) const {
    typename BlockHashMap::const_iterator it = block_map_.find(index);
    if (it != block_map_.end()) {
      return it->second;
    } else {
      return typename BlockType::ConstPtr();
    }
  }

  inline typename BlockType::Ptr getBlockPtrByIndex(const BlockIndex& index) {
    typename BlockHashMap::iterator it = block_map_.find(index);
    if (it != block_map_.end()) {
      return it->second;
    } else {
      return typename BlockType::Ptr();
    }
  }

  // Gets a block by the block index it if already exists, otherwise allocates a
  // new one.
  inline typename BlockType::Ptr allocateBlockPtrByIndex(
      const BlockIndex& index) {
    typename BlockHashMap::iterator it = block_map_.find(index);
    if (it != block_map_.end()) {
      return it->second;
    } else {
      return allocateNewBlock(index);
    }
  }

  // Get by coords (helenol)
  // Delete blocks (helenefwefeol)

  // check by coords
  // virtual void addBlock() = 0;
  // delete blocks

  // Coord to block index.
  inline BlockIndex computeBlockIndexFromCoordinates(
      const Coordinates& coords) const {
    return VoxelIndex(
        static_cast<int>(std::floor(coords.x() * block_size_inv_)),
        static_cast<int>(std::floor(coords.y() * block_size_inv_)),
        static_cast<int>(std::floor(coords.z() * block_size_inv_)));
  }

  virtual typename BlockType::Ptr allocateNewBlock(const BlockIndex& index) = 0;

 protected:
  Map(FloatingPoint block_size) : block_size_(block_size) {
    block_size_inv_ = 1.0 / block_size_;
  }

  FloatingPoint block_size_;

  // Derived types.
  FloatingPoint block_size_inv_;

  BlockHashMap block_map_;
};

class TsdfMap : public Map<TsdfBlock> {
 public:
  TsdfMap(size_t voxels_per_side, FloatingPoint voxel_size)
      : Map(voxel_size * voxels_per_side),
        voxel_size_(voxel_size),
        voxels_per_side_(voxels_per_side) {}

  virtual TsdfBlock::Ptr allocateNewBlock(const BlockIndex& index) {
    auto my_iter = block_map_.find(index);
    auto insert_status = block_map_.emplace(index,
        std::make_shared<TsdfBlock>(index.cast<FloatingPoint>() * block_size_,
                      voxels_per_side_, voxel_size_));
    CHECK(insert_status.second) << "Block already exists when allocating at "
                                << index.transpose();
    // I don't wanna talk about it.
    return insert_status.first->second;
  }

 protected:
  size_t voxels_per_side_;
  FloatingPoint voxel_size_;
};

}  // namespace voxblox

#endif