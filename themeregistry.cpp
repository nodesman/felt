#include "themeregistry.h"

ThemePalette ThemeRegistry::obsidianVoid() {
    const CanvasSurface surface{
        QColor(0x08, 0x09, 0x0c),
        QColor(0x0d, 0x0f, 0x14),
        QColor(0xec, 0xeb, 0xe8),
        QColor(0x4c, 0x52, 0x5b)
    };
    const SyntaxAccents accents{
        QColor(0x5e, 0x81, 0xac),
        QColor(0x1e, 0x3a, 0x8a),
        QColor(0xff, 0xff, 0xff),
        QColor(0x4b, 0x55, 0x63),
        QColor(0x6b, 0x72, 0x80),
        QColor(0x14, 0x17, 0x1f),
        QColor(0x7a, 0xa2, 0xf7),
        QColor(0x9c, 0xa3, 0xaf)
    };
    return ThemePalette(QStringLiteral("obsidian_void"), QStringLiteral("Obsidian Void"), surface, accents);
}

ThemePalette ThemeRegistry::warmParchment() {
    const CanvasSurface surface{
        QColor(0xf4, 0xef, 0xe6),
        QColor(0xfb, 0xf8, 0xf2),
        QColor(0x2d, 0x2a, 0x26),
        QColor(0x9e, 0x96, 0x8b)
    };
    const SyntaxAccents accents{
        QColor(0xc8, 0x4b, 0x28),
        QColor(0xdf, 0xd5, 0xc5),
        QColor(0x1f, 0x1d, 0x1a),
        QColor(0x8c, 0x84, 0x79),
        QColor(0x78, 0x71, 0x67),
        QColor(0xef, 0xe9, 0xdd),
        QColor(0x9a, 0x34, 0x12),
        QColor(0x5c, 0x55, 0x4d)
    };
    return ThemePalette(QStringLiteral("warm_parchment"), QStringLiteral("Warm Parchment"), surface, accents);
}

ThemePalette ThemeRegistry::slateConcrete() {
    const CanvasSurface surface{
        QColor(0x14, 0x17, 0x1a),
        QColor(0x1a, 0x1d, 0x20),
        QColor(0xe6, 0xed, 0xf3),
        QColor(0x58, 0x60, 0x69)
    };
    const SyntaxAccents accents{
        QColor(0xf9, 0x73, 0x16),
        QColor(0x33, 0x41, 0x55),
        QColor(0xff, 0xff, 0xff),
        QColor(0x64, 0x74, 0x8b),
        QColor(0x94, 0xa3, 0xb8),
        QColor(0x22, 0x27, 0x2b),
        QColor(0x38, 0xbd, 0xf8),
        QColor(0x94, 0xa3, 0xb8)
    };
    return ThemePalette(QStringLiteral("slate_concrete"), QStringLiteral("Slate Concrete"), surface, accents);
}

ThemePalette ThemeRegistry::midnightIndigo() {
    const CanvasSurface surface{
        QColor(0x07, 0x0a, 0x12),
        QColor(0x0a, 0x0e, 0x17),
        QColor(0xe2, 0xe8, 0xf0),
        QColor(0x47, 0x55, 0x69)
    };
    const SyntaxAccents accents{
        QColor(0x38, 0xbd, 0xf8),
        QColor(0x1e, 0x3a, 0x8a),
        QColor(0xff, 0xff, 0xff),
        QColor(0x47, 0x55, 0x69),
        QColor(0x64, 0x74, 0x8b),
        QColor(0x11, 0x18, 0x27),
        QColor(0x7d, 0xd3, 0xfc),
        QColor(0x94, 0xa3, 0xb8)
    };
    return ThemePalette(QStringLiteral("midnight_indigo"), QStringLiteral("Midnight Indigo"), surface, accents);
}

const std::vector<ThemePalette> &ThemeRegistry::allPalettes() {
    static const std::vector<ThemePalette> palettes = {
        obsidianVoid(),
        warmParchment(),
        slateConcrete(),
        midnightIndigo()
    };
    return palettes;
}

ThemePalette ThemeRegistry::resolve(const QString &key) {
    for (const ThemePalette &palette : allPalettes()) {
        if (palette.key() == key) {
            return palette;
        }
    }
    return obsidianVoid();
}
