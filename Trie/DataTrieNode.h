#pragma once
#include "TrieNode.h"

template< typename CharTy, typename dataTy >
class DataTrieNode : public TrieNode< CharTy >
{
public:
  #pragma region Constructors
  DataTrieNode( CharTy const charVal, dataTy const data )
    : DataTrieNode( charVal ), m_data { data }
  {

  }

  DataTrieNode( CharTy const charVal )
    : TrieNode< CharTy >( charVal ), m_data {}
  {
    static_assert( !std::is_same< CharTy, dataTy >::value, "Cannot use same type for data as char." );
  }

  DataTrieNode( dataTy const data )
    : DataTrieNode(), m_data { data }
  {

  }

  DataTrieNode()
    : DataTrieNode( (CharTy)0 )
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
  template< typename NodeTy, typename IterTy, typename dataTy >
  static std::shared_ptr < NodeTy > const Insert( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end, dataTy&& data )
  {
    auto& node { __super::Insert( root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) ) };
    node->SetData( data );
    return node;
  }
  #pragma endregion
protected:
  dataTy m_data;
};