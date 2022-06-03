#pragma once
struct CEffects{
    explicit CEffects(int slowEffect = 0, int fireEffect = 0): m_slowEffect(slowEffect), m_fireEffect(fireEffect){}
    int m_slowEffect;
    int m_fireEffect;
    // to be added
};

struct terminal{
    int height{};
    int width{};
};