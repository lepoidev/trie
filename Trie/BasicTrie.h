#pragma once
#include "TrieNode.h"

template< typename nodeTy, typename charTy >
class BasicTrie
{
public:
  BasicTrie()
    : m_root { std::make_shared< nodeTy >() }
  {
    static_assert( std::is_base_of< TrieNode< charTy >, nodeTy >::value, "Must use a TrieNode type" );
  }

  std::shared_ptr< nodeTy > const Insert( std::basic_string< charTy > str )
  {
    return nodeTy::Insert( m_root, str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< nodeTy > const Insert( IterTy&& begin, IterTy&& end )
  {
    return nodeTy::Insert( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  std::shared_ptr< nodeTy > const Remove( std::basic_string< charTy > str )
  {
    return nodeTy::Remove( m_root, str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< nodeTy > const Remove( IterTy&& begin, IterTy&& end )
  {
    return nodeTy::Remove( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  std::shared_ptr< nodeTy > const Find( std::basic_string< charTy > str ) const
  {
    return Find( str.begin(), str.end() );
  }

  template< typename IterTy >
  std::shared_ptr< nodeTy > const Find( IterTy&& begin, IterTy&& end ) const
  {
    return nodeTy::Find( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  bool const HasString( std::basic_string< charTy > str ) const
  {
    return HasString( str.begin(), str.end() );
  }

  template< typename IterTy >
  bool const HasString( IterTy&& begin, IterTy&& end ) const
  {
    return nodeTy::HasString( m_root, std::forward< IterTy >( begin ), std::forward< IterTy >( end ) );
  }

  void GetAllStrings( std::vector< std::basic_string< charTy > >& strings )
  {
    nodeTy::GetAllStrings( m_root, strings );
  }

  std::vector< std::basic_string< charTy > > const GetAllStrings()
  {
    std::vector< std::basic_string< charTy > > strings;
    GetAllStrings( strings );
    return strings;
  }

protected:
  std::shared_ptr< nodeTy > m_root;
};