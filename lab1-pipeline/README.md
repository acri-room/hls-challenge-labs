# 演習1

## パイプラインによる処理のタスク並列化

この演習では [bai-gaeshi2](https://acri-vhls-challenge.web.app/challenge/bai-gaeshi2) を例に、ループをパイプライン化してみます。bai-gaeshi2 は次の内容のチャレンジです。

```markdown
# 問題
入力されるfloat配列の要素の値をすべて倍にして出力しなさい。
ただし入力される値の範囲は [-1000, 1000) とします。
```

リファレンスとして次の

```cpp:kernel.cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE off
    out[i] = in[i] * 2;
  }
}
```

