# 演習 6

## アルゴリズム

この演習では [vector-mean-var2](https://acri-vhls-challenge.web.app/challenge/vector-mean-var2) を題材として用い、適切なアルゴリズムを選択することで計算を効率的に行うことができることを学びます。

`vector-mean-var2` の問題文は次です（数式は省略しています）。

```markdown
# 問題
入力されるfloat配列の要素の平均 mean と分散 var を計算し、出力してください。ただし、0.01 以下の誤差は許容します。

## カーネル引数
- in
  - 入力float配列。最大要素数は1024。
- size
  - 入力float配列のうち処理すべき要素数。1~1024。
- mean
  - 計算した平均を格納するサイズ１のfloat配列。
- vari
  - 計算した分散を格納するサイズ１のfloat配列。
```

リファレンスコードは次です。式に従ってふたつのループで平均と分散を求めています。

```cpp
#include "kernel.hpp"

void kernel(
  const float in[1024],
  const int size,
  float mean[1],
  float vari[1]
) {
  // Calc mean
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += in[i];
  }
  mean[0] = sum / size;

  // Calc var
  sum = 0;
  for (int i = 0; i < size; i++) {
    sum += (in[i] - mean[0]) * (in[i] - mean[0]);
  }
  vari[0] = sum / size;
}
```

### 演習 6-1

リファレンスコードのサイクル数を確認してください。

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  リファレンスコードを採点すると次の結果になります。

  ```
  Bytes of kernel code: 488
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 2633
    LUT  : 4473
    DSP  : 5
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.659
  Clock frequency (MHz): 602
  Simulation cycle: 27089
  Simulation time (ns): 44940.651
  ```
  
  累積演算が 13 サイクル、1,024回のループがふたつであることから、見積もりサイクル数は 13 * 1024 * 2 = 26,624 となり、シミュレーション結果とおおよそ一致します。
</details>

### 演習 6-2

分散の式には平均が出てくるので、平均を求めてから分散を求める必要があると思ってしまいますが、ひとつのループで平均と分散を同時に計算する方法があります。[高校数学の美しい物語「分散の意味と2通りの求め方・計算例」](https://manabitimes.jp/math/1081)を参考にして上記のコードを書き換えてみてください。

答えは用意していませんので各自でチャレンジしてみましょう。うまくできたら HLS チャレンジに投稿してください。

### 演習 6-3（任意）

これまでに学んだテクニックを適用してさらなる高速化を行い、上位入賞を目指しましょう。
