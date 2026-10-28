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

    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return stats;
    }

    static const QRegularExpression wordRegex(QStringLiteral("\\s+"));
    const QStringList tokens = trimmed.split(wordRegex, Qt::SkipEmptyParts);
    stats.wordCount = tokens.size();
    stats.readingTimeMinutes = std::max(1, (stats.wordCount + 150) / 200);
    return stats;
}

QString DocumentStats::formatReadingTime(int minutes) {
    if (minutes <= 0) {
        return QStringLiteral("0 min read");
    }
    if (minutes == 1) {
        return QStringLiteral("1 min read");
    }
    return QString::asprintf("%d min read", minutes);
}

QString DocumentStats::formatSummary(const DocumentStats &stats) {
    if (stats.wordCount <= 0) {
        return QStringLiteral("0 words · 0 min read");
    }
    const QString timeStr = formatReadingTime(stats.readingTimeMinutes);
    if (stats.wordCount == 1) {
        return QString::asprintf("1 word · %s", timeStr.toUtf8().constData());
    }
    return QString::asprintf("%d words · %s",
                            stats.wordCount,
                            timeStr.toUtf8().constData());
}
