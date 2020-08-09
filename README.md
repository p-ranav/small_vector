small_vector
============

"Small Vector" optimization for Modern C++: store up to a small number of items on the stack

## Example

```cpp
#include <iostream>
#include <iterator>

#include <small_vector/small_vector.h>
using namespace sv;

int main() {
 
  // This small vector can hold up to 4 items on the stack
  small_vector<int, 4> vec{1, 2, 3, 4};
  
  // It will automatically move its contents to the heap if
  // contains more than four items:
  vec.push_back(5);
  
  // Access and modify contents
  vec[0] = vec[1] + vec[2];

  // Iterate and print contents
  std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));

}
```
