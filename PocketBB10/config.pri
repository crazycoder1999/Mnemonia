# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/ActiveFrame.cpp) \
                 $$quote($$BASEDIR/src/BlackSound.cpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.cpp) \
                 $$quote($$BASEDIR/src/MyIndexMapper.cpp) \
                 $$quote($$BASEDIR/src/MySqlDataModel.cpp) \
                 $$quote($$BASEDIR/src/StatusEventHandler.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/ActiveFrame.h) \
                 $$quote($$BASEDIR/src/BlackSound.hpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.h) \
                 $$quote($$BASEDIR/src/MyIndexMapper.h) \
                 $$quote($$BASEDIR/src/MySqlDataModel.h) \
                 $$quote($$BASEDIR/src/StatusEventHandler.h)
    }

    CONFIG(release, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/ActiveFrame.cpp) \
                 $$quote($$BASEDIR/src/BlackSound.cpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.cpp) \
                 $$quote($$BASEDIR/src/MyIndexMapper.cpp) \
                 $$quote($$BASEDIR/src/MySqlDataModel.cpp) \
                 $$quote($$BASEDIR/src/StatusEventHandler.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/ActiveFrame.h) \
                 $$quote($$BASEDIR/src/BlackSound.hpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.h) \
                 $$quote($$BASEDIR/src/MyIndexMapper.h) \
                 $$quote($$BASEDIR/src/MySqlDataModel.h) \
                 $$quote($$BASEDIR/src/StatusEventHandler.h)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        SOURCES +=  $$quote($$BASEDIR/src/ActiveFrame.cpp) \
                 $$quote($$BASEDIR/src/BlackSound.cpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.cpp) \
                 $$quote($$BASEDIR/src/MyIndexMapper.cpp) \
                 $$quote($$BASEDIR/src/MySqlDataModel.cpp) \
                 $$quote($$BASEDIR/src/StatusEventHandler.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/ActiveFrame.h) \
                 $$quote($$BASEDIR/src/BlackSound.hpp) \
                 $$quote($$BASEDIR/src/HtmlRemoval.h) \
                 $$quote($$BASEDIR/src/MyIndexMapper.h) \
                 $$quote($$BASEDIR/src/MySqlDataModel.h) \
                 $$quote($$BASEDIR/src/StatusEventHandler.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs) \
             $$quote($$BASEDIR/../assets/720x720/*.qml) \
             $$quote($$BASEDIR/../assets/720x720/*.js) \
             $$quote($$BASEDIR/../assets/720x720/*.qs) \
             $$quote($$BASEDIR/../assets/720x720/images/*.qml) \
             $$quote($$BASEDIR/../assets/720x720/images/*.js) \
             $$quote($$BASEDIR/../assets/720x720/images/*.qs) \
             $$quote($$BASEDIR/../assets/images/*.qml) \
             $$quote($$BASEDIR/../assets/images/*.js) \
             $$quote($$BASEDIR/../assets/images/*.qs) \
             $$quote($$BASEDIR/../assets/models/*.qml) \
             $$quote($$BASEDIR/../assets/models/*.js) \
             $$quote($$BASEDIR/../assets/models/*.qs) \
             $$quote($$BASEDIR/../assets/sql/*.qml) \
             $$quote($$BASEDIR/../assets/sql/*.js) \
             $$quote($$BASEDIR/../assets/sql/*.qs) \
             $$quote($$BASEDIR/../assets/web/*.qml) \
             $$quote($$BASEDIR/../assets/web/*.js) \
             $$quote($$BASEDIR/../assets/web/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)
