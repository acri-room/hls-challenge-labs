# 演習 3

## インターフェースの軽量化

この演習では演習 2 に引き続き [bai-gaeshi2](https://acri-vhls-challenge.web.app/challenge/bai-gaeshi2) を題材として用います。

次のコードを起点に `INTERFACE` プラグマと `ARRAY_PARTITION` プラグマを使ってインターフェースを軽量化する方法を学びます。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in[i] * 2;
  }
}
```

### 演習3-1

`INTERFACE` プラグマを使用すると、カーネル引数をどのようなインターフェースとして実現するかを指定することができます。配列引数に対して特に何も指定しない場合は AXI-MM が選択されます。

次のように `INTERFACE` プラグマを適用することでインターフェースを AXI-Stream（AXI-S）に指定できます。コードを変更し効果を確認してください。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
#pragma HLS INTERFACE mode=axis port=in 
#pragma HLS INTERFACE mode=axis port=out
  for (int i = 0; i < size; i++) {
    out[i] = in[i] * 2;
  }
}
```

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  上記のコードを採点すると次のような結果になります。

  ```
  Bytes of kernel code: 325
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 377
    LUT  : 304
    DSP  : 3
    BRAM : 0
    URAM : 0
  Clock period (ns): 2.246
  Clock frequency (MHz): 445
  Simulation cycle: 1065
  Simulation time (ns): 2391.990
  ```
  
  インターフェースが AXI-MM のときは 1,381 サイクルでしたが、AXI-S に変更することで 1,065 サイクルまで短縮することが分かりました。
</details>

### 演習3-2

インターフェースを AXI-S に変更した配列引数へのアクセスは自動で並列化されません。このため次のようにループ内の回路だけを並列化してもサイクル数は短縮しません。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
#pragma HLS INTERFACE mode=axis port=in 
#pragma HLS INTERFACE mode=axis port=out
  for (int i = 0; i < size; i++) {
#pragma HLS UNROLL factor=4 skip_exit_check
    out[i] = in[i] * 2;
  }
}
```

AXI-S インターフェースの配列引数は `ARRAY_PARTITION` プラグマで明示的に並列化する必要があります。次のように `in`、`out` に対して `ARRAY_PARTITION` プラグマを適用し、効果を確認してください。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
#pragma HLS INTERFACE mode=axis port=in 
#pragma HLS INTERFACE mode=axis port=out
#pragma HLS ARRAY_PARTITION variable=in  type=cyclic factor=4 dim=1 
#pragma HLS ARRAY_PARTITION variable=out type=cyclic factor=4 dim=1
  for (int i = 0; i < size; i++) {
#pragma HLS UNROLL factor=4 skip_exit_check
    out[i] = in[i] * 2;
  }
}
```

> :information_source: `ARRAY_PARTITION` プラグマは配列を分割します。対象がメモリであれば、メモリを分割することで配列上の複数箇所を同時にアクセスできるようになるなどの利点があります。今回の場合は AXI-S を分割して同時に複数個のデータを読めるようにする目的で使用しています。

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  上記のコードを採点すると次のような結果になります。

  ```
  Bytes of kernel code: 506
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 1385
    LUT  : 907
    DSP  : 12
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.291
  Clock frequency (MHz): 774
  Simulation cycle: 302
  Simulation time (ns): 389.882
  ```
  
  ループ内の回路とインターフェースの両方が並列化され、サイクル数が短縮することが分かりました。
</details>

### 演習 3-3

ここまでの成果を HLS チャレンジに投稿してみましょう。

- https://acri-vhls-challenge.web.app/challenge/bai-gaeshi2

### 演習 3-4（任意）

並列数を大きくして上位入賞に挑戦してみましょう。

<hr>
<p align="center"><a href="..">トップへ戻る</a> － <a href="../lab4-accum/">次の演習へ進む</a></p>