#include "colladadae.h"
#include <QFile>
#include <QDebug>


DaeGeometry::DaeGeometry()
{
    vertices_found = 0;
}

void DaeGeometry::clear()
{
    float_arrays.clear();
    float_array_ids.clear();
    float_array_count.clear();
    vertices_found = 0;
    vertices.clear();
    VertexVector.clear();
}

ColladaDae::ColladaDae(QObject *parent) :
    QObject(parent)
{
}

bool ColladaDae::ParseDae(QString file)
{
    qDebug() << "Parsing dae";
    QFile daefile(file);
    if (!daefile.open(QIODevice::ReadOnly | QIODevice::Text))
           return false;

    QXmlStreamReader dae(&daefile);

    bool libgeom = 0;
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        QXmlStreamReader::TokenType token = dae.readNext();
        if((dae.name() == "library_geometries") && (dae.isStartElement()))
        {
            libgeom=1;
            qDebug() << "lib geom start parse " << dae.characterOffset();

        }
        if(libgeom)
        {
            if((dae.name() == "library_geometries") && (dae.isEndElement()))
            {
                libgeom = 0;
                qDebug() << "lib geom end" << dae.characterOffset();
            }
            if((dae.name() == "geometry") && (dae.isStartElement()))
            {
                ParseGeom(dae);
            }
        }
    }

    //File reading finished, indexing results

    for(int i3=0; i3<Geometries.size();i3++)
    {
        if(Geometries.at(i3).vertices.at(0) == '#') Geometries[i3].vertices.replace(0,1,"");
        qDebug() << Geometries.at(i3).vertices;

        for(int i=0; i<Geometries.at(i3).float_array_ids.size();i++)
        {
            if(Geometries.at(i3).float_array_ids.at(i) == Geometries.at(i3).vertices)
            {
                qDebug() << "Vertices are " << Geometries.at(i3).float_array_count.at(i);
                qDebug() << Geometries.at(i3).float_arrays.at(i);

                IndexVertices(Geometries.at(i3).float_arrays.at(i),
                              Geometries.at(i3).float_array_count.at(i),
                              Geometries[i3].VertexVector);
                for(int i4=0; i4< Geometries.at(i3).VertexVector.size();i4++)
                    qDebug() << Geometries[i3].VertexVector.at(i4);

            }
        }
    }
}

//Parse Geometry
bool ColladaDae::ParseGeom(QXmlStreamReader &dae)
{
    if((dae.name() == "geometry") && (dae.isStartElement()))
    {
        qDebug() << "geom start parse" << dae.characterOffset();
        TempGeometry.clear();
    }
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        dae.readNext();
        if((dae.name() == "geometry") && (dae.isEndElement()))
        {
            qDebug() << "geom end parse" << dae.characterOffset();
            Geometries.append(TempGeometry);
            break;
        }
        if((dae.name() == "mesh") && (dae.isStartElement()))
        {
            ParseMesh(dae);
        }
    }
}

//Parse Mesh
bool ColladaDae::ParseMesh(QXmlStreamReader &dae)
{
    if((dae.name() == "mesh") && (dae.isStartElement()))
    {
        qDebug() << "mesh start parse" << dae.characterOffset();
    }
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        dae.readNext();
        if((dae.name() == "mesh") && (dae.isEndElement()))
        {
            qDebug() << "mesh end parse" << dae.characterOffset();
            break;
        }
        if((dae.name() == "vertices") && (dae.isStartElement()))
        {
            ParseVert(dae);
        }
        else if((dae.name() == "source") && (dae.isStartElement()))
        {
            ParseSource(dae);
        }
    }
}

//Parse vertices
bool ColladaDae::ParseVert(QXmlStreamReader &dae)
{
    if((dae.name() == "vertices") && (dae.isStartElement()))
    {
        qDebug() << "vertices start parse" << dae.characterOffset();
    }
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        if((dae.name() == "vertices") && (dae.isEndElement()))
        {
            qDebug() << "vertices end parse" << dae.characterOffset();
            break;
        }
        if((dae.name() == "input"))
        {
            //Get name of vertices source
            QXmlStreamAttributes input = dae.attributes();
            for(int i=0; i<input.size();i++)
            {
                if(input.at(i).name() == "source")
                {
                    TempGeometry.vertices_found = 1;
                    TempGeometry.vertices = input.at(i).value().toString();
                }
            }
        }
        dae.readNext();
    }
}

//Parse source
bool ColladaDae::ParseSource(QXmlStreamReader &dae)
{
    QString source_id;
    if((dae.name() == "source") && (dae.isStartElement()))
    {
        qDebug() << "source start parse" << dae.characterOffset();
        qDebug() << dae.attributes().at(0).value();
        //Get name of source
        QXmlStreamAttributes input = dae.attributes();
        for(int i=0; i<input.size();i++)
        {
            if(input.at(i).name() == "id")
            {
                source_id = input.at(i).value().toString();
            }
        }
    }
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        dae.readNext();
        if((dae.name() == "source") && (dae.isEndElement()))
        {
            qDebug() << "source end parse" << dae.characterOffset();
            break;
        }
        if((dae.name() == "float_array") && (dae.isStartElement()))
        {
            ParseFloatArray(dae, source_id);
        }
    }
}

//Parse float array in source
bool ColladaDae::ParseFloatArray(QXmlStreamReader &dae, QString source_id)
{
    if((dae.name() == "float_array") && (dae.isStartElement()))
    {
        qDebug() << "float_array start parse" << dae.characterOffset();
        //Find count, if found, add all contents in TempGeometry
        for(int i=0; i<dae.attributes().size();i++)
        {
            if(dae.attributes().at(i).name() == "count")
            {
                int count = dae.attributes().at(i).value().toInt();
                if(count > 0)
                {
                    TempGeometry.float_array_ids.append(source_id);
                    TempGeometry.float_arrays.append(dae.readElementText());
                    TempGeometry.float_array_count.append(count);
                }
            }
        }

    }
    while((!dae.atEnd()) && (!dae.hasError()))
    {
        if((dae.name() == "float_array") && (dae.isEndElement()))
        {
            qDebug() << "float_array end parse" << dae.characterOffset();
            break;
        }
        dae.readNext();
    }
}

//Pick vertices from the string and split the in xyz coordinates.
bool ColladaDae::IndexVertices(QString vertices, int count, QVector <QVector3D> &VertexVec)
{
    QStringList verts = vertices.split(" ");
    int xyz = 0;

    QVector3D TempVert;

    //Check that all verts are on place
    if(verts.size() == count)
    {
        qDebug() << "vert indexing";
        for(int i=0; i<verts.size(); i++)
        {
            if(xyz == 0)
            {
                //X
                TempVert.setX(verts.at(i).toFloat());
                xyz++;
            }
            else if(xyz == 1)
            {
                //Y
                TempVert.setY(verts.at(i).toFloat());
                xyz++;
            }
            else if(xyz == 2)
            {
                //Z
                TempVert.setZ(verts.at(i).toFloat());
                VertexVec.append(TempVert);
                xyz = 0;
            }
        }
        return true;
    }
    else return false;


}
