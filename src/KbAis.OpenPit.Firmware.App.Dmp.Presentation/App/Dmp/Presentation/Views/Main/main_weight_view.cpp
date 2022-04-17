#include "main_weight_view.h"

#include <QtGui/qpainter.h>
#include <QtSvg/qsvgrenderer.h>

main_weight_view::main_weight_view(QWidget *parent)
    :   QWidget(parent)
{
}

void main_weight_view::set_svg(const QString& svg) {
    svg_ = svg;

    this->update();
}

void main_weight_view::paintEvent([[maybe_unused]] QPaintEvent* event) {
    if (svg_.isEmpty()) return;

    auto pntr = QPainter { this };

    auto svgr = QSvgRenderer(svg_);
    svgr.render(&pntr);

    pntr.end();
}
