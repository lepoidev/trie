#pragma once
#include "TrieNode.h"

template< typename CharTy, typename DataTy >
class DataTrieNode : public TrieNode< CharTy >
{
public:
  typedef std::pair < std::basic_string< CharTy >, std::shared_ptr< DataTrieNode< CharTy, DataTy > > const > Pair;

  #pragma region Constructors
  DataTrieNode( CharTy const charVal, DataTy const data )
    : DataTrieNode( charVal ), m_data { data }
  {

  }

  DataTrieNode( CharTy const charVal )
    : TrieNode< CharTy >( charVal ), m_data {}
  {
    static_assert( !std::is_same< CharTy, DataTy >::value, "Cannot use same type for data as char." );
  }

  DataTrieNode( DataTy const data )
    : DataTrieNode(), m_data { data }
  {

  }

  DataTrieNode()
    : DataTrieNode( (CharTy)0 )
  {

  }
  #pragma endregion

  #pragma region Getters / Setters
  void SetData( DataTy const& data )
  {
    m_data = data;
  }
  DataTy const GetData() const
  {
    return m_data;
  }
  #pragma endregion

  #pragma region Static Operations
  template< typename NodeTy, typename IterTy, typename DataTy >
  static std::shared_ptr < NodeTy > const Insert( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end, DataTy&& data )
  {
    auto& node { __super::Insert( root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) ) };
    node->SetData( data );
    return node;
  }
  #pragma endregion
protected:
  DataTy m_data;
};