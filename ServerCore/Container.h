#pragma once

#include "Types.h"
#include "Allocator.h"

#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename T>
using xVector = vector<T, StlAllocator<T>>;

template <typename T>
using xList = list<T, StlAllocator<T>>;

template <typename Key, typename Value, typename Pred = less<Key>>
using xMap = map<Key, Value, Pred, StlAllocator<pair<const Key, Value>>>;

template <typename Key,  typename Pred = less<Key>>
using xSet = set<Key, Pred, StlAllocator<const Key>>;

template <typename Type>
using xDeque = deque<Type, StlAllocator<Type>>;

template <typename Type, typename Container = xDeque<Type>>
using xQueue = queue<Type, Container>;

template <typename Type,typename Container = xDeque<Type>>
using xStack = stack<Type, Container>;

template <typename Type, typename Container = xVector<Type>,typename Pred = less<typename Container::value_type>>
using xPriorityQueue = priority_queue<Type, Container,Pred>;

template <typename Key, typename Type, typename Hasher = hash<Key> ,typename KeyEq =equal_to<Key>>
using xHashMap = unordered_map<Key,Type,Hasher, KeyEq,StlAllocator<pair<const Key,Type>>>;

template <typename Key,  typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using xHashSet = unordered_map<Key, Hasher, KeyEq, StlAllocator<Key>>;


using String = basic_string<char, char_traits<char>, StlAllocator<char>>;

using Wstring = basic_string<wchar_t, char_traits<wchar_t>, StlAllocator<wchar_t>>;
