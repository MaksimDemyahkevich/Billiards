#include <SFML/Graphics.hpp>
#include "ball.h"
#include "map.h"


using namespace std;

void ball::controlWB(const int startX, const int startY, float time, bool& allBallStop, bool& isWBHidden, bool& isWBLocated, bool& isStickHidden) {
	if (allBallStop == true && isWBHidden == true && isWBLocated == false) { x = startX + mapWidth / 2; y = startY + mapHeight / 2 + 20; isWBHidden = false; }
	if (isWBLocated == false && allBallStop == true && isWBLocated == false) {
		isStickHidden = true;
		if (Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))) { Move(x, -1, time); }
		if (Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))) { Move(x, 1, time); }
		if (Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W))) { Move(y, -1, time); }
		if (Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S))) { Move(y, 1, time); }

		if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))) && Keyboard::isKeyPressed(Keyboard::LShift)) { Move(x, -3, time); }
		if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))) && Keyboard::isKeyPressed(Keyboard::LShift)) { Move(x, 3, time); }
		if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W))) && Keyboard::isKeyPressed(Keyboard::LShift)) { Move(y, -3, time); }
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S))) && Keyboard::isKeyPressed(Keyboard::LShift)) { Move(y, 3, time); }

		if ((Keyboard::isKeyPressed(Keyboard::Enter))) { isWBLocated = true; isStickHidden = false; }
	}
}

void ball::Move(float& coord, float speed, float time) {
	coord += speed * time;
}

void ball::reduceSpeed(float deceleration, float time) {
	// Âû÷èñëÿåì òåêóùóþ ñêîðîñòü êàê âåêòîðíóþ âåëè÷èíó
	float speed = sqrt(dx * dx + dy * dy);

	// Åñëè ñêîðîñòü áîëüøå íóëÿ, óìåíüøàåì å¸
	if (speed > 0) {
		// Âû÷èñëÿåì íîâîå çíà÷åíèå ñêîðîñòè ñ ó÷¸òîì çàìåäëåíèÿ
		speed = max(0.0f, speed - deceleration * time);

		// Âû÷èñëÿåì êîýôôèöèåíòû äëÿ ñêîðîñòè ïî îñÿì x è y
		float factor = speed / (sqrt(dx * dx + dy * dy));

		// Îáíîâëÿåì êîìïîíåíòû ñêîðîñòè
		dx *= factor;
		dy *= factor;
	}
	else {
		// if speed ball ~ 0 we stop it
		dx = 0;
		dx = 0;
	}
}

void handleWallCollision(ball& ball, const int startX, const int startY, const int mapWidth) {
	float f1 = ball.x - startX - 16;
	float f2 = ball.x - startX + 16;
	float f3 = -ball.x + startX + 336;
	float f4 = -ball.x + startX + 368;
	float f5 = -ball.x + startX + 464;
	float f6 = -ball.x + startX + 496;
	float f7 = ball.x - startX + 112;
	float f8 = ball.x - startX + 144;

	if ((ball.y + ball.r > 240 + startY) && (ball.y + ball.r < 258 + startY)) { return; }

	if ((ball.y - startY > f1 && ball.y - startY < f2) && (ball.x > startX && ball.x <= startX + 50)) { return; }
	if ((ball.y - startY > f3 && ball.y - startY < f4) && (ball.x > mapWidth + startX - 50 && ball.x < mapWidth + startX)) { return; }
	if ((ball.y - startY > f5 && ball.y - startY < f6) && (ball.x > startX && ball.x <= startX + 50)) { return; }
	if ((ball.y - startY > f7 && ball.y - startY < f8) && (ball.x > mapWidth + startX - 50 && ball.x < mapWidth + startX)) { return; }

	else {
		if ((ball.x + ball.r > 320 + startX) || (ball.x - ball.r < 32 + startX)) {
			ball.dx = -ball.dx;
			ball.x = max(ball.x, 32 + ball.r + startX);
			ball.x = min(ball.x, 320 - ball.r + startX);
		}
		if ((ball.y + ball.r > 448 + startY) || (ball.y - ball.r < 32 + startY)) {
			ball.dy = -ball.dy;
			ball.y = max(ball.y, 32 + ball.r + startY);
			ball.y = min(ball.y, 448 - ball.r + startY);
		}
	}
}

