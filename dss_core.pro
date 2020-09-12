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
    SWITCH_LOGGER_SIMPLE \
#    SWITCH_LOGGER_ASTRA \
#    OBJREPR_LIBRARY_EXIST \
#    UNIT_TESTS_GOOGLE \

LIBS += \
    -lpthread \
    -lprotobuf \
    -lrti1516e \ # TODO: wtf?
    -lfedtime1516e \ # TODO: wtf?
    -lFedTime \     # OpenRTI # TODO: wtf?
    -lOpenRTI \     # OpenRTI # TODO: wtf?
    -lRTI-NG \      # OpenRTI # TODO: wtf?
    -lboost_program_options \ # TODO: wtf?
    -lboost_system \ # TODO: wtf?
    -lboost_filesystem \
    -lboost_regex \
    -ljsoncpp \ # TODO: replace by boost one
    -lmicroservice_common \
    -ldss_common

contains( DEFINES, OBJREPR_LIBRARY_EXIST ){
    message("connect 'unilog' and 'objrepr' libraries")
LIBS += \
    -lunilog \
    -lobjrepr
}

contains( DEFINES, UNIT_TESTS_GOOGLE ){
    message("connect 'gtests' library")
LIBS += \
    -lgtest
}

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
        communication/commands/cmd_node_agent_ping.cpp \
        communication/commands/cmd_user_ping.cpp \
        communication/commands/cmd_user_register.cpp \
        communication/communication_gateway_facade_dss.cpp \
        communication/unified_command_convertor_dss.cpp \
        datasource/context.cpp \
        datasource/dispatcher_node_dump.cpp \
        datasource/dispatcher_node_real.cpp \
        datasource/dispatcher_node_simulation.cpp \
        datasource/dispatcher_user.cpp \
        datasource/i_node_mirror.cpp \
        datasource/node_agent_dump_proxy.cpp \
        datasource/node_agent_real_proxy.cpp \
        datasource/node_agent_simulation_proxy.cpp \
        datasource/node_worker_service_dump.cpp \
        datasource/node_worker_service_real.cpp \
        datasource/node_worker_service_simulation.cpp \
        datasource/realtime_synchronizer.cpp \
        datasource/source_manager_facade.cpp \
        datasource/user.cpp \
        datasource/visitor_node_configurator.cpp \
        distributed_simulation_system.cpp \
        main.cpp \
        storage/storage_engine_facade.cpp \
    system/args_parser.cpp \
    system/config_reader.cpp \
    system/path_locator.cpp \
    communication/commands/cmd_user_configure_simulation.cpp \
    communication/commands/cmd_user_configure_real.cpp \
    communication/commands/cmd_user_processing_pause.cpp \
    communication/commands/cmd_user_processing_reset.cpp \
    communication/commands/cmd_user_processing_start.cpp \
    communication/commands/cmd_user_live_processing.cpp \
    system/system_environment_facade_core.cpp \
    system/objrepr_bus_core.cpp

HEADERS += \
    analyze/analytic_manager_facade.h \
    communication/command_factory.h \
    communication/commands/cmd_context_close.h \
    communication/commands/cmd_context_open.h \
    communication/commands/cmd_node_agent_ping.h \
    communication/commands/cmd_user_ping.h \
    communication/commands/cmd_user_register.h \
    communication/communication_gateway_facade_dss.h \
    communication/unified_command_convertor_dss.h \
    datasource/context.h \
    datasource/dispatcher_node_dump.h \
    datasource/dispatcher_node_real.h \
    datasource/dispatcher_node_simulation.h \
    datasource/dispatcher_user.h \
    datasource/i_node_worker_service.h \
    datasource/node_agent_dump_proxy.h \
    datasource/node_agent_real_proxy.h \
    datasource/node_agent_simulation_proxy.h \
    datasource/node_worker_service_dump.h \
    datasource/node_worker_service_real.h \
    datasource/node_worker_service_simulation.h \
    datasource/realtime_synchronizer.h \
    datasource/source_manager_facade.h \
    datasource/user.h \
    datasource/visitor_node_configurator.h \
    distributed_simulation_system.h \
    storage/storage_engine_facade.h \
    system/args_parser.h \
    system/config_reader.h \
    system/path_locator.h \
    communication/commands/cmd_user_configure_simulation.h \
    communication/commands/cmd_user_configure_real.h \
    communication/commands/cmd_user_processing_pause.h \
    communication/commands/cmd_user_processing_reset.h \
    communication/commands/cmd_user_processing_start.h \
    communication/commands/cmd_user_live_processing.h \
    system/system_environment_facade_core.h \
    system/objrepr_bus_core.h
