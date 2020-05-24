#pragma once
#include "TrieNode.h"
#include "BasicTrie.h"

template< typename CharTy >
class Trie : public BasicTrie< TrieNode< CharTy >, CharTy >
{

};