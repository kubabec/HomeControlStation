#ifndef SINGLE_WAVE_ANIMATION_HPP
#define SINGLE_WAVE_ANIMATION_HPP

#include "devices/LedStrip/Animations/ILiveAnimation.hpp"
#include <cstdint>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>

// 1. ILiveAnimation – interfejs dla animacji "żyjących"
class ILiveAnimation {
protected:
    uint16_t ledsCount = 0;
    LedColor* mainStrip = nullptr;
    std::vector<LedColor> savedState;
    bool initialized = false;
    bool running = false;

public:
    virtual ~ILiveAnimation() {}

    virtual void initialize(LedColor* stripPtr, uint16_t count) {
        mainStrip = stripPtr;
        ledsCount = count;
        savedState.assign(stripPtr, stripPtr + count);
        initialized = true;
    }

    virtual void start() {
        if (!initialized) return;
        running = true;
        onStart();
    }

    virtual void process() = 0;

    virtual void stop() {
        if (!initialized) return;
        running = false;
        std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
        onStop();
    }

    bool isRunning() const { return running; }

protected:
    virtual void onStart() {}
    virtual void onStop() {}
};

// 2. SingleWaveAnimation – przesuwa blok średniego koloru na tle przyciemnionych oryginałów bez zmiany wyłączonych
class SingleWaveAnimation : public ILiveAnimation {
public:
    enum class Direction { LeftToRight, RightToLeft };

    SingleWaveAnimation(uint16_t width, Direction dir, float dimFactor = 0.3f)
        : width(width), dir(dir), dimFactor(dimFactor) {}
    ~SingleWaveAnimation() override = default;

    void initialize(LedColor* stripPtr, uint16_t count) override {
        ILiveAnimation::initialize(stripPtr, count);
        computeAverageColor();
    }

    void onStart() override {
        pos = (dir == Direction::LeftToRight ? 0 : int(ledsCount) - int(width));
    }

    void process() override {
        if (!running) return;
        // dim and preserve zeros
        for (uint16_t i = 0; i < ledsCount; ++i) {
            const auto &orig = savedState[i];
            if (orig.r == 0 && orig.g == 0 && orig.b == 0) {
                mainStrip[i] = LedColor(0,0,0);
            } else {
                mainStrip[i].r = uint8_t(orig.r * dimFactor);
                mainStrip[i].g = uint8_t(orig.g * dimFactor);
                mainStrip[i].b = uint8_t(orig.b * dimFactor);
            }
        }
        // overlay moving block
        for (uint16_t i = 0; i < width; ++i) {
            int idx = (dir == Direction::LeftToRight ? pos + i : pos - i);
            if (idx < 0 || idx >= int(ledsCount)) continue;
            // only apply to originally non-zero
            const auto &orig = savedState[idx];
            if (orig.r || orig.g || orig.b) {
                mainStrip[idx] = averageColor;
            }
        }
        advance();
    }

protected:
    void onStop() override {}

private:
    uint16_t width;
    Direction dir;
    float dimFactor;
    int pos = 0;
    LedColor averageColor{0,0,0};

    void computeAverageColor() {
        uint32_t sumR = 0, sumG = 0, sumB = 0;
        uint32_t countNonZero = 0;
        for (auto &c : savedState) {
            if (c.r || c.g || c.b) {
                sumR += c.r;
                sumG += c.g;
                sumB += c.b;
                ++countNonZero;
            }
        }
        if (countNonZero == 0) countNonZero = 1;
        averageColor.r = uint8_t(sumR / countNonZero);
        averageColor.g = uint8_t(sumG / countNonZero);
        averageColor.b = uint8_t(sumB / countNonZero);
    }

    void advance() {
        if (dir == Direction::LeftToRight) {
            if (++pos > int(ledsCount - width)) {
                dir = Direction::RightToLeft;
                pos = ledsCount - width;
            }
        } else {
            if (--pos < 0) {
                dir = Direction::LeftToRight;
                pos = 0;
            }
        }
    }
};

// 3. SmoothWaveAnimation – łagodna fala z sinusoidalnym wygładzaniem krawędzi, bez psucia zero
class SmoothWaveAnimation : public ILiveAnimation {
public:
    enum class Direction { LeftToRight, RightToLeft };

    SmoothWaveAnimation(uint16_t width, Direction dir, float dimFactor = 0.3f)
        : width(width), dir(dir), dimFactor(dimFactor) {}
    ~SmoothWaveAnimation() override = default;

    void initialize(LedColor* stripPtr, uint16_t count) override {
        ILiveAnimation::initialize(stripPtr, count);
        computeAverageColor();
    }

    void onStart() override {
        pos = (dir == Direction::LeftToRight ? -int(width) : int(ledsCount));
    }

