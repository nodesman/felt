#include "stats.h"
#include <QRegularExpression>
#include <algorithm>

DocumentStats DocumentStats::compute(const QString &text) {
    DocumentStats stats;
    if (text.isEmpty()) {
        return stats;
    }

    stats.charCount = text.length();

    int nonSpaces = 0;
    for (const QChar &ch : text) {
        if (!ch.isSpace()) {
            nonSpaces++;
        }
    }
    stats.charNoSpacesCount = nonSpaces;

    // Word count via regex splitting on whitespace
    static const QRegularExpression wordRegex(QStringLiteral("\\s+"));
    const QString trimmed = text.trimmed();
    if (!trimmed.isEmpty()) {
        const QStringList tokens = trimmed.split(wordRegex, Qt::SkipEmptyParts);
        stats.wordCount = tokens.size();
    } else {
        stats.wordCount = 0;
    }

    // Standard reading speed: 200 words per minute
    if (stats.wordCount > 0) {
        stats.readingTimeMinutes = std::max(1, (stats.wordCount + 150) / 200);
    } else {
        stats.readingTimeMinutes = 0;
    }

    return stats;
}

QString DocumentStats::formatReadingTime(int minutes) {
    if (minutes <= 1) {
        return QStringLiteral("1 min read");
    }
    return QString::asprintf("%d min read", minutes);
}

QString DocumentStats::formatSummary(const DocumentStats &stats) {
    if (stats.wordCount == 0) {
        return QStringLiteral("0 words");
    }

    const QString wordUnit = (stats.wordCount == 1) ? QStringLiteral("word") : QStringLiteral("words");
    const QString timeStr = formatReadingTime(stats.readingTimeMinutes);

    return QString::asprintf("%d %s  ·  %s",
                            stats.wordCount,
                            wordUnit.toUtf8().constData(),
                            timeStr.toUtf8().constData());
}
