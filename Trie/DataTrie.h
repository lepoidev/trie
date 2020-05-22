#pragma once
#include "DataTrieNode.h"
#include "BasicTrie.h"

template< typename charTy, typename dataTy >
class DataTrie : BasicTrie< DataTrieNode< dataTy, charTy >, charTy >
{

};
