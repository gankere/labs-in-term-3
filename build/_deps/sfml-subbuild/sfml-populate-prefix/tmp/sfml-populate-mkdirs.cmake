# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-src"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-build"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/TPU_WorkSpace/3 term/lab 5/build/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
