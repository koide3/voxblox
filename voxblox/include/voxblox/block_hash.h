#ifndef VOXBLOX_BLOCK_HASH_H
#define VOXBLOX_BLOCK_HASH_H

#include <Eigen/Dense>
#include <unordered_map>

namespace voxblox {

struct BlockIndexHash {
  static constexpr size_t prime1 = 73856093;
  static constexpr size_t prime2 = 19349663;
  static constexpr size_t prime3 = 83492791;

  std::size_t operator()(const BlockIndex& index) const {
    return (index.x() * prime1 ^ index.y() * prime2 ^ index.z() * prime3);
  }
};

template <typename BlockType>
struct BlockHashMapType {
  typedef std::unordered_map<
      BlockIndex, BlockType, BlockIndexHash, std::equal_to<BlockIndex>,
      Eigen::aligned_allocator<std::pair<const BlockIndex, BlockType> > > type;
};

}  // namespace voxblox

#endif  // VOXBLOX_BLOCK_HASH_H