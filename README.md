東方文花帖の自動AIのやつです。

main .. dll injection させるやつ。これ単体でコンパイルする。
dlltest .. dll側の本体。AIとかとの間を受け持ってる。
debug_window .. dllから飛んできたHDCを描画するだけのもの。あと、ウィンドウの生成とかもする。
rensya_window .. ゲーム側のHDCを取ってきて返すやつ。
reverser .. 弾位置とかを確認するひと。
ai_tes .. AIのプロトタイプ。勢いで書いてるのでデファクタリングしたい。
defines .. odsの描画の可否を決める。

実演:
https://youtu.be/e-9-gPbwMNE
https://youtu.be/Flar8kTm6sE
https://youtu.be/vd5RLdCYJKo
