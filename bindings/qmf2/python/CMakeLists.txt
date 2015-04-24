#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

##------------------------------------------------------
## Use Swig to generate a literal binding to the C++ API
##------------------------------------------------------
<<<<<<< HEAD
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/python.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/python.i PROPERTIES SWIG_FLAGS "-I${qpid-cpp_SOURCE_DIR}/include")

swig_add_module(cqmf2_python python ${CMAKE_CURRENT_SOURCE_DIR}/python.i)
swig_link_libraries(cqmf2_python qmf2 ${PYTHON_LIBRARIES})

set_source_files_properties(${swig_generated_file_fullname} PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing -I${PYTHON_INCLUDE_PATH} -I${qpid-cpp_SOURCE_DIR}/include")

##------------------------------------
## Install the complete Python binding
##------------------------------------
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
=======

# NB For python the SWIG module name must have the same name as the input .i file for CMake to generate the
# correct dependencies

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/cqmf2.i PROPERTIES
    CPLUSPLUS ON
    SWIG_FLAGS "-I${qpid-cpp_SOURCE_DIR}/include;-I${qpid-cpp_SOURCE_DIR}/bindings")

list(APPEND SWIG_MODULE_cqmf2_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/include/qmf/qmf2.i
    ${CMAKE_SOURCE_DIR}/include/qpid/swig_python_typemaps.i
)
swig_add_module(cqmf2 python ${CMAKE_CURRENT_SOURCE_DIR}/cqmf2.i)
swig_link_libraries(cqmf2 qmf2 qpidmessaging qpidtypes ${PYTHON_LIBRARIES})

set_source_files_properties(${swig_generated_file_fullname} PROPERTIES COMPILE_FLAGS "${NOSTRICT_ALIASING}")
include_directories(${PYTHON_INCLUDE_PATH}
                    ${qpid-cpp_SOURCE_DIR}/include
                    ${qpid-cpp_SOURCE_DIR}/bindings)

# Move source into binary dir so compiled .pyc,pyo files will be in binary dir.
# NOTE: not using the file(COPY) command as it is not available in cmake 2.6
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/qmf2.py" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)

# Python compile the modules
>>>>>>> 3bbfc42... Imported Upstream version 0.32
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m py_compile cqmf2.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -O -m py_compile cqmf2.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")
<<<<<<< HEAD
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/qmf2.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
=======
>>>>>>> 3bbfc42... Imported Upstream version 0.32
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m py_compile qmf2.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")
install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -O -m py_compile qmf2.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")
<<<<<<< HEAD
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.py
              ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.pyc
              ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.pyo
              ${CMAKE_CURRENT_BINARY_DIR}/qmf2.py
              ${CMAKE_CURRENT_BINARY_DIR}/qmf2.pyc
              ${CMAKE_CURRENT_BINARY_DIR}/qmf2.pyo
        DESTINATION ${PYTHON_SITE_PACKAGES}
        COMPONENT ${QPID_COMPONENT_CLIENT}
        )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_cqmf2_python.so
        RENAME _cqmf2.so
        DESTINATION ${PYTHON_SITE_PACKAGES}
=======
##------------------------------------
## Install the complete Python binding
##------------------------------------
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.py
  ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.pyc
  ${CMAKE_CURRENT_BINARY_DIR}/cqmf2.pyo
  ${CMAKE_CURRENT_SOURCE_DIR}/qmf2.py
  ${CMAKE_CURRENT_BINARY_DIR}/qmf2.pyc
  ${CMAKE_CURRENT_BINARY_DIR}/qmf2.pyo
  DESTINATION ${PYTHON_SITEARCH_PACKAGES}
  COMPONENT ${QPID_COMPONENT_CLIENT}
  )

install(TARGETS ${SWIG_MODULE_cqmf2_REAL_NAME}
        DESTINATION ${PYTHON_SITEARCH_PACKAGES}
>>>>>>> 3bbfc42... Imported Upstream version 0.32
        COMPONENT ${QPID_COMPONENT_CLIENT}
        )

