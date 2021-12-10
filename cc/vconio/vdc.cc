#include <vxio/vconio/vdc.h>


namespace conio
{
//
//// clang-format off
//const std::map<std::string, TileEncoding> tile_encoding = {
//    {"─", {1, 0, 1, 0, 0}},
//    {"━", {2, 0, 2, 0, 0}},
//
//    {"│", {0, 1, 0, 1, 0}},
//    {"┃", {0, 2, 0, 2, 0}},
//
//    {"┌", {0, 0, 1, 1, 0}},
//    {"┍", {0, 0, 2, 1, 0}},
//    {"┎", {0, 0, 1, 2, 0}},
//    {"┏", {0, 0, 2, 2, 0}},
//
//    {"┐", {1, 0, 0, 1, 0}},
//    {"┑", {2, 0, 0, 1, 0}},
//    {"┒", {1, 0, 0, 2, 0}},
//    {"┓", {2, 0, 0, 2, 0}},
//
//    {"└", {0, 1, 1, 0, 0}},
//    {"┕", {0, 1, 2, 0, 0}},
//    {"┖", {0, 2, 1, 0, 0}},
//    {"┗", {0, 2, 2, 0, 0}},
//
//    {"┘", {1, 1, 0, 0, 0}},
//    {"┙", {2, 1, 0, 0, 0}},
//    {"┚", {1, 2, 0, 0, 0}},
//    {"┛", {2, 2, 0, 0, 0}},
//
//    {"├", {0, 1, 1, 1, 0}},
//    {"┝", {0, 1, 2, 1, 0}},
//    {"┞", {0, 2, 1, 1, 0}},
//    {"┟", {0, 1, 1, 2, 0}},
//    {"┠", {0, 2, 1, 2, 0}},
//    {"┡", {0, 2, 2, 1, 0}},
//    {"┢", {0, 1, 2, 2, 0}},
//    {"┣", {0, 2, 2, 2, 0}},
//
//    {"┤", {1, 1, 0, 1, 0}},
//    {"┥", {2, 1, 0, 1, 0}},
//    {"┦", {1, 2, 0, 1, 0}},
//    {"┧", {1, 1, 0, 2, 0}},
//    {"┨", {1, 2, 0, 2, 0}},
//    {"┩", {2, 2, 0, 1, 0}},
//    {"┪", {2, 1, 0, 2, 0}},
//    {"┫", {2, 2, 0, 2, 0}},
//
//    {"┬", {1, 0, 1, 1, 0}},
//    {"┭", {2, 0, 1, 1, 0}},
//    {"┮", {1, 0, 2, 1, 0}},
//    {"┯", {2, 0, 2, 1, 0}},
//    {"┰", {1, 0, 1, 2, 0}},
//    {"┱", {2, 0, 1, 2, 0}},
//    {"┲", {1, 0, 2, 2, 0}},
//    {"┳", {2, 0, 2, 2, 0}},
//
//    {"┴", {1, 1, 1, 0, 0}},
//    {"┵", {2, 1, 1, 0, 0}},
//    {"┶", {1, 1, 2, 0, 0}},
//    {"┷", {2, 1, 2, 0, 0}},
//    {"┸", {1, 2, 1, 0, 0}},
//    {"┹", {2, 2, 1, 0, 0}},
//    {"┺", {1, 2, 2, 0, 0}},
//    {"┻", {2, 2, 2, 0, 0}},
//
//    {"┼", {1, 1, 1, 1, 0}},
//    {"┽", {2, 1, 1, 1, 0}},
//    {"┾", {1, 1, 2, 1, 0}},
//    {"┿", {2, 1, 2, 1, 0}},
//    {"╀", {1, 2, 1, 1, 0}},
//    {"╁", {1, 1, 1, 2, 0}},
//    {"╂", {1, 2, 1, 2, 0}},
//    {"╃", {2, 2, 1, 1, 0}},
//    {"╄", {1, 2, 2, 1, 0}},
//    {"╅", {2, 1, 1, 2, 0}},
//    {"╆", {1, 1, 2, 2, 0}},
//    {"╇", {2, 2, 2, 1, 0}},
//    {"╈", {2, 1, 2, 2, 0}},
//    {"╉", {2, 2, 1, 2, 0}},
//    {"╊", {1, 2, 2, 2, 0}},
//    {"╋", {2, 2, 2, 2, 0}},
//
//    {"═", {3, 0, 3, 0, 0}},
//    {"║", {0, 3, 0, 3, 0}},
//
//    {"╒", {0, 0, 3, 1, 0}},
//    {"╓", {0, 0, 1, 3, 0}},
//    {"╔", {0, 0, 3, 3, 0}},
//
//    {"╕", {3, 0, 0, 1, 0}},
//    {"╖", {1, 0, 0, 3, 0}},
//    {"╗", {3, 0, 0, 3, 0}},
//
//    {"╘", {0, 1, 3, 0, 0}},
//    {"╙", {0, 3, 1, 0, 0}},
//    {"╚", {0, 3, 3, 0, 0}},
//
//    {"╛", {3, 1, 0, 0, 0}},
//    {"╜", {1, 3, 0, 0, 0}},
//    {"╝", {3, 3, 0, 0, 0}},
//
//    {"╞", {0, 1, 3, 1, 0}},
//    {"╟", {0, 3, 1, 3, 0}},
//    {"╠", {0, 3, 3, 3, 0}},
//
//    {"╡", {3, 1, 0, 1, 0}},
//    {"╢", {1, 3, 0, 3, 0}},
//    {"╣", {3, 3, 0, 3, 0}},
//
//    {"╤", {3, 0, 3, 1, 0}},
//    {"╥", {1, 0, 1, 3, 0}},
//    {"╦", {3, 0, 3, 3, 0}},
//
//    {"╧", {3, 1, 3, 0, 0}},
//    {"╨", {1, 3, 1, 0, 0}},
//    {"╩", {3, 3, 3, 0, 0}},
//
//    {"╪", {3, 1, 3, 1, 0}},
//    {"╫", {1, 3, 1, 3, 0}},
//    {"╬", {3, 3, 3, 3, 0}},
//
//    {"╭", {0, 0, 1, 1, 1}},
//    {"╮", {1, 0, 0, 1, 1}},
//    {"╯", {1, 1, 0, 0, 1}},
//    {"╰", {0, 1, 1, 0, 1}},
//
//    {"╴", {1, 0, 0, 0, 0}},
//    {"╵", {0, 1, 0, 0, 0}},
//    {"╶", {0, 0, 1, 0, 0}},
//    {"╷", {0, 0, 0, 1, 0}},
//
//    {"╸", {2, 0, 0, 0, 0}},
//    {"╹", {0, 2, 0, 0, 0}},
//    {"╺", {0, 0, 2, 0, 0}},
//    {"╻", {0, 0, 0, 2, 0}},
//
//    {"╼", {1, 0, 2, 0, 0}},
//    {"╽", {0, 1, 0, 2, 0}},
//    {"╾", {2, 0, 1, 0, 0}},
//    {"╿", {0, 2, 0, 1, 0}},
//};
//// clang-format on
//
//template <class A, class B>
//const std::map<B, A> InvertMap(const std::map<A, B> input) {
//  std::map<B, A> output;
//  for (const auto& it : input)
//    output[it.second] = it.first;
//  return output;
//}
//
//const std::map<TileEncoding, std::string> tile_encoding_inverse =
//    InvertMap(tile_encoding);
//
//void UpgradeLeftRight(std::string& left, std::string& right) {
//  const auto it_left = tile_encoding.find(left);
//  if (it_left == tile_encoding.end())
//    return;
//  const auto it_right = tile_encoding.find(right);
//  if (it_right == tile_encoding.end())
//    return;
//
//  if (it_left->second.right == 0 && it_right->second.left != 0) {
//    TileEncoding encoding_left = it_left->second;
//    encoding_left.right = it_right->second.left;
//    const auto it_left_upgrade = tile_encoding_inverse.find(encoding_left);
//    if (it_left_upgrade != tile_encoding_inverse.end())
//      left = it_left_upgrade->second;
//  }
//
//  if (it_right->second.left == 0 && it_left->second.right != 0) {
//    TileEncoding encoding_right = it_right->second;
//    encoding_right.left = it_left->second.right;
//    const auto it_right_upgrade = tile_encoding_inverse.find(encoding_right);
//    if (it_right_upgrade != tile_encoding_inverse.end())
//      right = it_right_upgrade->second;
//  }
//}
//
//void UpgradeTopDown(std::string& top, std::string& down) {
//  const auto it_top = tile_encoding.find(top);
//  if (it_top == tile_encoding.end())
//    return;
//  const auto it_down = tile_encoding.find(down);
//  if (it_down == tile_encoding.end())
//    return;
//
//  if (it_top->second.down == 0 && it_down->second.top != 0) {
//    TileEncoding encoding_top = it_top->second;
//    encoding_top.down = it_down->second.top;
//    const auto it_top_down = tile_encoding_inverse.find(encoding_top);
//    if (it_top_down != tile_encoding_inverse.end())
//      top = it_top_down->second;
//  }
//
//  if (it_down->second.top == 0 && it_top->second.down != 0) {
//    TileEncoding encoding_down = it_down->second;
//    encoding_down.top = it_top->second.down;
//    const auto it_down_top = tile_encoding_inverse.find(encoding_down);
//    if (it_down_top != tile_encoding_inverse.end())
//      down = it_down_top->second;
//  }
//}
//
//}  // namespace

}