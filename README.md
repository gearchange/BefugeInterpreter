# Befugeインタープリタ

## これなに

2次元を縦横無尽に駆け巡るプログラミング言語、Befugeのインタープリタです。

## 各所頭悪すぎでしょ

ワカル

## なんで幾つか実装されてないところがあるん

```c
case '&': //ユーザに数値を入力させ、その値をスタックにプッシュする
	break; //未実装というかあえて実装しない
case '~': //ユーザに1文字入力させ、その文字のASCIIコードをスタックにプッシュする
	break; //未実装というかあえて実装しない
```

ここやな。ユーザーに入出力されるのは移植されたりするのを前提に考えているのであえて実装していない。
環境依存も激しかったりするわけだし。（言い訳）

というわけで各自実装してくれよな。

## ﾗｲｾｾｯｽｽｽﾝﾝｰ

ライセンスはMITライセンス。

## スシパワー

バイオスシを食べたのでバグがだいたい無くなった。と思う。