    void process() override {
        if (!running) return;
        // dim and preserve zeros
        for (uint16_t i = 0; i < ledsCount; ++i) {
            const auto &orig = savedState[i];
            if (orig.r == 0 && orig.g == 0 && orig.b == 0) {
                mainStrip[i] = LedColor(0,0,0);
            } else {
                mainStrip[i].r = uint8_t(orig.r * dimFactor);
                mainStrip[i].g = uint8_t(orig.g * dimFactor);
                mainStrip[i].b = uint8_t(orig.b * dimFactor);
            }
        }
        // smooth wave: distance-based blend
        for (uint16_t i = 0; i < ledsCount; ++i) {
            int dist = (dir == Direction::LeftToRight ? i - pos : pos - i);
            float t = 1.0f - std::abs(dist) / float(width);
            if (t <= 0.0f) continue;
            float factor = std::sin(t * float(M_PI));
            // apply only if orig non-zero
            const auto &orig = savedState[i];
            if (orig.r || orig.g || orig.b) {
                uint8_t dr = mainStrip[i].r;
                uint8_t dg = mainStrip[i].g;
                uint8_t db = mainStrip[i].b;
                mainStrip[i].r = uint8_t(dr * (1 - factor) + averageColor.r * factor);
                mainStrip[i].g = uint8_t(dg * (1 - factor) + averageColor.g * factor);
                mainStrip[i].b = uint8_t(db * (1 - factor) + averageColor.b * factor);
            }
        }
        advance();
    }

protected:
    void onStop() override {}

private:
    uint16_t width;
    Direction dir;
    float dimFactor;
    int pos = 0;
    LedColor averageColor{0,0,0};

    void computeAverageColor() {
        uint32_t sumR = 0, sumG = 0, sumB = 0;
        uint32_t countNonZero = 0;
        for (auto &c : savedState) {
            if (c.r || c.g || c.b) {
                sumR += c.r;
                sumG += c.g;
                sumB += c.b;
                ++countNonZero;
            }
        }
        if (countNonZero == 0) countNonZero = 1;
        averageColor.r = uint8_t(sumR / countNonZero);
        averageColor.g = uint8_t(sumG / countNonZero);
        averageColor.b = uint8_t(sumB / countNonZero);
    }

    void advance() {
        if (dir == Direction::LeftToRight) {
            if (++pos >= int(ledsCount + width)) pos = -int(width);
        } else {
            if (--pos < -int(width)) pos = int(ledsCount);
        }
    }
};

struct Config {
        uint8_t maxBursts = 15;    // liczba burstów
        uint8_t fadeSteps = 25;    // kroki fade-in/out dla burstów i split/collapse
        uint16_t holdSteps = 250;    // kroki trzymania w pełnej jasności w RandomCycle
        uint16_t slideWidth = 30;   // szerokość fazy slide
    };

// 4. ComplexSequenceAnimation – maszyna stanów: fade-out, random fade-in/out cycles, then sliding fragment
class ComplexSequenceAnimation : public ILiveAnimation {
public:
    Config cfg;

    ComplexSequenceAnimation(const Config& c = Config()) : cfg(c) {}
    ~ComplexSequenceAnimation() override = default;

    void initialize(LedColor* stripPtr, uint16_t count) override {
        ILiveAnimation::initialize(stripPtr, count);
        avgColor = computeAvg();
    }

    void onStart() override {
        // reset all state
        state         = State::SplitFade;
        bursts.clear();
        burstsIssued  = 0;
        splitStep     = 0;
        collapseStep  = 0;
        slidePos      = 0;
        slideForward  = true;
        centerLeft    = (ledsCount - 1) / 2;
        centerRight   = ledsCount / 2;
    }

    void process() override {
        if (!running) return;
        switch (state) {
        case State::SplitFade:    processSplitFade();    break;
        case State::RandomCycle:  processRandomCycle();  break;
        case State::Slide:        processSlide();        break;
        case State::Collapse:     processCollapse();     break;
        }
    }

private:
    enum class State { SplitFade, RandomCycle, Slide, Collapse } state;

    // SplitFade / Collapse
    int     centerLeft   = 0;
    int     centerRight  = 0;
    uint8_t splitStep    = 0;
    uint8_t collapseStep = 0;

    // RandomCycle
    struct Burst { int idx; uint8_t step; uint16_t hold; bool held; };
    std::vector<Burst> bursts;
    uint8_t burstsIssued = 0;

    // Slide
    uint16_t slidePos    = 0;
    bool     slideForward= true;

    // Common
    LedColor avgColor;

