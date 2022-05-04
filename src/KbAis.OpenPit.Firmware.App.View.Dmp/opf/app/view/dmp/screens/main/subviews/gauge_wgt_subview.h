#ifndef GAUGE_WGT_SUBVIEW_H
#define GAUGE_WGT_SUBVIEW_H

// qt
#include <QtWidgets/qlabel.h>

#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"

#include "opf/app/view/dmp/utils/neon_colors.h"
#include "opf/modules/sensors/lcs/lcs_sensor_publisher.h"

class gauge_wgt_subview {

    QcNeedleItem* needle_;
    QLabel*       lable_;

    rxcpp::subscriber<lcs_sensor_message> sub_wgt_message_ {
        rxcpp::make_subscriber<lcs_sensor_message>([this](auto x) { handle_lcs_message(x); })
    };

    void handle_lcs_message(const lcs_sensor_message& message);

public:
    void setup_subview(QcGaugeWidget* gauge, QLabel* label) noexcept;

    void update_value_from(rxcpp::observable<lcs_sensor_message> obs_lcs_message) const;

};

inline void gauge_wgt_subview::handle_lcs_message(const lcs_sensor_message& message) {
    needle_->setCurrentValue(message.weight / 100.0);

    const auto lable_wgt_text = QStringLiteral("%1").arg(std::round(message.weight / 100.0));

    lable_->setText(lable_wgt_text);
}

inline void gauge_wgt_subview::update_value_from(
    rxcpp::observable<lcs_sensor_message> obs_lcs_message
) const {
    obs_lcs_message.subscribe(sub_wgt_message_);
}

inline void gauge_wgt_subview::setup_subview(QcGaugeWidget* gauge, QLabel* lable) noexcept {
    constexpr float GAUGE_DEG_S = -30.0f, GAUGE_DEG_E = 210.0f;
    constexpr float GAUGE_VAL_MIN = 0.0f, GAUGE_VAL_MAX = 200.0f;

    auto arc = gauge->addArc(95);
    arc->setColor(opf::app::view::utils::colors::neon_yellow);
    arc->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);

    auto deg = gauge->addDegrees(88);
    deg->setColor(opf::app::view::utils::colors::neon_yellow);
    deg->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    deg->setStep(GAUGE_VAL_MAX / 10);
    deg->setSubDegree(true);
    deg->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    auto val =  gauge->addValues(60);
    val->setColor(opf::app::view::utils::colors::neon_yellow);
    val->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    val->setFont("Roboto");
    val->setStep(GAUGE_VAL_MAX / 5);
    val->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    auto bnd = gauge->addColorBand(90);
    bnd->setColors({
        { opf::app::view::utils::colors::neon_yellow, GAUGE_VAL_MAX }
    ,   { opf::app::view::utils::colors::neon_red,    GAUGE_VAL_MAX * 0.75f }
    });
    bnd->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    bnd->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    needle_ = gauge->addNeedle(100);
    needle_->setColor(opf::app::view::utils::colors::neon_blue);
    needle_->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    needle_->setNeedle(QcNeedleItem::DiamonNeedle);
    needle_->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

    lable_ = lable;
}

#endif // GAUGE_WGT_SUBVIEW_H
