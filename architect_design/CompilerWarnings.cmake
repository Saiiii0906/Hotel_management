# Centralised warning flags.
# Usage: apply_project_warnings(<target>)
#
# Keep -Wno-unused-parameter during active development — stubs are common.
# Remove it before final submission.

function(apply_project_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /w14242   # 'identifier': conversion, possible loss of data
            /w14254   # 'operator': conversion, possible loss of data
            /w14263   # member function does not override base class virtual
            /w14265   # class has virtual functions but destructor is not virtual
            /w14287   # unsigned/negative constant mismatch
            /we4289   # loop control variable used outside for-loop scope
            /w14296   # 'operator': expression is always false/true
            /w14311   # pointer truncation from 'type1' to 'type2'
            /w14545   # expression before comma evaluates to function missing argument list
            /w14546   # function call before comma missing argument list
            /w14547   # 'operator': operator before comma has no effect
            /w14549   # 'operator': operator before comma has no effect; did you intend 'operator'?
            /w14555   # expression has no effect; expected expression with side-effect
            /w14619   # pragma warning: there is no warning number 'number'
            /w14640   # Enable warning on thread un-safe static member initialization
            /w14826   # Conversion from 'type1' to 'type2' is sign-extended
            /w14905   # wide string literal cast to 'LPSTR'
            /w14906   # string literal cast to 'LPWSTR'
            /w14928   # illegal copy-initialization; more than one user-defined conversion
            /permissive-
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wdouble-promotion
            -Wformat=2
            -Wno-unused-parameter
        )

        # GCC-only flags
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${target} PRIVATE
                -Wmisleading-indentation
                -Wduplicated-cond
                -Wduplicated-branches
                -Wlogical-op
                -Wnull-dereference
                -Wuseless-cast
            )
        endif()
    endif()
endfunction()
