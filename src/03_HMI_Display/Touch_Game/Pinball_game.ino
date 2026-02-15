#include <TFT_eSPI.h>

TFT_eSPI tft;
uint16_t x = 0, y = 0;

// 遊戲狀態
enum GameState {
  IDLE,
  PLAYING,
  GAME_OVER
};

// 當前遊戲狀態
GameState gameState = IDLE;

// 板的座標和大小
int paddleX, paddleY;
int paddleWidth = 40;
int paddleHeight = 10;
int refreashCnt = 0;

// 球的座標和速度
int numBalls = 3;  // 初始球數
int ballX[3], ballY[3];  // 多個球的座標
int ballSpeedX[3] = {5, -3, 4};  // 不同球的水平速度
int ballSpeedY[3] = {5, 3, -4};  // 不同球的垂直速度
int ballRadius = 5;
TFT_eSprite buffer = TFT_eSprite(&tft);

// 遊戲難度
int difficulty = 1;  // 1: 簡單, 2: 中等, 3: 困難

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(3); // 調整屏幕方向
  tft.fillScreen(TFT_BLACK);

  // 設置初始板的位置
  paddleX = tft.width() / 2 - paddleWidth / 2;
  paddleY = tft.height() - 20;

  // 設置初始球的位置
  for (int i = 0; i < numBalls; i++) {
    ballX[i] = tft.width() / 2;
    ballY[i] = tft.height() / 2;
  }

  // 顯示初始畫面
  showStartScreen();
  // 在 setup 函數中初始化後台緩衝
  buffer.createSprite(tft.width(), tft.height());
}

void loop() {
  bool pressed = tft.getTouch(&x, &y);

  switch (gameState) {
    case IDLE:
      if (pressed) {
        gameState = PLAYING;
        tft.fillScreen(TFT_BLACK);
      }
      break;

    case PLAYING:
      updateGame();
      break;

    case GAME_OVER:
      if (pressed) {
        resetGame();
      }
      break;
  }
}

void updateGame() {
  // 清除整個螢幕
  tft.fillScreen(TFT_BLACK);

  // 移動板
  paddleX = x - paddleWidth / 2;
  // 確保板的位置在螢幕內
  paddleX = constrain(paddleX, 0, tft.width() - paddleWidth);

  // 移動球
  for (int i = 0; i < numBalls; i++) {
    ballX[i] += ballSpeedX[i];
    ballY[i] += ballSpeedY[i];
  }

  // 顯示板
  tft.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, TFT_WHITE);

  // 顯示多個球
  for (int i = 0; i < numBalls; i++) {
    tft.fillCircle(ballX[i], ballY[i], ballRadius, TFT_WHITE);
  }

  // 判斷球碰到邊界反彈
  for (int i = 0; i < numBalls; i++) {
    if (ballX[i] - ballRadius <= 0 || ballX[i] + ballRadius >= tft.width()) {
      ballSpeedX[i] = -ballSpeedX[i];
    }
    if (ballY[i] - ballRadius <= 0) {
      ballSpeedY[i] = -ballSpeedY[i];
    }
  }

  // 檢查球是否碰到板
  for (int i = 0; i < numBalls; i++) {
    if (ballY[i] + ballRadius >= paddleY && ballX[i] >= paddleX && ballX[i] <= paddleX + paddleWidth) {
      ballSpeedY[i] = -ballSpeedY[i];
    }
  }

  // 判斷球是否掉出畫面
  for (int i = 0; i < numBalls; i++) {
    if (ballY[i] - ballRadius >= tft.height()) {
      gameState = GAME_OVER;
      showGameOverScreen();
    }
  }
}

void showStartScreen() {
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, tft.height() / 2 - 20);
  tft.print("Touch to Start");
}

void showGameOverScreen() {
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, tft.height() / 2 - 20);
  tft.print("Game Over");
  tft.setCursor(10, tft.height() / 2 + 20);
  tft.print("Touch to Play Again");
}

void resetGame() {
  paddleX = tft.width() / 2 - paddleWidth / 2;
  paddleY = tft.height() - 20;
  for (int i = 0; i < numBalls; i++) {
    ballX[i] = tft.width() / 2;
    ballY[i] = tft.height() / 2;
  }
  gameState = PLAYING;
  tft.fillScreen(TFT_BLACK);
}
