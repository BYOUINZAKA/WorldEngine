#ifndef __TOPOGRAPHY_TOPOGRAPHY_H
#define __TOPOGRAPHY_TOPOGRAPHY_H

#include <cassert>

#include <QObject>
#include <QVector>

#include "unit/area.h"

class Topography : public QObject {
    Q_OBJECT
public:
    using ValueType = Area;

    // 采用一维数组来模拟矩阵操作
    using MapType = QVector<ValueType>;

    using SizeType = MapType::size_type;
    using Iterator = MapType::Iterator;

    explicit Topography(QObject* parent = nullptr);
    explicit Topography(SizeType _length, SizeType _width, QObject* parent = nullptr);
    ~Topography() = default;

    float avgHeight() const;

signals:
    void refreshed();

public slots:

private:
    SizeType length;
    SizeType width;
    MapType map;

public:
    // 可接受具有成员函数x()和y()的向量类型（如QPoint和QPointF）
    // @Todo: 应改为由 std::declval 调用检测的形式，泛用性更好
    template <typename T, typename U = typename std::remove_reference<T>::type,
              typename X = decltype(&U::x), typename Y = decltype(&U::y)>
    constexpr bool accepted(T&& point) const {
        return accepted(static_cast<SizeType>(std::forward<T>(point).y()),
                        static_cast<SizeType>(std::forward<T>(point).x()));
    }

    template <typename T, typename U = typename std::remove_reference<T>::type,
              typename X = decltype(&U::x), typename Y = decltype(&U::y)>
    constexpr const ValueType& at(T&& point) const& {
        return at(static_cast<SizeType>(std::forward<T>(point).y()),
                  static_cast<SizeType>(std::forward<T>(point).x()));
    }

    inline ValueType& at(SizeType i, SizeType j) {
#ifdef QT_DEBUG
        assert(i >= 0 && i < getLength() && j >= 0 && j < getWidth());
#endif
        return map[i * width + j];
    }

    inline const ValueType& at(SizeType i, SizeType j) const {
#ifdef QT_DEBUG
        assert(i >= 0 && i < getLength() && j >= 0 && j < getWidth());
#endif
        return map[i * width + j];
    }

    inline MapType::Iterator at(SizeType i) {
#ifdef QT_DEBUG
        assert(i >= 0 && i < getLength());
#endif
        return map.begin() + i * width;
    }

    inline MapType::ConstIterator at(SizeType i) const {
#ifdef QT_DEBUG
        assert(i >= 0 && i < getLength());
#endif
        return map.cbegin() + i * width;
    }

    inline bool accepted(SizeType i) const { return i >= 0 && i < getLength(); }

    inline bool accepted(SizeType i, SizeType j) const {
        return accepted(i) && j >= 0 && j < getWidth();
    }

    inline MapType::Iterator operator[](SizeType i) { return this->at(i); }

    inline MapType::ConstIterator operator[](SizeType i) const { return this->at(i); }

    inline SizeType getLength() const { return length; }

    inline SizeType getWidth() const { return width; }

    inline SizeType getRealSize() const { return width * length; }

    inline MapType::Iterator begin() { return map.begin(); }

    inline MapType::Iterator end() { return map.end(); }
};

#endif  // __TOPOGRAPHY_TOPOGRAPHY_H
