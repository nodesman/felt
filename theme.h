#pragma once

#include <QColor>
#include <QString>

struct CanvasSurface {
    QColor bgOuter;
    QColor bgVignetteCenter;
    QColor textMain;
    QColor textDimmed;
};

struct SyntaxAccents {
    QColor accent;
    QColor selectionBg;
    QColor selectionText;
    QColor markdownPrefix;
    QColor hudText;
    QColor codeBg;
    QColor codeText;
    QColor quoteText;
};

class ThemePalette {
public:
    ThemePalette(const QString &key,
                 const QString &displayName,
                 const CanvasSurface &surface,
                 const SyntaxAccents &accents);

    QString key() const;
    QString displayName() const;
    QColor bgOuter() const;
    QColor bgVignetteCenter() const;
    QColor textMain() const;
    QColor textDimmed() const;
    QColor accent() const;
    QColor selectionBg() const;
    QColor selectionText() const;
    QColor markdownPrefix() const;
    QColor hudText() const;
    QColor codeBg() const;
    QColor codeText() const;
    QColor quoteText() const;

    double bodyContrastRatio() const;
    double dimmedContrastRatio() const;
    double caretContrastRatio() const;
    double selectionContrastRatio() const;

    static double relativeLuminance(const QColor &color);
    static double contrastRatio(const QColor &first, const QColor &second);

private:
    QString m_identityKey;
    QString m_displayName;
    CanvasSurface m_surface;
    SyntaxAccents m_accents;
};
