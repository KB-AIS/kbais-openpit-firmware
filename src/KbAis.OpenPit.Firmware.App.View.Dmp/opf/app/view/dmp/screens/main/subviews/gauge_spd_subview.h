#ifndef GAUGE_SPD_SUBVIEW_H
#define GAUGE_SPD_SUBVIEW_H

#include "Utils/Widgets/QcGauge/QcGaugeWidget.h"

#include "opf/app/view/dmp/utils/neon_colors.h"

#include <IRxGpsSensorPublisher.h>

class gauge_spd_subview {
    QcLabelItem*  lable_;

    QcNeedleItem* needle_;

    rxcpp::subscriber<GpsMessage> sub_gps_message_ {
        rxcpp::make_subscriber<GpsMessage>([this](auto x) {
            handle_gps_message(x);
        })
    };

    void handle_gps_message(const GpsMessage& message);

public:
    void update_value_from(rxcpp::observable<GpsMessage> obs_gps_message) const;

    void setup_speed_gauge(QcGaugeWidget* gauge) noexcept;

};

inline void gauge_spd_subview::handle_gps_message(const GpsMessage& message) {
    needle_->setCurrentValue(message.speed);

    const auto lable_speed_text = QStringLiteral("%1 КМ/Ч.").arg(std::round(message.speed));

    lable_->setText(lable_speed_text);
}

inline void gauge_spd_subview::update_value_from(rxcpp::observable<GpsMessage> obs_gps_message) const {
    obs_gps_message.subscribe(sub_gps_message_);
}

inline void gauge_spd_subview::setup_speed_gauge(QcGaugeWidget* gauge) noexcept {
    constexpr int GAUGE_DEG_S = -60, GAUGE_DEG_E = 240;

    constexpr int GAUGE_VAL_MIN = 0, GAUGE_VAL_MAX = 80;

    auto setup_arc = [&]() {
        auto x = gauge->addArc(95);

        x->setColor(opf::app::view::utils::colors::neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
    };
    setup_arc();

    auto setup_degrees = [&]() {
        auto x = gauge->addDegrees(88);
        x->setColor(opf::app::view::utils::colors::neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setStep(10);
        x->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);

        auto y = gauge->addDegrees(88);
        y->setColor(opf::app::view::utils::colors::neon_yellow);
        y->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        y->setStep(2.5);
        y->setSubDegree(true);
        y->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);
    };
    setup_degrees();

    auto setup_values = [&]() {
        auto x = gauge->addValues(65);
        x->setColor(opf::app::view::utils::colors::neon_yellow);
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setFont("Roboto");
        x->setStep(10);
        x->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);
    };
    setup_values();

    auto setup_color_band = [&]() {
        auto x = gauge->addColorBand(90);
        x->setColors({
            { opf::app::view::utils::colors::neon_green, 80 }
        ,   { opf::app::view::utils::colors::neon_red, 55 },
        });
        x->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        x->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);
    };
    setup_color_band();

    auto setup_needle = [&]() {
        needle_ = gauge->addNeedle(100);
        needle_->setColor(opf::app::view::utils::colors::neon_blue);
        needle_->setDegreeRange(GAUGE_DEG_S, GAUGE_DEG_E);
        needle_->setNeedle(QcNeedleItem::DiamonNeedle);
        needle_->setValueRange(GAUGE_VAL_MIN, GAUGE_VAL_MAX);
    };
    setup_needle();

    auto setup_label = [&]() {
        lable_ = gauge->addLabel(90);
        lable_->setColor(opf::app::view::utils::colors::neon_yellow);
        lable_->setFont("Roboto");
    };
    setup_label();
}

#endif // GAUGE_SPD_SUBVIEW_H
