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
    : m_char { charVal }, m_isEndOfAnEntry { false }
  {
    static_assert( std::is_integral< charTy >::value, "Must use an integral type for charTy" );
    m_children.resize( NumChars() );
  }
  TrieNode() : TrieNode( (charTy)0 ) {}
  #pragma endregion

  #pragma region Static Operations
  template< typename nodeTy, typename IterTy >
  static std::shared_ptr < nodeTy > const Find( std::shared_ptr< nodeTy > root, IterTy begin, IterTy end )
  {
    static_assert(std::is_base_of< TrieNode< charTy >, nodeTy >::value, "Must use a TrieNode type");
    if( root == nullptr )
    {
      return std::shared_ptr< nodeTy >();
    }

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast<nodeTy>( curNode->GetChild( *it ) );
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

    return std::shared_ptr< nodeTy >();
  }

  template< typename nodeTy, typename IterTy >
  static std::shared_ptr < nodeTy > const Insert( std::shared_ptr< nodeTy > root, IterTy begin, IterTy end )
  {
    static_assert(std::is_base_of< TrieNode< charTy >, nodeTy >::value, "Must use a TrieNode type");
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< nodeTy >();
    }

    auto curNode { root };
    auto lastNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast< nodeTy >( curNode->GetChild( *it ) );

      if( curNode == nullptr )
      {
        curNode = std::make_shared< nodeTy >( *it );
        lastNode->m_children[*it] = curNode;
      }
      lastNode = curNode;
    }

    lastNode->m_isEndOfAnEntry = true;

    return lastNode;
  }

  template< typename nodeTy, typename IterTy >
  static std::shared_ptr < nodeTy > const Remove( std::shared_ptr< nodeTy > root, IterTy begin, IterTy end )
  {
    static_assert(std::is_base_of< TrieNode< charTy >, nodeTy >::value, "Must use a TrieNode type");
    if( root == nullptr || begin == end )
    {
      return std::shared_ptr< nodeTy >();
    }

    std::shared_ptr< nodeTy > lastEntryEnd { root };
    charTy nearestCharToLastEntryEnd { *begin };

    auto curNode { root };
    for( IterTy it { begin }; it != end; ++it )
    {
      curNode = std::static_pointer_cast<nodeTy>( curNode->GetChild( *it ) );
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

    return std::shared_ptr< nodeTy >();
  }

  template< typename nodeTy, typename IterTy >
  static bool const HasString( std::shared_ptr< nodeTy > root, IterTy begin, IterTy end )
  {
    static_assert( std::is_base_of< TrieNode< charTy >, nodeTy >::value, "Must use a TrieNode type" );
    auto& node { Find( root, begin, end ) };
    return node == nullptr;
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

  std::shared_ptr< TrieNode< charTy > > GetChild( charTy const c )
  {
    return m_children[c];
  }
};