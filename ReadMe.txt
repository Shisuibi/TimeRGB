
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
　Displays Standard time from a public Network Time Protocol server via Wi-Fi.
　Save the time Adjustment method (Wireless/Key operation) to Flash memory.
　Mood lighting with NeoPixel. (RGB color LED)

　ESP32-S3-Zeroと4桁7セグメントLEDで小型時計を作りましたｗ
　PC有線接続で簡単操作、電源接続だけでもBOOTボタンで操作可能です

　ラーメンタイマーとムードライトでTimeRGBと命名しました
　愛称は零（れい）ちゃんです

　WiFi経由でNTPサーバーから標準時を取得する自動時刻調整機能付き
　時刻調整の方法（無線／キー操作）をFlashメモリに保存します

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
　J2・J3　　　　｜　4桁7セグメントLED　0.36インチ　共通アノード／共通カソード両対応
　RN0 　　　　　｜　Bourns Inc　4116R-1-202LF　分離抵抗器（2[KΩ]）8素子

　（部品は全て基板に直付けです）

　4桁7セグメントLEDの共通アノード／共通カソードは自動識別されます
　分離抵抗器（2[KΩ]）は、LEDの定格（電気的・光学的特性）により適宜調節して下さい
　（実機外観の緑LEDには（1[KΩ]）・赤LEDには（2[KΩ]）を採用しています）

　低めの抵抗値を選択すると共通アノードは自動識別されなくなるので、
　ESP32スケッチ（SegLed.h）の変更による手動設定が必要です


★技術資料

　【小数点】
　LEDモードがクロック（時・分）表示の時、小数点は秒数の下位1桁を2進数で表示します
　秒数の上位1桁はNeoPixelの色で読み取る事が出来ます

　LEDモードがタイマー（分・秒）表示の時、小数点は0.2秒毎に増加します

　【NeoPixel】
　LEDモードがクロック表示の時、NeoPixelは60秒で一周します
　LEDモードがタイマー表示の時、NeoPixelは30秒で一周します

　赤→橙→緑→青→藍→菫と連続的に変化するムードライトです


★操作説明

　電源投入またはESP32リセット時、Flashメモリから以下の設定を読み込みます

　・4桁7セグメントLEDの初期表示モード（クロック／タイマー）
　・NeoPixel（RGB LED）の初期表示モード（消灯／点灯）
　・WiFi経由で標準時を取得する自動時刻調整（禁止／許可）

　自動時刻調整が許可設定なら、WiFi経由でNTPサーバーから標準時を取得します
　ESP32スケッチ（Function.h）のWiFiSSIDPSWDを設定して下さい（初回Wi-Fi接続時だけ）

　WiFiルーター接続中　…　4桁7セグメントLED表示が1秒間（上位2桁の回転）動きます
　NTP サーバー接続中　…　4桁7セグメントLED表示が1秒間（下位2桁の回転）動きます
　（接続出来ない場合は、1分後にタイムアウトします）


★キー操作＆システムメッセージ

　Ctrl＋A　｜　システム再起動　"ColdBoot[RST]"
　Ctrl＋S　｜　時刻設定（4桁の時刻を数字キーで入力）　"Set Time[NUM]"
　Ctrl＋D　｜　7セグLED（時刻／計時）　"7Seg LED[CLK]"　"7Seg LED[TMR]"
　Ctrl＋F　｜　NeoPixel（消灯／点灯）　"NeoPixel[OFF]"　"NeoPixel[ ON]"
　Ctrl＋G　｜　NTP WiFi（禁止／許可）　"NTP WiFi[DIS]"　"NTP WiFi[ENA]"
　Ctrl＋K　｜　ヘルプ画面表示（キー操作）
　Ctrl＋L　｜　テキスト画面消去

　Ctrl＋A（システム再起動）を押すと、クロック・消灯に切り替わります
　（時刻設定の途中だった場合は、数字キー入力が取り消されます）

　Ctrl＋S（時刻設定）を押すと、7セグメントLEDの表示が"----"に変わり、
　4桁の時刻を数字キーで入力出来ます（クロック／タイマー共通）

　Ctrl＋D（7セグLED）を1回押す（またはBOOTボタンを短押し）毎に、
　4桁7セグメントLEDの表示モード（クロック／タイマー）が切り替わります

　Ctrl＋F（NeoPixel）を1回押す（またはBOOTボタンを2回短押し）毎に、
　NeoPixel（RGB LED）の表示モード（消灯／点灯）が切り替わります

　Ctrl＋G（NTP WiFi）を1回押す（またはBOOTボタンを1秒長押し）毎に、
　WiFi経由で標準時を取得する自動時刻調整（禁止／許可）が切り替わります

　NTP WiFi禁止　…　4桁7セグメントLED表示が1秒間（上から下に）動きます
　NTP WiFi許可　…　4桁7セグメントLED表示が1秒間（下から上に）動きます

　NTP WiFiを変更した時、LED表示モードも一緒にFlashメモリに保存されます
　（次回電源投入またはESP32リセット時の初期表示モードになります）

