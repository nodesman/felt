#pragma once

#include "theme.h"
#include <vector>

class ThemeRegistry {
public:
    static ThemePalette obsidianVoid();
    static ThemePalette warmParchment();
    static ThemePalette slateConcrete();
    static ThemePalette midnightIndigo();

    static const std::vector<ThemePalette> &allPalettes();
    static ThemePalette resolve(const QString &key);
};
