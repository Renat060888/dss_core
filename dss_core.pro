ROOT_DIR=../

TEMPLATE = app
TARGET = dss_core

include($${ROOT_DIR}pri/common.pri)

CONFIG -= qt
#CONFIG += release

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

# TODO: add defines to logger, system monitor, restbed webserver, database, etc...
DEFINES += \
    SWITCH_LOGGER_ASTRA \

LIBS += \
    -lpthread \
    -lrti1516e \ # TODO: wtf?
    -lfedtime1516e \ # TODO: wtf?
    -lFedTime \     # OpenRTI # TODO: wtf?
    -lOpenRTI \     # OpenRTI # TODO: wtf?
    -lRTI-NG \      # OpenRTI # TODO: wtf?
    -lboost_program_options \ # TODO: wtf?
    -ljsoncpp \ # TODO: replace by boost one
    -lunilog \  # TODO: wtf?
    -lobjrepr \  # TODO: wtf?
    -lmicroservice_common \
    -ldss_common

# NOTE: paths for dev environment ( all projects sources in one dir )
INCLUDEPATH +=  \
    /usr/include/libmongoc-1.0 \
    /usr/include/libbson-1.0 \
    $${ROOT_DIR}/microservice_common/ \ # TODO: avoidance of ugly traverse "../" for this lib's headers
    $${ROOT_DIR}/dss_common/ \ # TODO: avoidance of ugly traverse "../" for this lib's headers

SOURCES += \
        analyze/analytic_manager_facade.cpp \
        communication/command_factory.cpp \
        communication/commands/cmd_context_close.cpp \
        communication/commands/cmd_context_open.cpp \
        communication/commands/cmd_node_ping.cpp \
        communication/commands/cmd_user_ping.cpp \
        communication/commands/cmd_user_register.cpp \
        communication/communication_gateway_facade_dss.cpp \
        communication/unified_command_convertor_dss.cpp \
        datasource/context.cpp \
        datasource/i_node_mirror.cpp \
        datasource/node_dispatcher_dump.cpp \
        datasource/node_dispatcher_real.cpp \
        datasource/node_dispatcher_simulation.cpp \
        datasource/node_mirror_dump.cpp \
        datasource/node_mirror_real.cpp \
        datasource/node_mirror_simulation.cpp \
        datasource/realtime_synchronizer.cpp \
        datasource/source_manager_facade.cpp \
        datasource/user.cpp \
        datasource/user_dispatcher.cpp \
        datasource/visitor_node_configurator.cpp \
        distributed_simulation_system.cpp \
        main.cpp \
        storage/storage_engine_facade.cpp

HEADERS += \
    analyze/analytic_manager_facade.h \
    communication/command_factory.h \
    communication/commands/cmd_context_close.h \
    communication/commands/cmd_context_open.h \
    communication/commands/cmd_node_ping.h \
    communication/commands/cmd_user_ping.h \
    communication/commands/cmd_user_register.h \
    communication/communication_gateway_facade_dss.h \
    communication/unified_command_convertor_dss.h \
    datasource/context.h \
    datasource/i_node_mirror.h \
    datasource/node_dispatcher_dump.h \
    datasource/node_dispatcher_real.h \
    datasource/node_dispatcher_simulation.h \
    datasource/node_mirror_dump.h \
    datasource/node_mirror_real.h \
    datasource/node_mirror_simulation.h \
    datasource/realtime_synchronizer.h \
    datasource/source_manager_facade.h \
    datasource/user.h \
    datasource/user_dispatcher.h \
    datasource/visitor_node_configurator.h \
    distributed_simulation_system.h \
    storage/storage_engine_facade.h
