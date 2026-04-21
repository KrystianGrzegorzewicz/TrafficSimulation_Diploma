#include "TJunction.h"
#include <random>
#include <iostream>

TJunction::TJunction(float cx, float cy) {

    float entryOffset = 300.0f;   // skąd startują auta
    float exitOffset = 300.0f;   // jak daleko kończą

    Vec2 entryLeft(cx - entryOffset, cy);

    Vec2 center(cx, cy);

    Vec2 exitRight(cx + exitOffset, cy);
    Vec2 exitDown(cx, cy + exitOffset);

    // ==============================
    // PAS 1 — jazda prosto
    // ==============================
    Lane straight(entryLeft, exitRight);
    addLane(straight);

    // ==============================
    // PAS 2 — skręt w prawo (łamany)
    // robimy 2 pasy segmentowe:
    // wjazd do skrzyżowania → zjazd w dół
    // ==============================
    Lane turnPart1(entryLeft, center);
    Lane turnPart2(center, exitDown);

    addLane(turnPart1);
    addLane(turnPart2);
}
TJunction::TJunction() {
    blocks.push_back(Block(40.12f, 13.99f, 40.12f + 31.86f, 13.99f + 2.68f));
    blocks.push_back(Block(39.97f, 24.05f, 39.97f + 11.43f, 24.05f + 0.96f));
    blocks.push_back(Block(45.14f, 29.19f, 45.14f + 0.96f, 29.19f + 0.96f));
	std::vector<Vec2> travelPoints1 = { Vec2(2.21f, 19.6f), Vec2(46.88f, 19.6f), Vec2(61.55f, 19.6f),Vec2(104.0f, 19.6f) };
	travels.push_back(Travel(travelPoints1));
    std::vector<Vec2> travelPoints2 = { Vec2(2.21f, 19.6f), Vec2(46.88f, 19.6f), Vec2(52.0f, 24.0f),Vec2(52.0f, 50.0f) };
    travels.push_back(Travel(travelPoints2));
}
std::vector<Block> TJunction::getBlocks() {
    return blocks;
}
Travel TJunction::getRandomTravel() {
	return travels[0];
}