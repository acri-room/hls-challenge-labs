# 演習 4

## 累積演算の高速化

この演習では [simple-accum](https://acri-vhls-challenge.web.app/challenge/simple-accum3) を題材として用い、累積演算がボトルネックとなっているループを高速化する方法を学びます。

`simple-accum` の問題文は次です。

```markdown
# 問題
入力されるfloat配列 in の先頭から size 個の要素の値をすべて足し合わせ out に出力してください。ただし、以下であることとします。
- 入力される値の範囲は [-1000, 1000) です
- size で与えられる値の個数は 1 以上 1024 以下です
- 0.1 以下の誤差は許容します
```

リファレンスコードはこちらです。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], const int size, float *out) {
  float sum = 0;
  for (int i = 0; i < size; i++) {
    sum += in[i];
  }
  *out = sum;
}
```

### 演習 4-1

まずはリファレンスコードをそのまま採点してサイクル数を確認し、結果について考察しましょう。

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  リファレンスコードを採点すると次の結果になります。

  ```
  Bytes of kernel code: 267
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 1800
    LUT  : 2770
    DSP  : 2
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.532
  Clock frequency (MHz): 652
  Simulation cycle: 13531
  Simulation time (ns): 20729.492
  ```
  
  ループがパイプライン化されているにもかかわらず、ループ 1 回あたり 13 サイクルほどかかっていることが分かります。累積演算（float どうしの足し算）がボトルネックになってパイプラインの II（Initiation Interval）が 13 になったためです。これは HLS のログで確認できます。

  ```
  Pipelining result : Target II = NA, Final II = 13, Depth = 16, loop 'VITIS_LOOP_5_1'
  ```

  `simple-accum` で設定されているクロック周期は 2ns で、これから計算するとクロック周波数は 500MHz です。上記は 500MHz で float の加算を行うには 13 サイクルほどかかることを意味します。この float 加算器のレイテンシを短縮することはできませんので、加算器の数を増やして性能を向上することを考えます。
</details>

### 演習 4-2

試しに累積演算を 4 並列にしてサイクル数がどうなるか確認してみましょう。ループに `UNROLL` プラグマを適用するだけではうまくいきません。並列数に合わせて累積対象の変数も増やす必要があることにご注意ください。

<details>
  <summary>コード例はこちら</summary>
  
  並列数を 4 にした場合がこちらです。

  ```cpp
  #include "kernel.hpp"

  void kernel(const float in[1024], const int size, float *out) {
    float sum[4] = {};
    for (int i = 0; i < size; i++) {
  #pragma HLS UNROLL factor=4 skip_exit_check
      sum[i % 4] += in[i];
    }
    *out = sum[0] + sum[1] + sum[2] + sum[3];
  }
  ```
</details>

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  上記のコード例を採点すると次の結果になります。

  ```
  Bytes of kernel code: 337
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 2665
    LUT  : 3793
    DSP  : 2
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.532
  Clock frequency (MHz): 652
  Simulation cycle: 3604
  Simulation time (ns): 5521.328
  ```

  並列化した分だけ高速化したことが分かります。それと同時に演算器を並列化したことでリソース使用量も増えていることが分かります。
</details>

### 演習 4-3（任意）

レイテンシの観点で並列数はいくつが最適でしょうか。考えてみてください。コードを変更し確かめてみましょう。

<hr>
<p align="center"><a href="..">トップへ戻る</a> － <a href="../lab5-cache/">次の演習へ進む</a></p>
