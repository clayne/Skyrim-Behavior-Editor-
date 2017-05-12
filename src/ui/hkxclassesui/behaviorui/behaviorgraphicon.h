#ifndef BEHAVIORGRAPHICON_H
#define BEHAVIORGRAPHICON_H

#include "src/ui/treegraphicsitem.h"

#include <QRadialGradient>
#include <QPen>

class BehaviorGraphIcon: public TreeGraphicsItem
{
public:
    BehaviorGraphIcon(TreeGraphicsItem *parent, DataIconManager *obj, int indexToInsert = -1, Qt::GlobalColor color = Qt::gray);
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void unselect();
private:
    static QRectF button;
    static QRectF textRec;
    static QFont font;
    QPen pen;
    static QBrush brush;
    static QBrush buttonBrush;
    static QLineF vert;
    static QLineF horiz;
    static QRectF ellipse;
    QPolygonF polygon;
    static QRectF square;
    QPolygonF arrowHead;
    QRadialGradient rGrad;
    QPen textPen;
    QPainterPath path;
};

#endif // BEHAVIORGRAPHICON_H