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

#ifdef COMPILE_TRIE_TESTS
#include "Trie/Trie.h"
#include "Trie/DataTrie.h"
#include <cassert>
#include <iostream>

typedef std::function< bool() > TestFn;

#pragma region Test Data
static std::vector< std::basic_string< char > > const testData
{
  "A",
  "to",
  "tea",
  "ted",
  "ten",
  "i",
  "in",
  "and",
  "inn"
};

static std::vector< std::basic_string< char > > const nonExistantData
{
  "te",   // inner node, not endpoint
  "tene", // past leaf
  "kyle", // no path from root
  "TEA"   // case sensitive
};
#pragma endregion

#pragma region Helpers

#define TrieTestAssert( x ) {\
                               bool cond { x };\
                               assert( cond );\
                               if( !( cond ) ) return false;\
                            }

static TestFn const WrapTrieTest( TestFn const& fn, std::basic_string< char > const fname )
{
  return [=]()
  {
    auto passed { fn() };
    if( !passed )
    {
      std::cout << "failed running function:" + fname + "\n";
    }
    return passed;
  };
}

#define WrapTrieTest( f ) WrapTrieTest( f, #f )

bool Populate( Trie< char >& trie )
{
  for( auto& str : testData )
  {
    trie.Insert( str.begin(), str.end() );
  }

  for( auto& str : testData )
  {
    auto& node { trie.Find( str.begin(), str.end() ) };
    TrieTestAssert( node != nullptr );
  }

  return true;
}

bool Populate( DataTrie< char, std::string > & trie )
{
  for( auto& str : testData )
  {
    trie.Insert( str.begin(), str.end(), str );
  }

  for( auto& str : testData )
  {
    auto node { trie.Find( str.begin(), str.end() ) };
    TrieTestAssert( node != nullptr );
  }

  return true;
}
#pragma endregion

#pragma region Test Functions
template< typename TrieTy >
bool TestInsert()
{
  TrieTy trie;

  TrieTestAssert( Populate( trie ) );

  // add data make sure its added
  for( auto& str : nonExistantData )
  {
    auto const node1 { trie.Insert( str ) };
    auto const node2 { trie.Find( str ) };
    TrieTestAssert( node1 != nullptr );
    TrieTestAssert( node2 != nullptr );
    TrieTestAssert( node1 == node2 );
  }

  // try to add existing data
  for( auto& str : nonExistantData )
  {
    auto const node1 { trie.Insert( str ) };
    auto const node2 { trie.Find( str ) };
    TrieTestAssert( node1 != nullptr );
    TrieTestAssert( node2 != nullptr );
    TrieTestAssert( node1 == node2 );
  }

  return true;
}

template< typename TrieTy >
bool TestFind()
{
  TrieTy trie;

  // insert and test if strings were inserted
  TrieTestAssert( Populate( trie ) );

  // try to find Non-Existing
  for( auto& str : nonExistantData )
  {
    auto const& node { trie.Find( str ) };
    TrieTestAssert( node == nullptr );
  }

  auto const& node { trie.Find( "\0" ) };
  TrieTestAssert( node == nullptr );

  return true;
}

template< typename TrieTy > 
bool TestRemove()
{
  TrieTy trie;

  TrieTestAssert( Populate( trie ) );

  // add new data
  for( auto& str : nonExistantData )
  {
    auto const& node { trie.Insert( str ) };
    TrieTestAssert( node != nullptr );
  }

  // remove initial data
  for( auto& str : testData )
  {
    auto const& node { trie.Remove( str ) };
    TrieTestAssert( trie.Find( str ) == nullptr );
  }

  // verify only the new data exists
  for( auto& str : nonExistantData )
  {
    auto const& node { trie.Find( str ) };
    TrieTestAssert( node != nullptr );
  }

  // empty the trie
  std::shared_ptr< TrieNode< char > > lastNode;
  for( auto& str : nonExistantData )
  {
    lastNode = trie.Remove( str );
  }

  // no more items in the Trie
  TrieTestAssert( lastNode->GetNumChildren() == 0 );

  return true;
}

template< typename TrieTy >
bool TestHasString()
{
  TrieTy trie;
  TrieTestAssert( Populate( trie ) );

  for( auto& str : testData )
  {
    TrieTestAssert( trie.HasString( str ) );
  }

  for( auto& str : nonExistantData )
  {
    TrieTestAssert( !trie.HasString( str ) );
  }

  return true;
}

template< typename TrieTy >
bool TestNumChildren()
{
  TrieTy trie;

  TrieTestAssert( Populate( trie ) );

  auto const& node { trie.Find( "in" ) };
  TrieTestAssert( node->GetNumChildren() == 1 );

  auto const& newNode { trie.Insert( "into" ) };
  TrieTestAssert( newNode->GetNumChildren() == 0 );
  TrieTestAssert( node->GetNumChildren() == 2 );

  auto const& trimPoint { trie.Remove( "inn" ) };
  TrieTestAssert( trimPoint == node );
  TrieTestAssert( node->GetNumChildren() == 1 );

  auto const& unrelated { trie.Insert( "intone" ) };
  TrieTestAssert( node->GetNumChildren() == 1 );

  return true;
}

