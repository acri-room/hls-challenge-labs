# 演習 2

## 回路の並列化

この演習では演習 1 に引き続き [bai-gaeshi2](https://acri-vhls-challenge.web.app/challenge/bai-gaeshi2) を例に、`UNROLL` プラグマを使って回路を並列化する方法を学びます。

`kernel.cpp` はループをパイプライン化した後の次の内容であるとします。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in[i] * 2;
  }
}
```

### 演習2-1

ループ内の回路を並列化するには `UNROLL` プラグマを使用します。次のようにしてループに `UNROLL` プラグマを適用し、サイクル数にどのような変化があるか確認してください。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
#pragma HLS UNROLL factor=4
    out[i] = in[i] * 2;
  }
}
```

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  上記のコードを採点すると次の結果になります。

  ```
  Bytes of kernel code: 270
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 2057
    LUT  : 3200
    DSP  : 3
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.460
  Clock frequency (MHz): 684
  Simulation cycle: 1381
  Simulation time (ns): 2016.260
  ```
  
  回路を並列化したのにもかかわらず、サイクル数が短縮していません。これはループ回数が未知のとき、配列アクセスを行うインターフェースが並列化されないためです。
</details>

### 演習2-2

`UNROLL` プラグマに `skip_exit_check` オプションを付けて終了条件チェックを削除することで、ツールがインターフェースを並列化できるようになります。

プラグマを次のように変更して結果を確認してください。

```cpp
#pragma HLS UNROLL factor=4 skip_exit_check
```

> :warning: この問題（[bai-gaeshi2](https://acri-vhls-challenge.web.app/challenge/bai-gaeshi2)）で `UNROLL` プラグマに `skip_exit_check` オプションを付けて高速化できるのは、テストベンチ（`tb.cpp`）内で、`out` 配列の `size` 以降の要素をチェックしていないためです。もし問題の要件に `size` 以降の要素を変更してはならないとありテストベンチ側でチェックが行われていれば答え合わせで `Fail` します。

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  `UNROLL` プラグマに `skip_exit_check` オプションを適用するとサイクル数が短縮します。

  ```
  Bytes of kernel code: 286
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 3388
    LUT  : 4454
    DSP  : 12
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.460
  Clock frequency (MHz): 684
  Simulation cycle: 517
  Simulation time (ns): 754.820
  ```
</details>

### 演習2-3（任意）

上記の例では `factor=4` として回路を 4 並列にしました。並列数をさらに大きくするとどうなるでしょうか。

<hr>
<p align="center"><a href="..">トップへ戻る</a> － <a href="../lab3-interface/">次の演習へ進む</a></p>