# 演習 5

## キャッシュ

この演習では [simple-sort](https://acri-vhls-challenge.web.app/challenge/simple-sort3) を題材として用い、配列引数へのアクセスがボトルネックとなっているカーネルを、キャッシュを使って高速化する方法を学びます。

`simple-sort` の問題文は次です。

```markdown
# 問題
配列 in で与えられた 128 個のデータを昇順で並び替えて out に格納してください。
```

リファレンスコードは次です。`SIZE` はヘッダーファイルで 128 と定義されています。

```cpp
#include "kernel.hpp"

void kernel(const int in[SIZE], int out[SIZE]) {
  // Copy
  for (int i = 0; i < SIZE; i++) {
    out[i] = in[i];
  }

  // Bubble sort
  for (int i = 0; i < SIZE-1; i++) {
    for (int j = SIZE-1; j > i; j--) {
      if (out[j] < out[j-1]) {
        int t = out[j];
        out[j] = out[j-1];
        out[j-1] = t;
      }
    }
  }
}
```

このコードではデータの並び替えにバブルソートを使用しています。二重ループの中で `out` 配列引数に頻繁にアクセスしています。カーネルから配列引数へのアクセスは時間がかかるため非効率です。

### 演習 5-1

リファレンスコードのサイクル数を確認してください。

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  リファレンスコードを採点すると次の結果になります。

  ```
  Bytes of kernel code: 462
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 9549
    LUT  : 20307
    DSP  : 0
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.460
  Clock frequency (MHz): 684
  Simulation cycle: 1264523
  Simulation time (ns): 1846203.580
  ```
  
  二重ループ内の処理はおよそ 128 * 128 / 2 = 8,192回実行されますが、それに比べて大きなサイクル数となっています。
</details>

### 演習 5-2

`out` 配列上でデータを並び替える代わりに、カーネル内で定義した配列上で並び替えを行い、結果を `out` にコピーするようにコードを修正してください。サイクル数がどのように変化するか確認してください。

<details>
  <summary>コード例はこちら</summary>
  
  カーネル内に `tmp` 配列を作り、`in` の内容をコピーしています。`tmp` 配列上でデータを並び替え、結果を `out` にコピーしています。

  ```cpp
  #include "kernel.hpp"

  void kernel(const int in[SIZE], int out[SIZE]) {
    // Copy
    int tmp[SIZE];
    for (int i = 0; i < SIZE; i++) {
      tmp[i] = in[i];
    }

    // Bubble sort
    for (int i = 0; i < SIZE-1; i++) {
      for (int j = SIZE-1; j > i; j--) {
        if (tmp[j] < tmp[j-1]) {
          int t = tmp[j];
          tmp[j] = tmp[j-1];
          tmp[j-1] = t;
        }
      }
    }

    // Copy
    for (int i = 0; i < SIZE; i++) {
      out[i] = tmp[i];
    }
  }
  ```
</details>

<details>
  <summary>確認が終わったらここをクリックして答え合わせ</summary>
  
  上記のコード例を採点すると次の結果になります。

  ```
  Bytes of kernel code: 551
  CSim: Pass
  HLS: Pass
  CoSim: Pass
  Resource usage
    FF   : 6830
    LUT  : 10223
    DSP  : 0
    BRAM : 0
    URAM : 0
  Clock period (ns): 1.460
  Clock frequency (MHz): 684
  Simulation cycle: 25235
  Simulation time (ns): 36843.100
  ```

  サイクル数が大幅に短縮されました。
</details>

<hr>
<p align="center"><a href="..">トップへ戻る</a> － <a href="../lab6-algo/">次の演習へ進む</a></p>
