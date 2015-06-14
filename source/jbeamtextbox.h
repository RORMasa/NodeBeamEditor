#pragma once

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

#include "NodeBeam.h"

class Highlighter : public QSyntaxHighlighter
{
public:

    Highlighter(QTextDocument * document);
    ~Highlighter();

    void highlightBlock(const QString &text);


private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector <HighlightingRule> rules;


};

class JBEAM_TextBox : public QPlainTextEdit
{
    Q_OBJECT
public:
    JBEAM_TextBox(QWidget *parent=0, NodeBeam *nb=0);
    void LineNumbersPaintEvent(QPaintEvent *event);
    int LineNumbers_Width;
    NodeBeam *CurrentNodeBeam;

    /* Text cursors */
    //Update cursors
    void JBEAM_UpdateCursors();

    //Node JBEAM cursor; if is set, add new nodes at cursor position. Value is -1 if not set
    int JBEAM_NodeCursor;

    //Beam JBEAM cursor; if is set, add new beams at cursor position. Value is -1 if not set
    int JBEAM_BeamCursor;

    /* Find containers */
    //Find JBEAM node or beam from string
    bool FindNodeContainer(QString *JBEAM_box, QString nodename, int &NodeBegin, int &NodeEnd, bool FindComma, int &RealNodeEnd);
    bool FindNodeContainer2(QString *JBEAM_box, QString nodename, int &nodesBegin, int &pos1, int &pos2);
    bool FindNodesSection(QString *JBEAM_box, int offset, int &nodesBegin, int &nodesEnd);
    bool FindBeamContainer(QString *JBEAM_box, QString beam, int &Begin, int &End, bool FindComma,int &RealEnd);
    bool JBEAM_FindOtherContainer(QString JBEAM_box, QString listtype, int &Begin, int &End);

    /* Add */
    void JBEAM_AddNode();
    void JBEAM_AddBeam();
    void JBEAM_AddArrayItem(int ListType_id);
    int JBEAM_AddComment(int CursorPos, QString Comment);
    void JBEAM_AddFromTemp();

    /* Delete */
    void JBEAM_DeleteNodes();
    void JBEAM_DeleteBeams();

    /* Filenames */
    QString FullFilePath;
    QString FileName;
    void SetFilePath(QString FileName);

    void ResetContents();
    QString EmptyJbeamTextTemplate;

    /* Syntax highligher */
    Highlighter * synhigh;

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void JBEAM_UpdateAllNodes(bool *updated);
    void JBEAM_UpdateSelectedNodes();
    void PlaceNodeCursor();
    void PlaceBeamCursor();
    void PrintNodePicked(int node_id);

private slots:
    void updateLineNumbers(const QRect &rect, int dy);

    void textchanged();

private:
    //Line numbers drawing
    QWidget * LineNumbersA;

    /* JBEAM pretty printing */
    void str_addIndent(QString * text, int indent);
    void str_addValueStr(QString * text, QString key, QString value, int indent);
    void str_beginObject(QString * text, QString key, int indent);
    void str_endObject(QString * text, int indent);
    void str_beginList(QString * text, QString key, int indent);
    void str_endList(QString * text, int indent);

    //Cursors
    QStringList JBEAM_curorNames;
    QList <int> JBEAM_cursorLocs;

};

class LineNumbers : public QWidget
{
public:
    LineNumbers(JBEAM_TextBox *textbox) : QWidget(textbox)
    {
        TextBox = textbox;
    }
protected:
    void paintEvent(QPaintEvent *event) {
        TextBox->LineNumbersPaintEvent(event);
    }

private:
    JBEAM_TextBox * TextBox;
};

class SyntaxHighlighter : public QSyntaxHighlighter
{



};



