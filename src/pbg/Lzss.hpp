#pragma once

#include "inttypes.hpp"

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

namespace th095
{
// Shared history ring used by compression, tree matching and decompression.
extern u8 g_LzssDictionary[LZSS_DICTSIZE];

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
};
} // namespace th095
