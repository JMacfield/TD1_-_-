#include <Novice.h>
#include <stdlib.h>
#include <time.h>

const char kWindowTitle[] = "LC1C_04_イソガイユウト_タイトル";

int inputKeyUpdate(char &keys, char &preKeys) { //キーが押されている間フレーム数を核のする

	int i = 0;

	for (i = 0; i < 256; i++) {
		if (keys[&i] != 0) {
			keys[&i]++;
		}
		else {
			keys[&i] = 0;
		}
	}
	return 0;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 900, 640);
	

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int car = Novice::LoadTexture("./Resources/Character/Car.png"); //車（左から用）
	int track = Novice::LoadTexture("./Resources/Character/track.png"); //トラック（左から用）
	int cycle = Novice::LoadTexture("./Resources/Character/Cycle.png"); //自転車（左から用）

	int carInvert = Novice::LoadTexture("./Resources/Character/Car_Invert.png"); //車（右から用）
	int trackInvert = Novice::LoadTexture("./Resources/Character/Track_Invert.png"); //トラック（右から用）
	int cycleInvert = Novice::LoadTexture("./Resources/Character/Cycle_Invert.png"); //自転車（右から用）

	int background = Novice::LoadTexture("./Resources/Field/background.png"); //背景用

	int carDrawPos[9] = { 0 }, trackDrawPos[9] = { 0 }, cycleDrawPos[9] = { 0 }, drawTimer = 0, randomTimer = 0; //ドロー用ポジション（タイマー含む）
	int carPosX[9], trackPosX[9], cyclePosX[9]; //キャラクターの座標関連

	int playerPosX = 300, playerPosY = 576, playerSpeed = 64, playerSize = 64; //プレイヤー関連

	int blockSize = 64; //ブロックサイズ

	srand((unsigned)time(NULL)); 
	int random[9];

	for (int i = 0; i < 9; i++) {
		carPosX[i] = 0;
		trackPosX[i] = 0;
		cyclePosX[i] = 0;
	}

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		//プレイヤーの操作挙動

		if (keys[DIK_SPACE] >= 1){
			playerPosY -= blockSize;
		}

		for (int i = 0; i < 9; i++) {
			if (randomTimer > 120) {
				random[i] = rand() % 3 + 1;
				randomTimer = 0;
			/*	carPosX[i] = -170;
				trackPosX[i] = -170;
				cyclePosX[i] = -170;*/
				if (carPosX[i] == 1 && trackPosX[i] == 1 && cyclePosX[i] == 1) {
					break;
				}
			}

			if (carPosX[i] > 640) carPosX[i] = -170;
			if (trackPosX[i] > 640) trackPosX[i] = -170;
			if (cyclePosX[i] > 640) cyclePosX[i] = -170;

			if (playerPosY < 0) playerPosY = 576; //プレイヤーが上超えたら初期地点に戻る（デバッグ用）
			if (drawTimer > 240) carDrawPos[i] = 256, trackDrawPos[i] = 256, cycleDrawPos[i] = 128; //ドロータイマー（２枚目へ）
			if (drawTimer > 300) carDrawPos[i] = 0, trackDrawPos[i] = 0, cycleDrawPos[i] = 0, drawTimer = 0; //ドロータイマー（３枚目へ）
			if (drawTimer > 120) carDrawPos[i] = 128, trackDrawPos[i] = 128, cycleDrawPos[i] = 64; //ドロータイマー（１枚目へ）

			carPosX[i] += 1.0;
			trackPosX[i] += 1.0;
			cyclePosX[i] += 1.0;
		}

		drawTimer++;
		randomTimer++;

		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///
		
		//フィールドを動くイメージの描写処理
		Novice::DrawSprite(0, 0, background, 1.0, 1.0, 0.0, WHITE); //背景
		//Novice::DrawSpriteRect(carPosX, 54, carDrawPos, 0, 128, 64, car, 0.5, 1.0, 0.0, WHITE); //車（赤）
		//Novice::DrawSpriteRect(trackPosX, 118, trackDrawPos, 0, 128, 64, track, 0.5, 1.0, 0.0, WHITE); //車（大型）
		//Novice::DrawSpriteRect(cyclePosX, 182, cycleDrawPos, 0, 64, 64, cycle, 0.5, 1.0, 0.0, WHITE); //自転車

		//プレイヤーの描写
		Novice::DrawBox(playerPosX, playerPosY, playerSize, playerSize, 0.0, WHITE, kFillModeSolid);

		for (int i = 0; i < 9; i++) {
			if (random[i]== 1) {
				Novice::DrawSpriteRect(carPosX[i], 54, carDrawPos[i], 0, 128, 64, car, 0.5, 1.0, 0.0, WHITE); //車（赤）
			}
			else if (random[i] == 2) {
				Novice::DrawSpriteRect(trackPosX[i], 54, trackDrawPos[i], 0, 128, 64, track, 0.5, 1.0, 0.0, WHITE); //車（大型）
			}
			else {
				Novice::DrawSpriteRect(cyclePosX[i], 54, cycleDrawPos[i], 0, 64, 64, cycle, 0.5, 1.0, 0.0, WHITE); //自転車
			}
		}

		//デバッグ用プリント
		Novice::ScreenPrintf(680, 20, "drawTimer = %d", drawTimer);
		Novice::ScreenPrintf(680, 40, "playerPosY = %d", playerPosY);
		Novice::ScreenPrintf(680, 60, "random = %d", random);
		
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
