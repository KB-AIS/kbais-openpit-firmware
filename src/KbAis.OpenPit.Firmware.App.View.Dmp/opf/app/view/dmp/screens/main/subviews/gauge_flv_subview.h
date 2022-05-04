#ifndef GAUGE_FLV_SUBVIEW_H
#define GAUGE_FLV_SUBVIEW_H

// qt
#include <QtWidgets/qlabel.h>

#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"

#include "opf/app/view/dmp/utils/neon_colors.h"

class gauge_flv_subview {

    QcNeedleItem* needle_;
    QLabel*       label_;

public:
    void setup_subview(QcGaugeWidget* gauge, QLabel* label) noexcept;

};

inline void gauge_flv_subview::setup_subview(QcGaugeWidget* gauge, QLabel* label) noexcept {
    constexpr float GAUGE_DEG_S = -30.0f, GAUGE_DEG_E = 210.0f;
    // TODO: Update from configuration
    constexpr float GAUGE_VAL_MIN = 0.0f, GAUGE_VAL_MAX = 2000.0f;

    auto arc = gauge->addArc(95.0f);
    arc->setColor(opf::app::view::utils::colors::neon_yellow);
    arc->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);

    auto deg = gauge->addDegrees(88.0f);
    deg->setColor(opf::app::view::utils::colors::neon_yellow);
    deg->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    deg->setStep(GAUGE_VAL_MAX / 10);
    deg->setSubDegree(true);
    deg->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    auto val =  gauge->addValues(60.0f);
    val->setColor(opf::app::view::utils::colors::neon_yellow);
    val->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    val->setFont("Roboto");
    val->setStep(GAUGE_VAL_MAX / 5);
    val->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    auto bnd = gauge->addColorBand(90.0f);
    bnd->setColors({
        { opf::app::view::utils::colors::neon_red,    GAUGE_VAL_MAX }
    ,   { opf::app::view::utils::colors::neon_green,  GAUGE_VAL_MAX * 0.66f }
    ,   { opf::app::view::utils::colors::neon_yellow, GAUGE_VAL_MAX * 0.33f }
    });
    bnd->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    bnd->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    needle_ = gauge->addNeedle(100.0f);
    needle_->setColor(opf::app::view::utils::colors::neon_blue);
    needle_->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    needle_->setNeedle(QcNeedleItem::DiamonNeedle);
    needle_->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    label_ = label;
}

#endif // GAUGE_FLV_SUBVIEW_H
