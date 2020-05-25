#pragma once
#include "DataTrieNode.h"
#include "BasicTrie.h"

template< typename CharTy, typename DataTy >
class DataTrie : public BasicTrie< DataTrieNode< CharTy, DataTy >, CharTy >
{
public:
  std::shared_ptr< DataTrieNode< CharTy, DataTy > > const Insert( std::basic_string< CharTy > str )
  {
    return Insert( str.begin(), str.end(), DataTy() );
  }

  std::shared_ptr< DataTrieNode< CharTy, DataTy > > const Insert( std::basic_string< CharTy > str, DataTy data )
  {
    return Insert( str.begin(), str.end(), data );
  }

  template< typename IterTy >
  std::shared_ptr< DataTrieNode< CharTy, DataTy > > const Insert( IterTy&& begin, IterTy&& end, DataTy data )
  {
    return DataTrieNode< CharTy, DataTy >::Insert( this->m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ), data );
  }
};
