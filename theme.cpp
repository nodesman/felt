#include "theme.h"
#include <algorithm>
#include <cmath>

namespace {

double linearizeChannel(double normalized) {
    if (normalized <= 0.04045) {
        return normalized / 12.92;
    }
    return std::pow((normalized + 0.055) / 1.055, 2.4);
}

} // namespace

ThemePalette::ThemePalette(const QString &key,
                           const QString &displayName,
                           const CanvasSurface &surface,
                           const SyntaxAccents &accents)
    : m_identityKey(key)
    , m_displayName(displayName)
    , m_surface(surface)
    , m_accents(accents)
{
}

QString ThemePalette::key() const { return m_identityKey; }
QString ThemePalette::displayName() const { return m_displayName; }
QColor ThemePalette::bgOuter() const { return m_surface.bgOuter; }
QColor ThemePalette::bgVignetteCenter() const { return m_surface.bgVignetteCenter; }
QColor ThemePalette::textMain() const { return m_surface.textMain; }
QColor ThemePalette::textDimmed() const { return m_surface.textDimmed; }
QColor ThemePalette::accent() const { return m_accents.accent; }
QColor ThemePalette::selectionBg() const { return m_accents.selectionBg; }
QColor ThemePalette::selectionText() const { return m_accents.selectionText; }
QColor ThemePalette::markdownPrefix() const { return m_accents.markdownPrefix; }
QColor ThemePalette::hudText() const { return m_accents.hudText; }
QColor ThemePalette::codeBg() const { return m_accents.codeBg; }
QColor ThemePalette::codeText() const { return m_accents.codeText; }
QColor ThemePalette::quoteText() const { return m_accents.quoteText; }

double ThemePalette::bodyContrastRatio() const {
    return contrastRatio(m_surface.textMain, m_surface.bgVignetteCenter);
}

double ThemePalette::dimmedContrastRatio() const {
    return contrastRatio(m_surface.textDimmed, m_surface.bgVignetteCenter);
}

double ThemePalette::caretContrastRatio() const {
    return contrastRatio(m_accents.accent, m_surface.bgVignetteCenter);
}

double ThemePalette::selectionContrastRatio() const {
    return contrastRatio(m_accents.selectionText, m_accents.selectionBg);
}

double ThemePalette::relativeLuminance(const QColor &color) {
    const double red = linearizeChannel(color.redF());
    const double green = linearizeChannel(color.greenF());
    const double blue = linearizeChannel(color.blueF());
    return 0.2126 * red + 0.7152 * green + 0.0722 * blue;
}

double ThemePalette::contrastRatio(const QColor &first, const QColor &second) {
    const double lumFirst = relativeLuminance(first);
    const double lumSecond = relativeLuminance(second);
    const double lighter = std::max(lumFirst, lumSecond);
    const double darker = std::min(lumFirst, lumSecond);
    return (lighter + 0.05) / (darker + 0.05);
}
