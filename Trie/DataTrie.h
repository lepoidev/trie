/*
   Copyright 2020 Kyle LePoidevin-Gonzales

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
