#include "measure.h"
#include <algorithm>

DocumentMeasure::DocumentMeasure(int requestedColumnWidth)
    : m_columnWidth(clampColumnWidth(requestedColumnWidth))
{
}

int DocumentMeasure::columnWidth() const {
    return m_columnWidth;
}

int DocumentMeasure::clampColumnWidth(int width) {
    return std::clamp(width, kMinColumnWidth, kMaxColumnWidth);
}

ViewportMargins DocumentMeasure::computeMargins(int windowWidth) const {
    if (windowWidth <= kMinSideMargin * 2) {
        const int safeSide = std::max(0, windowWidth / 4);
        return {safeSide, kVerticalMargin, safeSide, kVerticalMargin};
    }
    if (windowWidth <= m_columnWidth + kMinSideMargin * 2) {
        return {kMinSideMargin, kVerticalMargin, kMinSideMargin, kVerticalMargin};
    }
    const int centeredSide = (windowWidth - m_columnWidth) / 2;
    return {centeredSide, kVerticalMargin, centeredSide, kVerticalMargin};
}
