
//=======================================================//
//　　　Portable NTP Clock Timer Mood Light "TimeRGB"　　//
//　 Implemented by Shisuibi --Grand Master Sorcerian--　//
//=======================================================//


★関連動画

　Crystal Palace（PC-84C0SD 20MHz 紹介サイト）
　　https://crystalpalace.web.fc2.com/

　祈宮翠微 YouTube チャンネル
　　https://www.youtube.com/channel/UCD1ukbXROQYn7PtXcsfblfQ


★概要

　A small and simple Clock/Timer using ESP32-S3-Zero and 4-Digit 7-Segment LED.
　Displays standard time from a public Network Time Protocol server via Wi-Fi.
　Save the time Adjustment method (Wireless/Key operation) to flash memory.
　Mood lighting with NeoPixel. (RGB color LED)

　ESP32-S3-Zeroと4桁7セグメントLEDで小型時計を作りましたｗ
　PC有線接続で簡単操作、電源接続だけでもBOOTボタンで利用可能

　ラーメンタイマーとムードライトでTimeRGBと命名しました
　愛称は零（れい）ちゃんです

　WiFi経由でNTPサーバーから標準時を取得する自動時刻調整機能付き
　時刻調整の方法（無線・キー操作）をFlashメモリに保存します

　【注意】技適マークが無い無線機器を日本国内で使用すると
　　　　　電波法違反になる場合があります

　[Caution] Using wireless devices without a Technical Conformity Mark
　　　　　　in Japan may be a violation of the Radio Law


★ダウンロード

　【TimeRGB　零（れい）】
　　https://github.com/Shisuibi/TimeRGB


★ファイル一覧

　＋―　TimeRGB 　　　　　　　　　　　…　TimeRGB.zip
　　＋―　Gerber　　　　　　　　　　　…　プリント基板の製造データ
　　　｜―　TimeRGB-B_Cu.gbr　　　　　…　導体層（裏面・第4層・+3.3[V]）
　　　｜―　TimeRGB-B_Mask.gbr　　　　…　ソルダーマスク（裏面・紫）
　　　｜―　TimeRGB-B_Silkscreen.gbr　…　シルクスクリーン（裏面・白）
　　　｜―　TimeRGB-Edge_Cuts.gbr 　　…　基板外形（60[mm]×23[mm]）
　　　｜―　TimeRGB-F_Cu.gbr　　　　　…　導体層（表面・第1層・GND）
　　　｜―　TimeRGB-F_Mask.gbr　　　　…　ソルダーマスク（表面・紫）
　　　｜―　TimeRGB-F_Silkscreen.gbr　…　シルクスクリーン（表面・白）
　　　｜―　TimeRGB-In1_Cu.gbr　　　　…　導体層（内部・第2層・縦配線）
　　　｜―　TimeRGB-In2_Cu.gbr　　　　…　導体層（内部・第3層・横配線）
　　　｜―　TimeRGB-NPTH.drl　　　　　…　ドリル（ノンスルーホール）
　　　｜―　TimeRGB-PTH.drl 　　　　　…　ドリル（メッキスルーホール）
　　＋―　Sketch　　　　　　　　　　　…　Arduino IDE開発環境
　　　｜―　Clock.h 　　　　　　　　　…　定義ファイル（クロック）
　　　｜―　Function.h　　　　　　　　…　定義ファイル（基本機能）
　　　｜―　Global.h　　　　　　　　　…　定義ファイル（広域）
　　　｜―　NeoPix.h　　　　　　　　　…　定義ファイル（NeoPixel）
　　　｜―　SegLed.h　　　　　　　　　…　定義ファイル（7セグLED）
　　　｜―　TimeRGB.c 　　　　　　　　…　ESP32スケッチ（拡張子を.inoに変更して下さい）
　　　｜―　Trans.h 　　　　　　　　　…　定義ファイル（シリアル通信）
　　｜―　LICENSE 　　　　　　　　　　…　利用許諾書
　　｜―　README.htm　　　　　　　　　…　説明画像
　　｜―　ReadMe.txt　　　　　　　　　…　説明書


★設計仕様

　　　　　名称　｜　TimeRGB
　　　　　愛称　｜　零（れい）
　　入出力装置　｜　ESP32-S3-Zero
　　　表示装置　｜　4桁7セグメントLED　0.36インチ

　　設計ソフト　｜　KiCad EDA 9.0.5
　プリント基板　｜　60[mm]×23[mm]　4層基板（1層:GND 2層:縦配線 3層:横配線 4層:+3.3[V]）
　　　表面処理　｜　ソルダーマスク（紫）　シルクスクリーン（白）
　　銅箔仕上げ　｜　ENIG（無電解ニッケル金フラッシュ）


★部品リスト

　J0・J1　　　　｜　Waveshare ESP32-S3-Zero
　J2・J3　　　　｜　4桁7セグメントLED　0.36インチ　共通アノード・共通カソード両対応
　RN0 　　　　　｜　Bourns Inc　4116R-1-202LF　分離抵抗器（2[KΩ]）8素子

　4桁7セグメントLEDの共通アノード・共通カソードは自動識別されます
　（部品は全て基板に直付けです）


プリント基板製造進行中
Printed circuit board manufacturing in progress

近日公開
Coming Soon

