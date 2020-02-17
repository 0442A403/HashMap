#include <algorithm>
#include <functional>
#include <list>
#include <iostream>
#include <vector>
#include <stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>> 
class HashMap {
public:
  using iterator = typename std::list<std::pair<const KeyType, ValueType> >::iterator;
  using const_iterator = typename std::list<std::pair<const KeyType, ValueType> >::const_iterator;

private:
  size_t sz = 0, capacity = 2;
  std::list<std::pair<const KeyType, ValueType>> data;
  std::vector<std::list<iterator>> pointers = std::vector<std::list<iterator>>(capacity);
  Hash hasher;

  size_t hash(const KeyType& key) const {
    return hasher(key) % capacity;
  }

public:
  iterator find(const KeyType& key) {
    for (auto& i : pointers[hash(key)]) {
      if (i->first == key) {
        return i;
      }
    }
    return data.end();
  }

  const_iterator find(const KeyType& key) const {
    for (auto& i : pointers[hash(key)]) {
      if (i->first == key) {
        return i;
      }
    }
    return data.end();
  }

  void insert(std::pair<KeyType, ValueType> toInsert) {
    for (auto& i : pointers[hash(toInsert.first)]) {
      if (i->first == toInsert.first) {
        return;
      }
    }
    KeyType& key = toInsert.first;
    data.push_back(toInsert);
    pointers[hash(key)].push_back(--data.end());
    sz++;
    if (2 * sz == capacity) {
      std::vector<std::list<iterator>> newContainer(capacity *= 2);
      for (auto it = data.begin(); it != data.end(); ++it) {
        newContainer[hash(it->first)].push_back(it);
      }
      std::swap(newContainer, pointers);
    }
  }

  void erase(const KeyType& key) {
    for (auto it = pointers[hash(key)].begin(); it != pointers[hash(key)].end(); ++it) {
      if ((*it)->first != key) {
        continue;
      }
      sz--;
      data.erase(*it);
      pointers[hash(key)].erase(it);
      break;
    }
  }

  ValueType& operator [] (const KeyType& key) {
    auto it = find(key);
    if (it != end()) {
      return it->second;
    }
    insert({ key, ValueType() });
    return find(key)->second;
  }

  const ValueType& at(const KeyType& key) const {
    auto it = find(key);
    if (it != data.end()) {
      return it->second;
    }
    throw std::out_of_range("Nothing to return");
  }
  
  HashMap(Hash hasher = Hash()) : hasher(hasher) {}

  template<typename Iterator> 
  HashMap(Iterator begin, Iterator end, Hash hasher = Hash()) : hasher(hasher) {
    while (begin != end) {
      operator[](begin->first) = begin->second;
    }
  }

  HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& initList, Hash hasher = Hash()) : hasher(hasher) {
    for (const auto& i : initList) {
      operator[](i.first) = i.second;
    }
  }

  size_t size() const {
    return sz;
  }

  size_t empty() const {
    return sz == 0u;
  }

  Hash hash_function() const {
    return hasher;
  }

  iterator begin() {
    return data.begin();
  }

  iterator end() {
    return data.end();
  }

  const_iterator begin() const {
    return data.begin();
  }

  const_iterator end() const {
    return data.end();
  }

  void clear() {
    sz = 0u;
    capacity = 2u;
    data.clear();
    pointers = std::vector<std::list<iterator>>(capacity);
  }
};

int main() {
  std::ios::sync_with_stdio(0);
  std::cin.tie(0); 
  std::cout.tie(0);

  HashMap<int, int> map;
  size_t q;
  std::cin >> q;
  for (size_t i = 0; i < q; ++i) {
    std::string query;
    std::cin >> query;
    if (query == "+") {
      int key, value;
      std::cin >> key >> value;
      map[key] = value;
    } else if (query == "-") {
      int key;
      std::cin >> key;
      map.erase(key);
    } else if (query == "?") {
      int key;
      std::cin >> key;
      auto it = map.find(key);
      if (it == map.end()) {
        std::cout << -1 << "\n";
      } else {
        std::cout << (*it).second << "\n";
      }
    } else if (query == "<") {
      for (const auto& i : map) {
        std::cout << i.first << " " << i.second << "\n";
      }
    } else if (query == "!") {
      map.clear();
    } else {
      return 1;
    }
  }
  std::cout << map.size() << "\n";
  return 0;
}
