#pragma once

class Map
{
    char* printMap();

    class Point {
        enum class PointType {
            Empty,
            Wall,
            Attacker,
            Defender
        };
    private:
        friend Map;
        Point(int x, int y);
        int x, y;
    };
};