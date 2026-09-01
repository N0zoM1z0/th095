#include "pbg/Lzss.hpp"

#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) ((pos + amount) & LZSS_DICTSIZE_MASK)

namespace th095
{
Lzss::TreeNode Lzss::m_Tree[LZSS_DICTSIZE + 1];
u8 Lzss::m_Dict[LZSS_DICTSIZE];

// FUNCTION: TH095 0x00456580; TH08 0x00474450 is the source-shape oracle.
void Lzss::InitTree(i32 root)
{
    m_Tree[LZSS_DICTSIZE].right = root;
    m_Tree[root].parent = LZSS_DICTSIZE;
    m_Tree[root].right = 0;
    m_Tree[root].left = 0;
}

// FUNCTION: TH095 0x004565D0; TH08 0x004744A0 is the source-shape oracle.
void Lzss::InitEncoderState()
{
    i32 i;

    for (i = 0; i < LZSS_DICTSIZE; i++)
    {
        m_Dict[i] = 0;
    }
    for (i = 0; i < LZSS_DICTSIZE + 1; i++)
    {
        m_Tree[i].parent = 0;
        m_Tree[i].left = 0;
        m_Tree[i].right = 0;
    }
}

// FUNCTION: TH095 0x00456650; TH08 0x00474520 is the source-shape oracle.
i32 Lzss::AddString(i32 newNode, i32 *matchPosition)
{
    struct AddStringLocals
    {
        i32 delta;
        i32 matchLength;
        i32 testNode;
        i32 *child;
        i32 i;
    } locals;

    if (newNode == 0)
        return 0;

    locals.testNode = m_Tree[LZSS_DICTSIZE].right;
    locals.matchLength = 0;
    for (;;)
    {
        for (locals.i = 0; locals.i < LZSS_LOOKAHEAD_MAX; locals.i++)
        {
            locals.delta =
                m_Dict[LZSS_DICTPOS_MOD(newNode, locals.i)] -
                m_Dict[LZSS_DICTPOS_MOD(locals.testNode, locals.i)];
            if (locals.delta != 0)
                break;
        }

        if (locals.i >= locals.matchLength)
        {
            locals.matchLength = locals.i;
            *matchPosition = locals.testNode;
            if (locals.matchLength >= LZSS_LOOKAHEAD_MAX)
            {
                ReplaceNode(locals.testNode, newNode);
                return locals.matchLength;
            }
        }

        if (locals.delta >= 0)
            locals.child = &m_Tree[locals.testNode].right;
        else
            locals.child = &m_Tree[locals.testNode].left;

        if (*locals.child == 0)
        {
            *locals.child = newNode;
            m_Tree[newNode].parent = locals.testNode;
            m_Tree[newNode].right = 0;
            m_Tree[newNode].left = 0;
            return locals.matchLength;
        }

        locals.testNode = *locals.child;
    }
}

// FUNCTION: TH095 0x00456770; TH08 0x00474640 is the source-shape oracle.
void Lzss::DeleteString(i32 p)
{
    if (m_Tree[p].parent == 0)
    {
        return;
    }
    if (m_Tree[p].right == 0)
    {
        ContractNode(p, m_Tree[p].left);
    }
    else if (m_Tree[p].left == 0)
    {
        ContractNode(p, m_Tree[p].right);
    }
    else
    {
        i32 replacement = FindNextNode(p);
        DeleteString(replacement);
        ReplaceNode(p, replacement);
    }
}

// FUNCTION: TH095 0x00456800; TH08 0x004746D0 is the source-shape oracle.
void Lzss::ContractNode(i32 oldNode, i32 newNode)
{
    m_Tree[newNode].parent = m_Tree[oldNode].parent;

    if (m_Tree[m_Tree[oldNode].parent].right == oldNode)
    {
        m_Tree[m_Tree[oldNode].parent].right = newNode;
    }
    else
    {
        m_Tree[m_Tree[oldNode].parent].left = newNode;
    }
    m_Tree[oldNode].parent = 0;
}

// FUNCTION: TH095 0x00456890; TH08 0x00474760 is the source-shape oracle.
void Lzss::ReplaceNode(i32 oldNode, i32 newNode)
{
    i32 parent = m_Tree[oldNode].parent;

    if (m_Tree[parent].left == oldNode)
    {
        m_Tree[parent].left = newNode;
    }
    else
    {
        m_Tree[parent].right = newNode;
    }
    m_Tree[newNode] = m_Tree[oldNode];
    m_Tree[m_Tree[newNode].left].parent = newNode;
    m_Tree[m_Tree[newNode].right].parent = newNode;
    m_Tree[oldNode].parent = 0;
}

// FUNCTION: TH095 0x00456950; TH08 0x00474820 is the source-shape oracle.
i32 Lzss::FindNextNode(i32 node)
{
    i32 next = m_Tree[node].left;

    while (m_Tree[next].right != 0)
    {
        next = m_Tree[next].right;
    }
    return next;
}
} // namespace th095
