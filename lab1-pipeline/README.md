# 演習 1

## パイプラインによる処理のタスク並列化

この演習では [bai-gaeshi](https://acri-vhls-challenge.web.app/challenge/bai-gaeshi3) を例に、ループのパイプライン化による高速化の効果を見てみます。bai-gaeshi は次の内容のチャレンジです。

```markdown
# 問題
入力されるfloat配列の要素の値をすべて倍にして出力しなさい。
ただし入力される値の範囲は [-1000, 1000) とします。
```

この問題の `kernel.cpp` にはリファレンスとしてあらかじめ次のコードが記述されています。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
  for (int i = 0; i < size; i++) {
#pragma HLS PIPELINE off
    out[i] = in[i] * 2;
  }
}
```

このコードはそのまま HLS チャレンジに投稿し採点をパスすることができます。

このコードではあえてパイプライン化を無効化するプラグマ `PIPELINE off` が指定されています。こうしないと Vitis HLS が自動的にループをパイプライン化してしまうためです。

### 演習 1-1

それではこのコードを高位合成してシミュレーションのサイクル数を確認してみましょう。この演習のディレクトリに移動して、`vhls-check` コマンドを実行します。`vhls-check` コマンドは HLS チャレンジの解答を採点するプログラムです。

```bash
$ cd lab1-pipeline
$ ./vhls-check -f
```

`-f` は過去に `vhls-check` を実行した結果（`work` ディレクトリ）を強制的に上書きするオプションです。

しばらくして次のように出力されたら成功です。ソースコードに問題がある場合は `CSim`、`HLS`、`CoSim` の結果のいずれかが `Fail` になります。

```
Bytes of kernel code: 267
CSim: Pass
HLS: Pass
CoSim: Pass
Resource usage
  FF   : 1605
  LUT  : 2503
  DSP  : 3
  BRAM : 0
  URAM : 0
Clock period (ns): 1.460
Clock frequency (MHz): 684
Simulation cycle: 11488
Simulation time (ns): 16772.480
```

シミュレーションサイクル数は `Simulation cycle` の項目を見てください。この例では 11,488 サイクルかかったと分かります。ループ回数が 1,024 ですので、ループ 1 回あたりに 11 サイクルくらいかかっている計算になります。

### 演習 1-2

パイプライン化を有効にしてサイクル数がどう変化するか見てみましょう。`kernel.cpp` をお好みのエディタで開いて、HLS プラグマ（`#pragma` の行）を削除してください。メモ帳で開く場合はターミナル上で次のコマンドを入力します。

```bash
$ gnome-text-editor kernel.cpp
```

`kernel.cpp` を変更して保存したら、もう一度 `vhls-check` コマンドを実行します。`-f` オプションを付けるのを忘れないようにしてください。

今度は次のように出力されます。

```
Bytes of kernel code: 242
CSim: Pass
HLS: Pass
CoSim: Pass
Resource usage
  FF   : 1679
  LUT  : 2637
  DSP  : 3
  BRAM : 0
  URAM : 0
Clock period (ns): 1.460
Clock frequency (MHz): 684
Simulation cycle: 1381
Simulation time (ns): 2016.260
```

サイクル数が 1,381 と大幅に減りました。パイプライン化によりループ 1 回あたりが見かけ上 1 サイクルで処理されるようになったためです。サイクル数が 1,024 より 300 ほど大きいのは、`in` 配列からデータを読む時間（レイテンシ）、シミュレーションが開始して回路が動き出すまでの時間、回路が停止してシミュレーションが終了するまでの時間などを含むためです。

### 演習 1-3

ここで HLS のログを見てみます。ログは `work/hls.log` に出力されています。お好みのテキストエディタで開いてみてください。すべての内容を理解する必要はありません。次のような出力があるところまで進んでください。

```
INFO: [HLS 200-10] ----------------------------------------------------------------
INFO: [HLS 200-42] -- Implementing module 'kernel_Pipeline_VITIS_LOOP_4_1'
INFO: [HLS 200-10] ----------------------------------------------------------------
INFO: [SCHED 204-11] Starting scheduling ...
INFO: [SCHED 204-61] Pipelining loop 'VITIS_LOOP_4_1'.
INFO: [HLS 200-1470] Pipelining result : Target II = NA, Final II = 1, Depth = 11, loop 'VITIS_LOOP_4_1'
INFO: [SCHED 204-11] Finished scheduling.
```

注目していただきたいのは `Final II = 1` の部分です。これを見ることによって、狙い通りにループが II=1 のパイプラインとして合成されたことが確認できます。

今回の例ではループがひとつのためログのどの部分を見ればよいかあまり迷うことはありませんが、回路が複雑になるとコード上のループとログの内容との対応関係が分かりにくくなることがあります。そのような場合はコード上でループに名前（ラベル）を付けることでログを分かりやすくすることができます。

例えば次のように `for` 文の直前に任意の名前のラベルを記述します。

```cpp
#include "kernel.hpp"

void kernel(const float in[1024], float out[1024], int size) {
myloop:
  for (int i = 0; i < size; i++) {
    out[i] = in[i] * 2;
  }
}
```

そうすると、変更前のログでは `kernel_Pipeline_VITIS_LOOP_4_1` などとなっていたモジュール（ループ）名が、`kernel_Pipeline_myloop` のように自分で付けた名前に変わり、対応関係が分かりやすくなります。


<hr>
<p align="center"><a href="..">トップへ戻る</a> － <a href="../lab2-unroll/">次の演習へ進む</a></p>
