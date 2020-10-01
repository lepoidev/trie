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
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <stack>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>

template< typename CharTy >
class TrieNode
{
public:
  static constexpr size_t NumChars = 1ULL << 8ULL * sizeof( CharTy );
  typedef std::pair < std::basic_string< CharTy >, std::shared_ptr< TrieNode< CharTy > > const > Pair;

  #pragma region Constructors
  TrieNode( CharTy const charVal )
    : m_char { charVal }, m_isEndOfAnEntry { false }, m_numChildren { 0ULL }
  {
    static_assert( std::is_integral< CharTy >::value, "Must use an integral type for CharTy" );
    m_children.resize( NumChars );
  }
  TrieNode() : TrieNode( static_cast<CharTy>( 0 ) ) {}
  #pragma endregion

  #pragma region Static Operations
  template< typename NodeTy >
  static std::shared_ptr < NodeTy > const CloneSubTrie( NodeTy const& root )
  {
    auto ret { std::make_shared< NodeTy >( root ) };
    for( CharTy c { 0ULL }; c < NumChars; ++c )
    {
      auto const& rhsChild { root.GetChild( c ) };
      if( rhsChild != nullptr )
      {
        ret->AddChild( CloneSubTrie( rhsChild ) );
      }
    }

    return std::move( ret );
  }

  template< typename NodeTy >
  static std::shared_ptr < NodeTy > const CloneSubTrie( std::shared_ptr< NodeTy > const& root )
  {
    return (root == nullptr) ? nullptr : CloneSubTrie( *root );
  }

  template< typename NodeTy, typename IterTy >
  static std::shared_ptr < NodeTy > const Find( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );
    if( root == nullptr )
    {
      return std::shared_ptr< NodeTy >();
    }

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast< NodeTy >( curNode->GetChild( *it ) );
      if( curNode == nullptr )
      {
        break;
      }

      auto nextIt { it };
      ++nextIt;
      auto const& isLastChar { nextIt == end };
      auto const& isSameChar { curNode->m_char == *it };

      if( curNode->m_isEndOfAnEntry && isLastChar && isSameChar )
      {
        return curNode;
      }
      else if( !isSameChar )
      {
        break;
      }
    }

    return std::shared_ptr< NodeTy >();
  }

  template< typename NodeTy, typename IterTy >
  static std::shared_ptr < NodeTy > const Insert( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< NodeTy >();
    }

    auto curNode { root };
    auto lastNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast< NodeTy >( curNode->GetChild( *it ) );

      if( curNode == nullptr )
      {
        curNode = std::make_shared< NodeTy >( *it );
        lastNode->AddChild( curNode );
      }
      lastNode = curNode;
    }

    lastNode->m_isEndOfAnEntry = true;

    return lastNode;
  }

  template< typename NodeTy, typename IterTy >
  static std::shared_ptr < NodeTy > const Remove( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end )
  {
    static_assert(std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type");
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< NodeTy >();
    }

    std::shared_ptr< NodeTy > lastEntryEnd { root };
    CharTy nearestCharToLastEntryEnd { *begin };

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast< NodeTy >( curNode->GetChild( *it ) );
      if( curNode == nullptr )
      {
        break;
      }

      auto nextIt { it };
      ++nextIt;

      auto const& isLastChar { nextIt == end };
      auto const& isSameChar { curNode->m_char == *it };

      if( curNode->m_isEndOfAnEntry && isLastChar && isSameChar )
      {
        curNode->m_isEndOfAnEntry = false;
        if( curNode->GetNumChildren() == 0 )
        {
          lastEntryEnd->RemoveChild( nearestCharToLastEntryEnd );
        }
        return lastEntryEnd;
      }
      else if( !isSameChar )
      {
        break;
      }

      if( (curNode->m_isEndOfAnEntry || (curNode->GetNumChildren() > 1 ) ) && !isLastChar )
      {
        lastEntryEnd = curNode;
        nearestCharToLastEntryEnd = *nextIt;
      }
    }

    return std::shared_ptr< NodeTy >();
  }

  template< typename NodeTy, typename IterTy >
  static bool const HasString( std::shared_ptr< NodeTy > const& root, IterTy&& begin, IterTy&& end )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );
    auto& node { Find( root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) ) };
    return node != nullptr;
  }

  template< typename NodeTy >
  static void GetAllStrings( std::shared_ptr< NodeTy > const& root, std::vector< std::basic_string< CharTy > >& strings )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );

    std::basic_string< CharTy > intialStr {};
    for( auto const& child : root->m_children )
    {
      if( child != nullptr )
      {
        GetAllStrings( child, intialStr, strings );
      }
    }
  }

  template< typename NodeTy >
  static void GetAllStringsWithNodes( std::shared_ptr< NodeTy > const& root, std::vector< typename NodeTy::Pair >& stringsWithNodes )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );

    std::basic_string< CharTy > intialStr {};
    for( auto const& child : root->m_children )
    {
      auto derived { std::static_pointer_cast< NodeTy >( child ) };
      if( derived != nullptr )
      {
        GetAllStringsWithNodes( derived, intialStr, stringsWithNodes );
      }
    }
  }
  #pragma endregion

  size_t const GetNumChildren() const
  {
    return m_numChildren;
  }

protected:
  CharTy m_char;
  bool m_isEndOfAnEntry;
  size_t m_numChildren;
  std::vector< std::shared_ptr< TrieNode< CharTy > > > m_children;


  std::shared_ptr< TrieNode< CharTy > > const& GetChild( CharTy const c ) const
  {
    return m_children[c];
  }

  template< typename NodeTy >
  void AddChild( NodeTy const& node )
  {
    auto const c { node->m_char };
    m_children[c] = node;
    ++m_numChildren;
  }

  void RemoveChild( CharTy const c )
  {
    m_children[c].reset();
    --m_numChildren;
  }

  #pragma region Private Static Operations
  template< typename NodeTy >
  static void GetAllStrings( std::shared_ptr< NodeTy > const& root, std::basic_string< CharTy > strToRoot, std::vector< std::basic_string< CharTy > >& strings )
  {
    static_assert( std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type" );

    auto strToCurrent { strToRoot + root->m_char };
    if( root->m_isEndOfAnEntry )
    {
      strings.push_back( strToCurrent );
    }

    for( auto const& child : root->m_children )
    {
      if( child != nullptr )
      {
        GetAllStrings( child, strToCurrent, strings );
      }
    }
  }

  template< typename NodeTy >
  static void GetAllStringsWithNodes( std::shared_ptr< NodeTy > const& root,
                                      std::basic_string< CharTy > strToRoot,
                                      std::vector< typename NodeTy::Pair >& stringsWithNodes )
  {
    static_assert(std::is_base_of< TrieNode< CharTy >, NodeTy >::value, "Must use a TrieNode type");

    auto strToCurrent { strToRoot + root->m_char };
    if( root->m_isEndOfAnEntry )
    {
      stringsWithNodes.push_back( { strToCurrent, root } );
    }

    for( auto const& child : root->m_children )
    {
      auto derived { std::static_pointer_cast<NodeTy>(child) };
      if( derived != nullptr )
      {
        GetAllStringsWithNodes( derived, strToCurrent, stringsWithNodes );
      }
    }
  }
  #pragma endregion
};