#ifndef PLOT_STYLE_H
#define PLOT_STYLE_H

#include <plot/qcustomplot.h>

void createPlot(QCustomPlot* plot,
                const QVector<double>& x, const QVector<double>& y);

#endif // PLOT_STYLE_H
