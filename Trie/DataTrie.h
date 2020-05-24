#pragma once
#include "DataTrieNode.h"
#include "BasicTrie.h"

template< typename CharTy, typename dataTy >
class DataTrie : public BasicTrie< DataTrieNode< CharTy, dataTy >, CharTy >
{
public:
  std::shared_ptr< DataTrieNode< CharTy, dataTy > > const Insert( std::basic_string< CharTy > str )
  {
    return Insert( str.begin(), str.end(), dataTy() );
  }

  std::shared_ptr< DataTrieNode< CharTy, dataTy > > const Insert( std::basic_string< CharTy > str, dataTy data )
  {
    return Insert( str.begin(), str.end(), data );
  }

  template< typename IterTy >
  std::shared_ptr< DataTrieNode< CharTy, dataTy > > const Insert( IterTy&& begin, IterTy&& end, dataTy data )
  {
    return DataTrieNode< CharTy, dataTy >::Insert( this->m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ), data );
  }
};