void handleBallCollision(ball& ball1, ball& ball2) {
	float Dx = ball1.x - ball2.x;
	float Dy = ball1.y - ball2.y;
	float d = sqrt(Dx * Dx + Dy * Dy);
	if (d == 0) d = 0.01;
	float s = Dx / d; // sin
	float e = Dy / d; // cos
	if (d < ball1.r + ball2.r) {
		float Vn1 = ball2.dx * s + ball2.dy * e;
		float Vn2 = ball1.dx * s + ball1.dy * e;
		float dt = (ball2.r + ball1.r - d) / (Vn1 - Vn2);
		if (dt > 0.6) dt = 0.6;
		if (dt < -0.6) dt = -0.6;
		ball1.x -= ball1.dx * dt;
		ball1.y -= ball1.dy * dt;
		ball2.x -= ball2.dx * dt;
		ball2.y -= ball2.dy * dt;
		Dx = ball1.x - ball2.x;
		Dy = ball1.y - ball2.y;
		d = sqrt(Dx * Dx + Dy * Dy);
		if (d == 0) d = 0.01;
		s = Dx / d; // sin
		e = Dy / d; // cos
		Vn1 = ball2.dx * s + ball2.dy * e;
		Vn2 = ball1.dx * s + ball1.dy * e;
		float Vt1 = -ball2.dx * e + ball2.dy * s;
		float Vt2 = -ball1.dx * e + ball1.dy * s;

		float o = Vn2;
		Vn2 = Vn1;
		Vn1 = o;

		ball1.dx = Vn2 * s - Vt2 * e;
		ball1.dy = Vn2 * e + Vt2 * s;
		ball2.dx = Vn1 * s - Vt1 * e;
		ball2.dy = Vn1 * e + Vt1 * s;
		ball1.x += ball1.dx * dt;
		ball1.y += ball1.dy * dt;
		ball2.x += ball2.dx * dt;
		ball2.y += ball2.dy * dt;

		// Ensure the balls are separated to avoid overlap
		float overlap = (ball1.r + ball2.r - d) / 2.0f;
		float correctionX = overlap * s;
		float correctionY = overlap * e;
		ball1.x += correctionX;
		ball1.y += correctionY;
		ball2.x -= correctionX;
		ball2.y -= correctionY;
	}
}

void MoveEveryBall(ball& whiteBall, ball* shar, float time, int n, float deceleration) {
	for (int i = 0; i < n; i++) {
		shar[i].Move(shar[i].x, shar[i].dx, time);
		shar[i].Move(shar[i].y, shar[i].dy, time);
		shar[i].reduceSpeed(deceleration, time);
	}
	whiteBall.Move(whiteBall.x, whiteBall.dx, time);
	whiteBall.Move(whiteBall.y, whiteBall.dy, time);
	whiteBall.reduceSpeed(deceleration, time);
}

void checkCollisionWithEveryrhing(int n, ball* shar, const int startX, const int startY, ball& whiteBall) {
	for (int i = 0; i < n; i++) {
		handleWallCollision(shar[i], startX, startY, mapWidth);
	}
	handleWallCollision(whiteBall, startX, startY, mapWidth);

	for (int i = 0; i < n; i++) {
		handleBallCollision(whiteBall, shar[i]);
		for (int j = i + 1; j < n; j++) {
			handleBallCollision(shar[i], shar[j]);
		}
	}
}

void drawAnythingBall(RenderWindow& window, ball* shar, int n, bool isWBHidden, ball whiteBall) {
	for (int i = 0; i < n; i++) {
		CircleShape shape(shar[i].r);
		shape.setRadius(shar[i].r);
		shape.setOrigin(shar[i].r, shar[i].r);
		shape.setPosition(shar[i].x, shar[i].y);
		shape.setFillColor(shar[i].color);
		window.draw(shape);
	}
	if (isWBHidden == false) {
		CircleShape shape(whiteBall.r);
		shape.setRadius(whiteBall.r);
		shape.setOrigin(whiteBall.r, whiteBall.r);
		shape.setPosition(whiteBall.x, whiteBall.y);
		shape.setFillColor(whiteBall.color);
		window.draw(shape);
	}
}
