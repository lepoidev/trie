#pragma once
#include "TrieNode.h"

template< typename charTy, typename dataTy >
class DataTrieNode : public TrieNode< charTy >
{
public:
  #pragma region Constructors
  DataTrieNode( charTy charVal, dataTy data )
    : DataTrieNode( charVal ), m_data { data }
  {

  }

  DataTrieNode( charTy charVal )
    : TrieNode< charTy >( charVal ), m_data {}
  {
    static_assert( !std::is_same< charTy, dataTy >::value, "Cannot use same type for data as char." );
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
  void SetData( dataTy const& data )
  {
    m_data = data;
  }
  dataTy const GetData() const
  {
    return m_data;
  }
  #pragma endregion

  #pragma region Static Operations
  template< typename nodeTy, typename IterTy, typename dataTy >
  static std::shared_ptr < nodeTy > const Insert( std::shared_ptr< nodeTy > root, IterTy begin, IterTy end, dataTy data )
  {
    auto& node { __super::Insert( root, begin, end ) };
    node->SetData( data );
    return node;
  }
  #pragma endregion
protected:
  dataTy m_data;
};