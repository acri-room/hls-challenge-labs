# HLS チャレンジ演習テキスト

## 準備

ACRi ルームにリモートデスクトップまたは ssh で接続してください。

このテキストのコピーは as101~as105 サーバーの `/tools/repo/acri-room/hls-challenge-labs` にあります。各自のホームディレクトリ等にコピーして演習を進めてください。

```bash
$ cd ~
$ cp -a /tools/repo/acri-room/hls-challenge-labs .
$ cd hls-challenge-labs/lab1-pipeline
```

## 演習

1. [パイプラインによる処理のタスク並列化](./lab1-pipeline)
2. [回路の並列化](./lab2-unroll)
3. [インターフェースの軽量化](./lab3-interface)
4. [累積演算の高速化](./lab4-accum)
5. [キャッシュ](./lab5-cache)
6. [アルゴリズム](./lab6-algo)

## 参考

- [ACRi ルームの FPGA 利用環境の予約・使用方法](https://gw.acri.c.titech.ac.jp/wp/manual/how-to-reserve)
- [Vitis 高位合成ユーザーガイド（UG1399）：HLS プラグマ](https://docs.xilinx.com/r/2022.1-Japanese/ug1399-vitis-hls/HLS-%E3%83%97%E3%83%A9%E3%82%B0%E3%83%9E)
