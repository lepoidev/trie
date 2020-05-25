cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(TrieTest)

set(SOURCES TrieTest.cpp)
set(TRIE_DIR Trie)
set(HEADERS ${TRIE_DIR}/Trie.h ${TRIE_DIR}/TrieNode.h ${TRIE_DIR}/BasicTrie.h ${TRIE_DIR}/DataTrie.h ${TRIE_DIR}/DataTrieNode.h)
set(COMPILE_DEFS COMPILE_TRIE_TESTS)

include_directories(${TRIE_DIR})

set(TARGET_NAME trieTest.bin)
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_compile_definitions(${TARGET_NAME} PRIVATE ${COMPILE_DEFS})
target_include_directories(${TARGET_NAME} PRIVATE ${TRIE_DIR})
