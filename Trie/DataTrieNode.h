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

template< typename CharTy, typename DataTy >
class DataTrieNode : public TrieNode< CharTy >
{
public:
  typedef std::pair < std::basic_string< CharTy >, std::shared_ptr< DataTrieNode< CharTy, DataTy > > const > Pair;

  #pragma region Constructors
  DataTrieNode( CharTy const charVal, DataTy const data )
    : TrieNode< CharTy >( charVal ), m_data { data }
  {
    static_assert(!std::is_same< CharTy, DataTy >::value, "Cannot use same type for data as char.");
  }

  DataTrieNode( CharTy const charVal )
    : DataTrieNode( charVal, DataTy() )
  {
    static_assert( !std::is_same< CharTy, DataTy >::value, "Cannot use same type for data as char." );
  }

  DataTrieNode( DataTy const data )
    : DataTrieNode( static_cast< CharTy >( 0 ), data )
  {

  }

  DataTrieNode()
    : DataTrieNode( static_cast< CharTy >( 0 ) )
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
  template< typename NodeTy, typename IterTy >
  static std::shared_ptr < NodeTy > const Insert( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end, DataTy data )
  {
    auto& node { TrieNode< CharTy >::Insert( root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) ) };
    node->SetData( data );
    return node;
  }
  #pragma endregion
protected:
  DataTy m_data;
};