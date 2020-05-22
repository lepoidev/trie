#pragma once
#include "TrieNode.h"

template< typename dataTy, typename charTy >
class DataTrieNode : TrieNode< charTy >
{
public:
  #pragma region Constructors
  DataTrieNode( dataTy data, charTy charVal )
    : DataTrieNode( charVal ), m_data { data }
  {

  }

  DataTrieNode( charTy charVal )
    : TrieNode<charTy>( charVal ), m_data {}
  {
    static_assert( !std::is_same<charTy, dataTy>::value, "Cannot use same type for data as char." );
  }

  DataTrieNode( dataTy data )
    : DataTrieNode(), m_data { data }
  {

  }

  DataTrieNode()
    : DataTrieNode( (charTy)0 )
  {

  }
  #pragma endregion

  #pragma region Getters / Setters
  void SetVal( dataTy const& data )
  {
    m_data = data;
  }
  dataTy const GetVal() const
  {
    return m_data;
  }
  #pragma endregion
protected:
  dataTy m_data;
};