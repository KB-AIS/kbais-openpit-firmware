#ifndef QCGAUGEWIDGET_H
#define QCGAUGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QObject>
#include <QRectF>
#include <QtMath>



#if defined(QCGAUGE_COMPILE_LIBRARY)
#  define QCGAUGE_DECL  Q_DECL_EXPORT
#elif defined(QCGAUGE_USE_LIBRARY)
#  define QCGAUGE_DECL Q_DECL_IMPORT
#else
#  define QCGAUGE_DECL
#endif

class QcGaugeWidget;
class QcItem;
class QcBackgroundItem;
class QcDegreesItem;
class QcValuesItem;
class QcArcItem;
class QcColorBand;
class QcNeedleItem;
class QcLabelItem;
class QcGlassItem;
class QcAttitudeMeter;
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class QCGAUGE_DECL QcGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QcGaugeWidget(QWidget *parent = 0);    

    QcBackgroundItem* addBackground(float position);
    QcDegreesItem* addDegrees(float position);
    QcValuesItem* addValues(float position);
    QcArcItem* addArc(float position);
    QcColorBand* addColorBand(float position);
    QcNeedleItem* addNeedle(float position);
    QcLabelItem* addLabel(float position);
    QcGlassItem* addGlass(float position);
    QcAttitudeMeter* addAttitudeMeter(float position);


    void addItem(QcItem* item, float position);
    int removeItem(QcItem* item);
    QList <QcItem*> items();
    QList <QcItem*> mItems;


signals:

public slots:
private:
    void paintEvent(QPaintEvent *);

};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcItem : public QObject
{
    Q_OBJECT
public:
    explicit QcItem(QObject *parent = 0);
    virtual void draw(QPainter *) = 0;
    virtual int type();

    void setPosition(float percentage);
    float position();
    QRectF rect();
    enum Error{InvalidValueRange,InvalidDegreeRange,InvalidStep};


protected:
    QRectF adjustRect(float percentage);
    float getRadius(const QRectF &);
    float getAngle(const QPointF&, const QRectF &tmpRect);
    QPointF getPoint(float deg, const QRectF &tmpRect);
    QRectF resetRect();
    void update();

private:
    QRectF mRect;
    QWidget *parentWidget;
    float mPosition;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcScaleItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcScaleItem(QObject *parent = 0);

    void setValueRange(float minValue,float maxValue);
    void setDegreeRange(float minDegree,float maxDegree);
    void setMinValue(float minValue);
    void setMaxValue(float maxValue);
    void setMinDegree(float minDegree);
    void setMaxDegree(float maxDegree);
    void setDegreeOffset(float degreeOffset);

signals:

public slots:
protected:

    float getDegFromValue(float) const;
    float getDegFromValue();

    float mMinValue;
    float mMaxValue;
    float mMinDegree;
    float mMaxDegree;
    float mDegreeOffset;

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcBackgroundItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcBackgroundItem(QObject *parent = 0);
    void draw(QPainter*);
    void addColor(float position, const QColor& color);
    void clearrColors();


private:
    QPen mPen;
    QList<QPair<float,QColor> > mColors;
    QLinearGradient mLinearGrad;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QcGlassItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcGlassItem(QObject *parent = 0);
    void draw(QPainter*);
};



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcLabelItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcLabelItem(QObject *parent = 0);
    virtual void draw(QPainter *);
    void setAngle(float);
    float angle();
    void setText(const QString &text, bool repaint = true);
    QString text();
    void setColor(const QColor& color);
    QColor color();
    void setFont(const QString &font);
    QString font();

private:
    float mAngle;
    QString mText;
    QColor mColor;
    QString mFont;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcArcItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcArcItem(QObject *parent = 0);
    void draw(QPainter*);
    void setColor(const QColor& color);

private:
    QColor mColor;

signals:

public slots:


};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcColorBand : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcColorBand(QObject *parent = 0);
    void draw(QPainter*);
    void setColors(const QList<QPair<QColor,float> >& colors);

private:
   QPainterPath createSubBand(float from,float sweep);

   QList<QPair<QColor,float> > mBandColors;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class QCGAUGE_DECL QcDegreesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcDegreesItem(QObject *parent = 0);
    void draw(QPainter *painter);
    void setStep(float step);
    void setColor(const QColor& color);
    void setSubDegree(bool );
