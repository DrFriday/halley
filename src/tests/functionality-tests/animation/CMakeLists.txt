cmake_minimum_required (VERSION 3.0)

project (halley-test-animation)

set (animation_test_sources
	"prec.cpp"

	"src/main.cpp"
	"src/test_stage.cpp"
	)

set (animation_test_headers
	"prec.h"
	"src/test_stage.h"
	)

set (animation_test_gen_definitions
	"gen_src/test.yaml"
	)

halleyProjectCodegen(halley-test-animation "${animation_test_sources}" "${animation_test_headers}" "${animation_test_gen_definitions}" ${CMAKE_CURRENT_SOURCE_DIR}/bin)
