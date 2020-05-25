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
#include "TrieNode.h"

template< typename NodeTy, typename CharTy >
class BasicTrie
{
public:
  BasicTrie()
    : m_root { std::make_shared< NodeTy >() }
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );
  }

  std::shared_ptr< NodeTy > const Insert( std::basic_string< CharTy > str )
  {
    return NodeTy::Insert( m_root, str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< NodeTy > const Insert( IterTy&& begin, IterTy&& end )
  {
    return NodeTy::Insert( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  std::shared_ptr< NodeTy > const Remove( std::basic_string< CharTy > str )
  {
    return NodeTy::Remove( m_root, str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< NodeTy > const Remove( IterTy&& begin, IterTy&& end )
  {
    return NodeTy::Remove( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  std::shared_ptr< NodeTy > const Find( std::basic_string< CharTy > str ) const
  {
    return Find( str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< NodeTy > const Find( IterTy&& begin, IterTy&& end ) const
  {
    return NodeTy::Find( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  bool const HasString( std::basic_string< CharTy > str ) const
  {
    return HasString( str.begin(), str.end() );
  }

  template< typename IterTy >
  bool const HasString( IterTy&& begin, IterTy&& end ) const
  {
    return NodeTy::HasString( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  void GetAllStrings( std::vector< std::basic_string< CharTy > >& strings ) const
  {
    NodeTy::GetAllStrings( m_root, strings );
  }

  std::vector< std::basic_string< CharTy > > const GetAllStrings() const
  {
    std::vector< std::basic_string< CharTy > > strings;
    GetAllStrings( strings );
    return strings;
  }

  void GetAllStringsWithNodes( std::vector< typename NodeTy::Pair >& stringsWithNodes ) const
  {
    NodeTy::GetAllStringsWithNodes( m_root, stringsWithNodes );
  }

  std::vector< typename NodeTy::Pair > const GetAllStringsWithNodes() const
  {
    std::vector< typename NodeTy::Pair > stringsWithNodes;
    GetAllStringsWithNodes( stringsWithNodes );
    return stringsWithNodes;
  }

protected:
  std::shared_ptr< NodeTy > m_root;
};