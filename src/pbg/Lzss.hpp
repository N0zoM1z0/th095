#pragma once

#include "inttypes.hpp"

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)

namespace th095
{
class Lzss
{
  public:
    static void InitTree(i32 root);
    static void InitEncoderState();
    static i32 AddString(i32 newNode, i32 *matchPosition);
    static void DeleteString(i32 p);
    static void ContractNode(i32 oldNode, i32 newNode);
    static void ReplaceNode(i32 oldNode, i32 newNode);
    static i32 FindNextNode(i32 node);

  private:
    struct TreeNode
    {
        i32 parent;
        i32 left;
        i32 right;
    };

    static TreeNode m_Tree[LZSS_DICTSIZE + 1];
    static u8 m_Dict[LZSS_DICTSIZE];
};
} // namespace th095
