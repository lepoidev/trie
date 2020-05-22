#pragma once
#include "TrieNode.h"

template< typename valTy, typename charTy >
class DataTrieNode : TrieNode< charTy >
{
public:
  #pragma region Constructors
  DataTrieNode( valTy val, charTy charVal )
    : DataTrieNode( charVal ), m_val { val }
  {}

  DataTrieNode( charTy charVal )
    : TrieNode<charTy>( charVal ), m_val {}
  {}

  DataTrieNode( valTy val )
    : DataTrieNode(), m_val { val }
  {}

  DataTrieNode()
    : DataTrieNode( (charTy)0 )
  {}
  #pragma endregion

protected:
  valTy m_val;
};