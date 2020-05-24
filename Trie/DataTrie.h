#pragma once
#include "DataTrieNode.h"
#include "BasicTrie.h"

template< typename charTy, typename dataTy >
class DataTrie : public BasicTrie< DataTrieNode< charTy, dataTy >, charTy >
{
public:
  std::shared_ptr< DataTrieNode< charTy, dataTy > > const Insert( std::basic_string< charTy > str )
  {
    return Insert( str.begin(), str.end(), dataTy() );
  }

  std::shared_ptr< DataTrieNode< charTy, dataTy > > const Insert( std::basic_string< charTy > str, dataTy data )
  {
    return Insert( str.begin(), str.end(), data );
  }

  template< typename IterTy >
  std::shared_ptr< DataTrieNode< charTy, dataTy > > const Insert( IterTy&& begin, IterTy&& end, dataTy data )
  {
    return DataTrieNode< charTy, dataTy >::Insert( this->m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ), data );
  }
};
