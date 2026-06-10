
function(apply_project_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /w14242   
            /w14254   
            /w14263  
            /w14265   
            /w14287   
            /we4289   
            /w14296   
            /w14311   
            /w14545   
            /w14546   
            /w14547   
            /w14549   
            /w14555   
            /w14619   
            /w14640   
            /w14826  
            /w14905   
            /w14906   
            /w14928   
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
