#include "TJunction.h"

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