#ifndef COLLADADAE_H
#define COLLADADAE_H

#include <QObject>
#include <QXmlStreamReader>
#include <QVector3D>

class DaeGeometry
{
public:
    DaeGeometry();
    void clear();
    QVector <QString> float_arrays;
    QVector <QString> float_array_ids;
    QVector <int> float_array_count;
    bool vertices_found;
    QString vertices;

    //After indexing
    QVector < QVector3D > VertexVector;

};

class ColladaDae : public QObject
{
    Q_OBJECT
public:
    explicit ColladaDae(QObject *parent = 0);

    bool ParseDae(QString file);


signals:

public slots:

private:

    QVector <DaeGeometry> Geometries;
    DaeGeometry TempGeometry;

    bool ParseGeom(QXmlStreamReader &dae);
    bool ParseMesh(QXmlStreamReader &dae);
    bool ParseVert(QXmlStreamReader &dae);
    bool ParseSource(QXmlStreamReader &dae);
    bool ParseFloatArray(QXmlStreamReader &dae, QString source_id);

    //Index vertices
    bool IndexVertices(QString vertices, int count, QVector <QVector3D> &VertexVec);
};

#endif // COLLADADAE_H
