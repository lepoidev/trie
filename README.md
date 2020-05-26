[![Build Status](https://travis-ci.org/lepoidev/trie.svg?branch=master)](https://travis-ci.org/lepoidev/trie)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://github.com/lepoidev/trie/blob/master/LICENSE)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Flepoidev%2Ftrie.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Flepoidev%2Ftrie?ref=badge_shield)

## Summary
This repository contains the implementation of a Trie data structure and a variation called a DataTrie. The DataTrie is a Trie where the nodes which mark the end a string also hold data. This is to provide an alternative to using `std::unordered_map` or `std::map` with string keys.

### More Info:
 * [Wikipedia](https://en.wikipedia.org/wiki/Trie)
 * [Visualization](https://www.cs.usfca.edu/~galles/visualization/Trie.html)

## Integration
Note that these data sctructures are designed for C++14
### Trie
```cpp
#include <Trie/Trie.h>
```
Instantiation examples:
```cpp
Trie< char > charTrie;

Trie< char16_t > char16Trie;

Trie< char32_t > char32Trie;

Trie< wchar_t > wcharTrie;
```
### DataTrie
```cpp
#include <Trie/DataTrie.h>
```
Instantiation examples:
```cpp
struct SomeData
{
  int someVariable;
  // ...
};

DataTrie< char, std::shared_ptr< SomeData > > charDataTrie;

DataTrie< char16_t, SomeData > char16DataTrie;

DataTrie< char32_t, std::function< bool(SomeData const&) > > char32DataTrie;

DataTrie< wchar_t, std::wstring > wcharDataTrie;
```
### Custom
To create a Trie variation:
1. Create a DerivedTrieNode class which extends `TrieNode` or `DataTrieNode`
2. Create a DerivedTrie class which extends `BasicTrie< DerivedTrieNode< ... >, CharTy >`
```cpp
#include <Trie/BasicTrie.h>
```
## Running Tests
```sh
$ cmake .
$ cmake --build .
$ ctest -VV --output-on-failure
```