private:
    float mStep;
    QColor mColor;
    bool mSubDegree;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcNeedleItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcNeedleItem(QObject *parent = 0);
    void draw(QPainter*);
    void setCurrentValue(float value);
    float currentValue();
    void setValueFormat(QString format);
    QString currentValueFormat();
    void setColor(const QColor & color);
    QColor color();

    void setLabel(QcLabelItem*);
    QcLabelItem * label();

    enum NeedleType{DiamonNeedle,TriangleNeedle,FeatherNeedle,AttitudeMeterNeedle,CompassNeedle};//#

    void setNeedle(QcNeedleItem::NeedleType needleType);
private:
    QPolygonF mNeedlePoly;
    float mCurrentValue;
    QColor mColor;
    void createDiamonNeedle(float r);
    void createTriangleNeedle(float r);
    void createFeatherNeedle(float r);
    void createAttitudeNeedle(float r);
    void createCompassNeedle(float r);
    NeedleType mNeedleType;
    QcLabelItem *mLabel;
    QString mFormat;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


class QCGAUGE_DECL QcValuesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcValuesItem(QObject *parent = 0);
    void draw(QPainter*);
    void setStep(float step);
    float step();
    void setColor(const QColor& color);
    QColor color();
    void setFont(const QString &font);
    QString font();
private:
    float mStep;
    QColor mColor;
    QString mFont;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcAttitudeMeter : public QcItem
{
    Q_OBJECT
public:
    explicit QcAttitudeMeter(QObject *parent = 0);

    void draw(QPainter *);
    void setCurrentPitch(float pitch);
    void setCurrentRoll(float roll);
private:
    float mRoll;
    float mPitch;
    float mPitchOffset;

    QPolygonF mHandlePoly;
    QPainterPath mStepsPath;

    QPointF getIntersection(float r,const QPointF& pitchPoint,const QPointF& pt);
    float getStartAngle(const QRectF& tmpRect);

    void drawDegrees(QPainter *);
    void drawDegree(QPainter * painter, const QRectF& tmpRect,float deg);
    void drawUpperEllipse(QPainter *,const QRectF&);
    void drawLowerEllipse(QPainter *,const QRectF&);
    void drawPitchSteps(QPainter *,const QRectF&);
    void drawHandle(QPainter *);
    void drawSteps(QPainter *,float);

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcBar : public QWidget {
    Q_OBJECT
public:
    enum DirectionEnum
    {
        Horizontal,
        Vertical
    };

public:
    QcBar(QWidget *parent = nullptr);
    ~QcBar() override;

protected:

    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawProgress(QPainter *painter);
    void drawRulerTop(QPainter *painter);
    void drawRulerBottom(QPainter *painter);
    void drawRulerLeft(QPainter *painter);
    void drawRulerRight(QPainter *painter);

private:
    DirectionEnum direction= DirectionEnum::Horizontal; //direction
    double minValue; //minimum
    double maxValue; //maximum
    double value; //target value
    int precision; //precision, a few decimal places
    int longStep; //long line equal division step
    int shortStep; //short line equal step size
    bool rulerTop=true; //the tick is above
    bool rulerBottom=true; //the tick is under
    bool rulerLeft=false; //the tick is on left
    bool rulerRight=false; //the tick is on right

    QColor bgColor; //background color
    QColor lineColor; //Line color
    QColor progressColor; // progress color

    double currentValue; //current value

public:
    DirectionEnum getDirection()    const;
    double getMinValue()            const;
    double getMaxValue()            const;
    double getValue()               const;
    int getPrecision()              const;

    int getLongStep()               const;
    int getShortStep()              const;
    bool getRulerTop()              const;
    bool getRulerBottom()           const;
    bool getRulerLeft()             const;
    bool getRulerRight()            const;

    QColor getBgColor()             const;
    QColor getLineColor()           const;
    QColor getProgressColor()       const;


public Q_SLOTS:
    void setCurrentValue(int value);

    // Set the range value
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    // Set paint direction
    void setDirection(DirectionEnum direction);

    // Set the maximum and minimum
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);

    // Set the accuracy
    void setPrecision(int precision);
    // Set the line equal step size
    void setLongStep(int longStep);
    void setShortStep(int shortStep);
    // Set the scale on the top
    void setRulerTop(bool rulerTop);
    // Set the scale on the bottom
    void setRulerBottom(bool rulerBottom);
    // Set the scale on the Left
    void setRulerLeft(bool rulerLeft);
    // Set the scale on the Right
    void setRulerRight(bool rulerRight);

    // Set the background color
    void setBgColor(const QColor &bgColor);
    // Set the line color
    void setLineColor(const QColor &lineColor);
    // Set the progress color
    void setProgressColor(const QColor &progressColor);
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

#endif // QCGAUGEWIDGET_H