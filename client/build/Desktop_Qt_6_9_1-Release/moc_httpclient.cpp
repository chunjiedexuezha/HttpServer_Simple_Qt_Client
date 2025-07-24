/****************************************************************************
** Meta object code from reading C++ file 'httpclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../httpclient.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10HttpClientE_t {};
} // unnamed namespace

template <> constexpr inline auto HttpClient::qt_create_metaobjectdata<qt_meta_tag_ZN10HttpClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HttpClient",
        "requestCompleted",
        "",
        "success",
        "registerSuccess",
        "loginSuccess",
        "ID",
        "connectionSuccess",
        "genericSuccess",
        "message",
        "GameBegin",
        "receivedOpponentMove",
        "row",
        "col",
        "registerFailed",
        "loginFailed",
        "genericFailed",
        "PvPMatchSuccess",
        "gameId",
        "roleStr",
        "PvPWaiting",
        "PvPMoveSuccess"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'requestCompleted'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'registerSuccess'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'loginSuccess'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'connectionSuccess'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'genericSuccess'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Signal 'GameBegin'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'receivedOpponentMove'
        QtMocHelpers::SignalData<void(int, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'registerFailed'
        QtMocHelpers::SignalData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Signal 'genericFailed'
        QtMocHelpers::SignalData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Signal 'PvPMatchSuccess'
        QtMocHelpers::SignalData<void(int, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 19 },
        }}),
        // Signal 'PvPWaiting'
        QtMocHelpers::SignalData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'PvPMoveSuccess'
        QtMocHelpers::SignalData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HttpClient, qt_meta_tag_ZN10HttpClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HttpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HttpClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HttpClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10HttpClientE_t>.metaTypes,
    nullptr
} };

void HttpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HttpClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->requestCompleted((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->registerSuccess(); break;
        case 2: _t->loginSuccess((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->connectionSuccess((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->genericSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->GameBegin(); break;
        case 6: _t->receivedOpponentMove((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->registerFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->loginFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->genericFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->PvPMatchSuccess((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->PvPWaiting(); break;
        case 12: _t->PvPMoveSuccess(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(bool )>(_a, &HttpClient::requestCompleted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)()>(_a, &HttpClient::registerSuccess, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(int )>(_a, &HttpClient::loginSuccess, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(bool )>(_a, &HttpClient::connectionSuccess, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(const QString & )>(_a, &HttpClient::genericSuccess, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)()>(_a, &HttpClient::GameBegin, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(int , int )>(_a, &HttpClient::receivedOpponentMove, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(const QString & )>(_a, &HttpClient::registerFailed, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(const QString & )>(_a, &HttpClient::loginFailed, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(const QString & )>(_a, &HttpClient::genericFailed, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)(int , const QString & )>(_a, &HttpClient::PvPMatchSuccess, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)()>(_a, &HttpClient::PvPWaiting, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpClient::*)()>(_a, &HttpClient::PvPMoveSuccess, 12))
            return;
    }
}

const QMetaObject *HttpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HttpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10HttpClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HttpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void HttpClient::requestCompleted(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void HttpClient::registerSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HttpClient::loginSuccess(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void HttpClient::connectionSuccess(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void HttpClient::genericSuccess(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void HttpClient::GameBegin()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void HttpClient::receivedOpponentMove(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void HttpClient::registerFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void HttpClient::loginFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void HttpClient::genericFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void HttpClient::PvPMatchSuccess(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void HttpClient::PvPWaiting()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void HttpClient::PvPMoveSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
