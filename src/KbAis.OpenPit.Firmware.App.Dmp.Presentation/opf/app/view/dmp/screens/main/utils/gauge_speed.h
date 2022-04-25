#ifndef GAUGE_SPEED_H
#define GAUGE_SPEED_H

#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"

class gauge_speed {
    QColor color_neon_yellow { 253, 254,   2 }
    ,      color_neon_red    { 254,   0,   0 }
    ,      color_neon_green  {  11, 255,   1 };

public:
    QcLabelItem*  lable;
    QcNeedleItem* needle;

    void setup_speed_gauge(QcGaugeWidget* gauge) noexcept;

};

inline void gauge_speed::setup_speed_gauge(QcGaugeWidget *gauge) noexcept {
    constexpr int GAUGE_DEG_S = -60, GAUGE_DEG_E = 240;

    constexpr int GAUGE_SPD_MIN = 0, GAUGE_SPD_MAX = 80;

    auto setup_arc = [&]() {
        auto x = gauge->addArc(95);

        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    };
    setup_arc();

    auto setup_degrees = [&]() {
        auto x = gauge->addDegrees(88);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);

        auto y = gauge->addDegrees(88);
        y->setColor(color_neon_yellow);
        y->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        y->setStep(2.5);
        y->setSubDegree(true);
        y->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_degrees();

    auto setup_values = [&]() {
        auto x = gauge->addValues(65);
        x->setColor(color_neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setFont("DS-Digital");
        x->setStep(10);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_values();

    auto setup_color_band = [&]() {
        auto x = gauge->addColorBand(90);
        x->setColors({ { color_neon_red, 80 }, { color_neon_green, 55 }, });
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_color_band();

    auto setup_needle = [&]() {
        needle = gauge->addNeedle(100);
        needle->setColor(color_neon_yellow);
        needle->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        needle->setNeedle(QcNeedleItem::TriangleNeedle);
        needle->setValueRange(GAUGE_SPD_MIN, GAUGE_SPD_MAX);
    };
    setup_needle();

    auto setup_label = [&]() {
        lable = gauge->addLabel(90);
        lable->setColor(color_neon_yellow);
        lable->setFont("DS-Digital");
    };
    setup_label();
}

#endif // GAUGE_SPEED_H
