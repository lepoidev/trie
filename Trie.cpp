#include <iostream>
#include "Trie\Trie.h"
#include "Trie\DataTrie.h"

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

#define TrieTestAssert( x ) { bool cond { x };\
                              assert( cond );\
                              if( !( cond ) ) return false;\
                            }

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
#pragma endregion

#pragma region Test Functions
bool TestInsert()
{
  Trie< char > trie;

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

bool TestFind()
{
  Trie< char > trie;

  // insert and test if strings were inserted
  TrieTestAssert( Populate( trie ) );

  // try to find Non-Existing
  for( auto& str : nonExistantData )
  {
    auto const& node { trie.Find( str ) };
    TrieTestAssert( node == nullptr );
  }

  return true;
}

bool TestRemove()
{
  Trie< char > trie;

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
    auto const node { trie.Find( str ) };
    TrieTestAssert( node != nullptr );
  }

  // empty the trie
  std::shared_ptr< TrieNode< char > > lastNode;
  for( auto& str : nonExistantData )
  {
    lastNode = trie.Remove( str );
  }

  // no more items in the Trie
  TrieTestAssert( lastNode->CountChildren() == 0 );

  return true;
}

bool RunAllTests()
{
  static std::vector<TestFn> tests
  {
    TestInsert,
    TestFind,
    TestRemove
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
  }
  else 
  {
    std::cout << "Fail\n";
  }

  return 0;
}