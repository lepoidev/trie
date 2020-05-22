#pragma once
#include "TrieNode.h"
#include "BasicTrie.h"

template< typename charTy >
class Trie : public BasicTrie< TrieNode< charTy >, charTy >
{

};