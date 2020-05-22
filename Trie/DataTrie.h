#pragma once
#include "DataTrieNode.h"
#include "BasicTrie.h"

template< typename charTy, typename dataTy >
class DataTrie : public BasicTrie< DataTrieNode< dataTy, charTy >, charTy >
{
public:
  std::shared_ptr< DataTrieNode< dataTy, charTy > > const Insert( std::basic_string< charTy > str, dataTy data )
  {
    return Insert( str.begin(), str.end(), data );
  }

  template< typename IterTy >
  std::shared_ptr< DataTrieNode< dataTy, charTy > > const Insert( IterTy begin, IterTy end, dataTy data )
  {
    auto& inserted { DataTrieNode< dataTy, charTy >::Insert( this->m_root, begin, end ) };
    inserted.SetVal( data );
  }
};
