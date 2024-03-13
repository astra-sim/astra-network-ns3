include_directories("../src/applications")
include_directories("../../../../../")
include_directories("../../../../../extern/graph_frontend/chakra")
include_directories("../../../../../extern/remote_memory_backend/analytical")
include_directories("../../../../../extern/helper")

file(GLOB_RECURSE ASTRA_SIM_CC_FILES "../../../../../astra-sim/*.cc")
file(GLOB_RECURSE NETWORK_FRONTEND_CC_FILES "../../../../../astra-sim/network_frontend/*/*.cc")
list(REMOVE_ITEM ASTRA_SIM_CC_FILES ${NETWORK_FRONTEND_CC_FILES})

file(GLOB_RECURSE ET_DEF_CC_FILES "../../../../../extern/graph_frontend/chakra/et_def/*.cc")
file(GLOB_RECURSE ET_FEEDER_CC_FILES "../../../../../extern/graph_frontend/chakra/et_feeder/*.cpp")
file(GLOB_RECURSE THIRD_PARTY_CC_FILES "../../../../../extern/graph_frontend/chakra/third_party/utils/*.cc")
file(GLOB_RECURSE REMOTE_MEMORY_BACKEND_CC_FILES "../../../../../extern/remote_memory_backend/analytical/*.cc")

set(ASTRA_SIM_CC_FILES
  ${ASTRA_SIM_CC_FILES}
  ${ET_DEF_CC_FILES}
  ${ET_FEEDER_CC_FILES}
  ${THIRD_PARTY_CC_FILES}
  ${REMOTE_MEMORY_BACKEND_CC_FILES}
)

file(GLOB_RECURSE ASTRA_SIM_HH_FILES "../../../../../astra-sim/*.hh")
file(GLOB_RECURSE NETWORK_FRONTEND_HH_FILES "../../../../../astra-sim/network_frontend/*/*.hh")
list(REMOVE_ITEM ASTRA_SIM_HH_FILES ${NETWORK_FRONTEND_HH_FILES})


foreach(hh_file ${ASTRA_SIM_HH_FILES})
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" modified_hh_file ${hh_file})
  list(APPEND MODIFIED_ASTRA_SIM_HH_FILES ${modified_hh_file})
endforeach()

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

build_lib(
  LIBNAME applications
  SOURCE_FILES
    helper/bulk-send-helper.cc
    helper/on-off-helper.cc
    helper/packet-sink-helper.cc
    helper/rdma-client-helper.cc
    helper/three-gpp-http-helper.cc
    helper/udp-client-server-helper.cc
    helper/udp-echo-helper.cc
    model/application-packet-probe.cc
    model/bulk-send-application.cc
    model/onoff-application.cc
    model/packet-loss-counter.cc
    model/packet-sink.cc
    model/rdma-client.cc
    model/seq-ts-echo-header.cc
    model/seq-ts-header.cc
    model/seq-ts-size-header.cc
    model/three-gpp-http-client.cc
    model/three-gpp-http-header.cc
    model/three-gpp-http-server.cc
    model/three-gpp-http-variables.cc
    model/udp-client.cc
    model/udp-echo-client.cc
    model/udp-echo-server.cc
    model/udp-server.cc
    model/udp-trace-client.cc
    ${ASTRA_SIM_CC_FILES}
  HEADER_FILES
    helper/bulk-send-helper.h
    helper/on-off-helper.h
    helper/packet-sink-helper.h
    helper/rdma-client-helper.h
    helper/three-gpp-http-helper.h
    helper/udp-client-server-helper.h
    helper/udp-echo-helper.h
    model/application-packet-probe.h
    model/bulk-send-application.h
    model/onoff-application.h
    model/packet-loss-counter.h
    model/packet-sink.h
    model/rdma-client.h
    model/seq-ts-echo-header.h
    model/seq-ts-header.h
    model/seq-ts-size-header.h
    model/three-gpp-http-client.h
    model/three-gpp-http-header.h
    model/three-gpp-http-server.h
    model/three-gpp-http-variables.h
    model/udp-client.h
    model/udp-echo-client.h
    model/udp-echo-server.h
    model/udp-server.h
    model/udp-trace-client.h
    ${MODIFIED_ASTRA_SIM_HH_FILES}
  LIBRARIES_TO_LINK ${libinternet}
                    ${libpoint-to-point}
                    ${Protobuf_LIBRARIES}
  TEST_SOURCES
    test/three-gpp-http-client-server-test.cc
    test/bulk-send-application-test-suite.cc
    test/udp-client-server-test.cc
)


