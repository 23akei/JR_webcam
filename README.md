# JR_webcam

# 外部と関連のあるトピック
- <std_msgs::Int32>search_mode
  * 制御の方法を指示する。
  * 0の時は通常制御。
  * 1,2の時に追跡。

# launchファイル
- launch/tracking.launch
    * このパッケージ内のノードをすべて起動する。

# 内部で行う処理
- カメラからの画像（映像）によるボールの検出
- ボールが画面の中心から外れてしまった時の軌道修正
- （通常時用）自立走行プログラムからの切り替え（指示）

# パラメータ
- src/tracking_ball.cpp
  * FRAME_CENTER 
    + カメラの中央とみなす範囲
    + カメラフレームの中央からのピクセル数。
  * int margin
    + カメラフレームの端からのピクセル数
    + この範囲にボールの重心が来た場合に追跡を行う
  * SIKII
    + この数以上のピクセルに追跡する色があった場合に追跡モードを変更する。
    + この値を超えると後退をやめ、左右の位置を調整しながら前進する。
  
- src/camera_find_ball_pub.cpp
  * B_MIN
  * G_MIN
  * R_MIN
  * B_MAX
  * G_MAX
  * R_MAX
    + 抽出する色（追跡するボールの色）の範囲を指定

- launch/tracking.launch
  * tf_twist/speed
    + tf_twistに出力する最高速度