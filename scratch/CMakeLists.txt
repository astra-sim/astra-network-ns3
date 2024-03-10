set(astra-sim-ns3-libs-list
  "libapplications"
  "libcore"
  "libcsma"
  "libinternet"
  "libpoint-to-point"
  "libnetwork"
)
list(JOIN astra-sim-ns3-libs-list " " astra-sim-ns3-libs)
set(astra-sim-dir "${PROJECT_SOURCE_DIR}/../../../")
include_directories("../scratch")

build_exec(
  EXECNAME AstraSimNetwork
  SOURCE_FILES ${astra-sim-dir}/astra-sim/network_frontend/ns3/AstraSimNetwork.cc
  LIBRARIES_TO_LINK "${astra-sim-ns3-libs-list}"
  EXECUTABLE_DIRECTORY_PATH "${PROJECT_SOURCE_DIR}/build/scratch/"
)
