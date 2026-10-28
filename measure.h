#pragma once

struct ViewportMargins {
    int left = 40;
    int top = 80;
    int right = 40;
    int bottom = 80;
};

class DocumentMeasure {
public:
    static constexpr int kMinColumnWidth = 520;
    static constexpr int kMaxColumnWidth = 960;
    static constexpr int kDefaultColumnWidth = 720;
    static constexpr int kMinSideMargin = 40;
    static constexpr int kVerticalMargin = 80;

    explicit DocumentMeasure(int requestedColumnWidth = kDefaultColumnWidth);

    int columnWidth() const;
    ViewportMargins computeMargins(int windowWidth) const;

    static int clampColumnWidth(int width);

private:
    int m_columnWidth;
};