    // 1) SplitFade: fade out from center outward
    void processSplitFade() {
        std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
        float t = float(splitStep) / float(cfg.fadeSteps);
        t = std::min(t, 1.0f);
        int left  = centerLeft  - splitStep;
        int right = centerRight + splitStep;
        for (int i = left; i <= right; ++i) {
            if (i >= 0 && i < int(ledsCount)) {
                mainStrip[i].r = uint8_t(savedState[i].r * (1 - t));
                mainStrip[i].g = uint8_t(savedState[i].g * (1 - t));
                mainStrip[i].b = uint8_t(savedState[i].b * (1 - t));
            }
        }
        if (++splitStep > std::max(centerLeft, ledsCount - 1 - centerRight)) {
            state = State::RandomCycle;
        }
    }

    // 2) RandomCycle: bursts with hold at peak, ~10% of leds each spawn
    void processRandomCycle() {
        // clear
        for (uint16_t i = 0; i < ledsCount; ++i)
            mainStrip[i] = LedColor(0,0,0);
        // spawn new burst when previous held or none, up to maxBursts
        if ((bursts.empty() || bursts.back().held) && burstsIssued < cfg.maxBursts) {
            int burstCount = std::max<int>(1, ledsCount / 10);
            for (int b = 0; b < burstCount; ++b) {
                int idx = std::rand() % ledsCount;
                bursts.push_back({idx, 0, 0, false});
            }
            ++burstsIssued;
        }
        // update all bursts
        for (auto it = bursts.begin(); it != bursts.end();) {
            float t = float(it->step) / float(cfg.fadeSteps - 1);
            if (!it->held && it->step >= cfg.fadeSteps/2) {
                if (++(it->hold) < cfg.holdSteps) {
                    mainStrip[it->idx] = savedState[it->idx];
                    ++it;
                    continue;
                } else {
                    it->held = true;
                }
            }
            float blend = (t <= 0.5f) ? (t * 2.0f) : (2.0f * (1.0f - t));
            int idx = it->idx;
            mainStrip[idx].r = uint8_t(savedState[idx].r * blend);
            mainStrip[idx].g = uint8_t(savedState[idx].g * blend);
            mainStrip[idx].b = uint8_t(savedState[idx].b * blend);
            if (++(it->step) >= cfg.fadeSteps) {
                it = bursts.erase(it);
            } else {
                ++it;
            }
        }
        if (burstsIssued >= cfg.maxBursts && bursts.empty()) {
            // state = State::Slide;
            // slidePos = 0;
            // slideForward = true;
            burstsIssued = 0; // reset for next cycle
        }
    }

    // 3) Slide: smooth fade of segment
    void processSlide() {
        for (uint16_t i = 0; i < ledsCount; ++i) mainStrip[i] = LedColor(0,0,0);
        for (uint16_t i = 0; i < cfg.slideWidth; ++i) {
            int idx = slideForward ? slidePos + i : slidePos - i;
            if (idx < 0 || idx >= int(ledsCount)) continue;
            float x = float(i) / float(cfg.slideWidth - 1);
            float ease = (std::sin((x - 0.5f) * float(M_PI)) + 1.0f) * 0.5f;
            auto &orig = savedState[idx];
            mainStrip[idx].r = uint8_t(orig.r * ease + avgColor.r * (1 - ease));
            mainStrip[idx].g = uint8_t(orig.g * ease + avgColor.g * (1 - ease));
            mainStrip[idx].b = uint8_t(orig.b * ease + avgColor.b * (1 - ease));
        }
        if (slideForward) {
            if (++slidePos > int(ledsCount - cfg.slideWidth)) {
                state = State::Collapse;
                collapseStep = 0;
            }
        }
    }

    // 4) Collapse: grow from edges to center
    void processCollapse() {
        std::memcpy(mainStrip, savedState.data(), ledsCount * sizeof(LedColor));
        int maxSpread = std::max(centerLeft, int(ledsCount - 1 - centerRight));
        float t = float(collapseStep) / float(maxSpread);
        t = std::min(t, 1.0f);
        for (int s = 0; s <= collapseStep; ++s) {
            int left = centerLeft - s;
            int right = centerRight + s;
            if (left >= 0) mainStrip[left] = blendColor(savedState[left], t);
            if (right < ledsCount) mainStrip[right] = blendColor(savedState[right], t);
        }
        if (++collapseStep > maxSpread) {
            onStart();
        }
    }

    // Blend original color by factor
    LedColor blendColor(const LedColor &orig, float factor) const {
        return LedColor(
            uint8_t(orig.r * factor),
            uint8_t(orig.g * factor),
            uint8_t(orig.b * factor)
        );
    }

    // Compute average non-zero color
    LedColor computeAvg() const {
        uint32_t sumR=0, sumG=0, sumB=0, count=0;
        for (auto &c : savedState) {
            if (c.r || c.g || c.b) {
                sumR += c.r;
                sumG += c.g;
                sumB += c.b;
                ++count;
            }
        }
        if (!count) count = 1;
        return LedColor(
            uint8_t(sumR/count),
            uint8_t(sumG/count),
            uint8_t(sumB/count)
        );
    }
};
#endif