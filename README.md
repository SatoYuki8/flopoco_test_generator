浮動小数点数がゼロ，無限大，正規化数，NaNのどれであるかチェックし，

FloPoCoのテストベンチに対応するための上位2ビットの情報を追加するプログラム．

makeでflotestgenができる

	./flotestgen input_sample.csv 5 10 (make do)でサンプル実行

	./flotestgen 入力ファイル 指数部のbit幅 仮数部のbit幅   
	※空行あるとセグるから注意※
	

	<上位2bitに追加される数値の意味>
	ゼロ　　： 00
	無限大　： 10
	正規仮数： 01
	NaN　　 ： 11


	入力データ例：
	1| 0000000000000000 0111110000000000
	2| 0110110010010110 0111110001001000
	:| :


	出力データ例：
	1| 000000000000000000 100111110000000000
	2| 010110110010010110 110111110001001000
	:| :
