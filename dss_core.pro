ROOT_DIR=../

TEMPLATE = app
TARGET = distributed_simulation_system

include($${ROOT_DIR}pri/common.pri)

CONFIG -= qt
#CONFIG += release

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

DEFINES += \
    SWITCH_LOGGER_SIMPLE \

LIBS += \
    -lpthread \
    -lrti1516e \ # TODO: wtf?
    -lfedtime1516e \ # TODO: wtf?
    -lFedTime \     # OpenRTI # TODO: wtf?
    -lOpenRTI \     # OpenRTI # TODO: wtf?
    -lRTI-NG \      # OpenRTI # TODO: wtf?
    -lboost_program_options \ # TODO: wtf?
    -ljsoncpp \ # TODO: replace by boost one
    -lmicroservice_common \
    -ldss_common

# NOTE: paths for dev environment ( all projects sources in one dir )
INCLUDEPATH +=  \
    $${ROOT_DIR}/microservice_common/ \

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
        datasource/source_manager.cpp \
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
    datasource/source_manager.h \
    datasource/user.h \
    datasource/user_dispatcher.h \
    datasource/visitor_node_configurator.h \
    distributed_simulation_system.h \
    storage/storage_engine_facade.h