bool TestDataTrieInsert()
{
  DataTrie< char, std::basic_string< char > > dataTrie;

  TrieTestAssert( Populate( dataTrie ) );

  for( auto& str : testData )
  {
    auto const& node { dataTrie.Find( str ) };
    TrieTestAssert( node->GetData() == str );
  }

  return true;
}

template< typename TrieTy >
bool TestGetAllStrings()
{
  TrieTy trie;

  TrieTestAssert( Populate( trie ) );

  auto actualStrings { trie.GetAllStrings() };
  auto expectedStrings { testData };

  TrieTestAssert( actualStrings.size() == expectedStrings.size() );

  std::sort( actualStrings.begin(), actualStrings.end() );
  std::sort( expectedStrings.begin(), expectedStrings.end() );

  for( auto const& str : actualStrings )
  {
    TrieTestAssert( std::binary_search( expectedStrings.begin(), expectedStrings.end(), str ) );
  }

  for( auto const& str : expectedStrings )
  {
    TrieTestAssert( std::binary_search( actualStrings.begin(), actualStrings.end(), str ) );
  }

  return true;
}

template< typename TrieTy >
bool TestGetAllStringsWithNodes()
{
  TrieTy trie;

  TrieTestAssert( Populate( trie ) );

  auto actualStringsWithNodes { trie.GetAllStringsWithNodes() };
  std::vector< std::basic_string< char > > actualStrings;
  for( auto stringWithNode : actualStringsWithNodes )
  {
    actualStrings.push_back( stringWithNode.first );
  }

  auto expectedStrings { testData };

  TrieTestAssert( actualStrings.size() == expectedStrings.size() );


  std::sort( actualStrings.begin(), actualStrings.end() );
  std::sort( expectedStrings.begin(), expectedStrings.end() );

  for( auto const& str : actualStrings )
  {
    TrieTestAssert( std::binary_search( expectedStrings.begin(), expectedStrings.end(), str ) );
  }

  for( auto const& str : expectedStrings )
  {
    TrieTestAssert( std::binary_search( actualStrings.begin(), actualStrings.end(), str ) );
  }

  auto const totalStrings { actualStrings.size() };
  for( auto i { totalStrings }; i < totalStrings; ++i )
  {
    auto const& expectedNode { trie.Find( expectedStrings[i] ) };
    auto const& actualNode { actualStringsWithNodes[i].second };
    TrieTestAssert( expectedNode == actualNode );
  }

  return true;
}

template< typename TrieTy >
bool TestAssignment()
{
  TrieTy t1;

  TrieTestAssert( Populate( t1 ) );

  // Copy
  TrieTy t2 = t1;
  TrieTy t3 ( t1 );

  auto t1Strings { t1.GetAllStrings() };
  auto t2Strings { t2.GetAllStrings() };
  auto t3Strings { t3.GetAllStrings() };

  t1.Remove( "A" );

  t1Strings = t1.GetAllStrings();
  t2Strings = t2.GetAllStrings();
  t2Strings = t3.GetAllStrings();

  TrieTestAssert( t1Strings.size() < t2Strings.size() );
  TrieTestAssert( t1Strings.size() < t3Strings.size() );

  // Move
  TrieTy t4;
  TrieTestAssert( Populate( t4 ) );

  TrieTy t5 = std::move( t2 );
  TrieTy t6 ( std::move( t4 ) );

  t2Strings = t2.GetAllStrings();
  auto t4Strings { t4.GetAllStrings() };
  auto t5Strings { t5.GetAllStrings() };
  auto t6Strings { t6.GetAllStrings() };

  TrieTestAssert( t2Strings.size() == 0 );
  TrieTestAssert( t4Strings.size() == 0 );
  TrieTestAssert( (t5Strings.size() != 0) && (t5Strings.size() == t6Strings.size()) )

  return true;
}

bool RunAllTests()
{
  static std::vector< TestFn > tests
  {
    WrapTrieTest( ( TestInsert< Trie< char > > ) ),
    WrapTrieTest( ( TestFind< Trie< char > > ) ),
    WrapTrieTest( ( TestRemove< Trie< char > > ) ),
    WrapTrieTest( ( TestHasString< Trie< char > > ) ),
    WrapTrieTest( ( TestNumChildren< Trie< char > > ) ),
    WrapTrieTest( ( TestGetAllStrings< Trie< char > > ) ),
    WrapTrieTest( ( TestGetAllStringsWithNodes< Trie< char > > ) ),
    WrapTrieTest( ( TestInsert< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestFind< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestRemove< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestHasString< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestNumChildren< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestGetAllStrings< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestGetAllStringsWithNodes< DataTrie< char, std::basic_string< char > > > ) ),
    WrapTrieTest( ( TestDataTrieInsert ) ),
    WrapTrieTest( ( TestAssignment< Trie< char > > ) ),
    WrapTrieTest( ( TestAssignment< DataTrie< char, std::basic_string< char > > > ) )
  };

  return std::all_of( tests.begin(), tests.end(), []( auto test )
  {
    return test();
  } );
}
#pragma endregion

int main()
{
  if( RunAllTests() )
  {
    std::cout << "Pass\n";
    return 0;
  }
  else 
  {
    std::cout << "Fail\n";
    return 1;
  }
}
#endif // COMPILE_TRIE_TESTS
