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
#include <cassert>

template< typename charTy >
class TrieNode
{
public:
  #pragma region Constructors
  TrieNode( charTy charVal )
    : m_char { charVal }
  {
    //static_assert( std::is_integral< charTy >::value, "Must use an integral type for charTy" );
    m_children.resize( NumChars() );
  }
  TrieNode() : TrieNode( (charTy)0 ) {}
  #pragma endregion

  #pragma region Static Operations
  template< typename IterTy >
  static std::shared_ptr < TrieNode< charTy > > const Find( std::shared_ptr< TrieNode< charTy > > root, IterTy begin, IterTy end )
  {
    if( root == nullptr )
    {
      return std::shared_ptr< TrieNode< charTy > >();
    }

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = curNode->m_children[*it];
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

    return std::shared_ptr< TrieNode< charTy > >();
  }

  template< typename IterTy >
  static std::shared_ptr < TrieNode< charTy > > const Insert( std::shared_ptr< TrieNode< charTy > > root, IterTy begin, IterTy end )
  {
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< TrieNode< charTy > >();
    }

    auto curNode { root };
    auto lastNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = curNode->m_children[*it];

      if( curNode == nullptr )
      {
        curNode = std::make_unique< TrieNode< charTy > >( *it );
        lastNode->m_children[*it] = curNode;
      }
      lastNode = curNode;
    }

    lastNode->m_isEndOfAnEntry = true;

    return lastNode;
  }

  template< typename IterTy >
  static std::shared_ptr < TrieNode< charTy > > const Remove( std::shared_ptr< TrieNode< charTy > > root, IterTy begin, IterTy end )
  {
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< TrieNode< charTy > >();
    }

    std::shared_ptr< TrieNode< charTy > > lastEntryEnd { root };
    charTy nearestCharToLastEntryEnd { *begin };

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = curNode->m_children[*it];
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
        if( curNode->CountChildren() == 0 )
        {
          lastEntryEnd->m_children[nearestCharToLastEntryEnd].reset();
        }
        return lastEntryEnd;
      }
      else if( !isSameChar )
      {
        break;
      }

      if( (curNode->m_isEndOfAnEntry || (curNode->CountChildren() > 1 ) ) && !isLastChar )
      {
        lastEntryEnd = curNode;
        nearestCharToLastEntryEnd = *nextIt;
      }
    }

    return std::shared_ptr< TrieNode< charTy > >();
  }
  #pragma endregion

  // temporary, will add a property for this
  size_t const CountChildren() const
  {
    size_t sum { 0 };
    for( auto const& child : m_children )
    {
      if( child != nullptr )
      {
        ++sum;
      }
    }

    return sum;
  }

protected:
  charTy m_char;
  bool m_isEndOfAnEntry;

  std::vector< std::shared_ptr< TrieNode< charTy > > > m_children;
  static size_t const NumChars()
  {
    return 1ULL << 8ULL * sizeof( charTy );
  }
};