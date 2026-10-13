#pragma once

#include <QString>

struct DocumentStats {
    int wordCount = 0;
    int charCount = 0;
    int charNoSpacesCount = 0;
    int readingTimeMinutes = 0;

    static DocumentStats compute(const QString &text);
    static QString formatReadingTime(int minutes);
    static QString formatSummary(const DocumentStats &stats);
};
